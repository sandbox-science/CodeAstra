#include "Tree.h"
#include "CodeEditor.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QTreeView>
#include <QMenu>
#include <filesystem>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QHeaderView>
#include <QMimeData>

Tree::Tree(QSplitter *splitter)
    : QObject(splitter),
      m_iconProvider(std::make_unique<QFileIconProvider>()),
      m_model(std::make_unique<QFileSystemModel>()),
      m_tree(std::make_unique<QTreeView>(splitter))
{
    connect(m_tree.get(), &QTreeView::clicked, this, &Tree::openFile);
}

Tree::~Tree() {}

void Tree::initialize(const QString &directory)
{
    setupModel(directory);
    setupTree();
}

void Tree::setupModel(const QString &directory)
{
    m_model->setRootPath(directory);
    m_model->setIconProvider(m_iconProvider.get());
    m_model->setFilter(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    m_model->setReadOnly(false);
}

void Tree::setupTree()
{
    QFont baseFont = QApplication::font(); 
    QFont treeFont = baseFont;
    treeFont.setPointSize(baseFont.pointSize() - 1);

    m_tree->setFont(treeFont);
    m_tree->setModel(m_model.get());
    m_tree->setRootIndex(m_model->index(m_model->rootPath()));
    m_tree->setRootIsDecorated(true);
    m_tree->setAnimated(true);
    m_tree->setIndentation(16);
    m_tree->setSortingEnabled(false);
    m_tree->sortByColumn(1, Qt::AscendingOrder);
    m_tree->setHeaderHidden(true);
    m_tree->setUniformRowHeights(true);

    // Configure file/folder drag & drop
    m_tree->setDragDropMode(QAbstractItemView::InternalMove);
    m_tree->setDragEnabled(true);
    m_tree->setAcceptDrops(true);
    m_tree->setDropIndicatorShown(true);
    m_tree->setDefaultDropAction(Qt::MoveAction);
    m_tree->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 1; i <= m_model->columnCount(); ++i)
    {
        m_tree->setColumnHidden(i, true);
    }

    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tree.get(), &QTreeView::customContextMenuRequested, this, &Tree::showContextMenu);

    // Connect to dragEnter and dragMove events
    m_tree->installEventFilter(this);
}

bool Tree::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_tree.get())
    {
        if (event->type() == QEvent::DragEnter)
        {
            QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent *>(event);
            if (dragEvent->mimeData()->hasUrls())
            {
                dragEvent->acceptProposedAction();
                return true;
            }
        }

        else if (event->type() == QEvent::DragMove)
        {
            QDragMoveEvent *dragEvent = static_cast<QDragMoveEvent *>(event);
            if (dragEvent->mimeData()->hasUrls())
            {
                dragEvent->acceptProposedAction();
                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void Tree::openFile(const QModelIndex &index)
{
    QString filePath = m_model->filePath(index);
    QFileInfo fileInfo(filePath);

    // Ensure it's a file, not a folder before loading
    if (!fileInfo.exists() || !fileInfo.isFile())
    {
        qWarning() << "Selected index is not a valid file:" << filePath;
        return;
    }

    FileManager &fm = FileManager::getInstance();
    if (fm.getCurrentFileName() != filePath)
    {
        if (!fm.promptUnsavedChanges())
        {
            return; // if user has cancelled
        }

        fm.setCurrentFileName(filePath);
        fm.loadFileInEditor(filePath);
    }
}

QFileSystemModel *Tree::getModel() const
{
    if (!m_model)
        throw std::runtime_error("Tree model is not initialized!");
    return m_model.get();
}

// Context menu for file operations
// such as creating new files, folders, renaming, and deleting
// This function is called when the user right-clicks on the tree view
void Tree::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu;

    QAction *newFileAction   = contextMenu.addAction("New File");
    QAction *newFolderAction = contextMenu.addAction("New Folder");
    contextMenu.addSeparator();
    QAction *renameAction    = contextMenu.addAction("Rename");
    QAction *duplicateAction = contextMenu.addAction("Duplicate");
    contextMenu.addSeparator();
    QAction *deleteAction    = contextMenu.addAction("Delete");

    QAction *selectedAction = contextMenu.exec(m_tree->viewport()->mapToGlobal(pos));

    if (selectedAction == newFileAction)
    {
        QFileInfo pathInfo = getPathInfo();
        if (!pathInfo.exists())
        {
            qWarning() << "Path does not exist: " << pathInfo.fileName();
            return;
        }

        bool ok;
        QString newFileName = QInputDialog::getText(
            nullptr,
            "New File",
            "Enter file name:",
            QLineEdit::Normal,
            nullptr,
            &ok
        );

        if (ok && !newFileName.isEmpty())
        {
            OperationResult result = FileManager::getInstance().newFile(pathInfo, newFileName);
            isSuccessful(result);
        }
    }
    else if (selectedAction == newFolderAction)
    {
        QFileInfo pathInfo = getPathInfo();
        if (!pathInfo.exists())
        {
            qWarning() << "Path does not exist: " << pathInfo.fileName();
            return;
        }

        bool ok;
        QString newFolderName = QInputDialog::getText(
            nullptr,
            "New Folder",
            "Enter folder name:",
            QLineEdit::Normal,
            nullptr,
            &ok
        );

        if (ok && !newFolderName.isEmpty())
        {
            OperationResult result = FileManager::getInstance().newFolder(pathInfo, newFolderName);
            isSuccessful(result);
        }
    }
    else if (selectedAction == duplicateAction)
    {
        QFileInfo pathInfo = getPathInfo();
        if (!pathInfo.exists())
        {
            qWarning() << "File does not exist: " << pathInfo.fileName();
            return;
        }

        OperationResult result = FileManager::getInstance().duplicatePath(pathInfo);
        isSuccessful(result);
    }
    else if (selectedAction == renameAction)
    {
        QFileInfo oldPathInfo = getPathInfo();
        if (!oldPathInfo.exists())
        {
            qWarning() << "File does not exist: " << oldPathInfo.fileName();
            return;
        }

        bool ok;
        QString newFileName = QInputDialog::getText(
            nullptr,
            "Rename File",
            "Enter new file name:",
            QLineEdit::Normal,
            oldPathInfo.fileName(),
            &ok);

        if (ok && !newFileName.isEmpty())
        {
            OperationResult result = FileManager::getInstance().renamePath(oldPathInfo, newFileName);
            isSuccessful(result);
        }
    }
    else if (selectedAction == deleteAction)
    {
        QFileInfo pathInfo = getPathInfo();
        if (!pathInfo.exists())
        {
            qWarning() << "File does not exist: " << pathInfo.fileName();
            return;
        }
        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Confirm Deletion",
                                                                  "Are you sure you want to delete\n'" + pathInfo.fileName() + "'?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            qInfo() << "Deletion cancelled.";
        }
        else
        {
            OperationResult result = FileManager::getInstance().deletePath(pathInfo);
            isSuccessful(result);
        }
    }
}

QFileInfo Tree::getPathInfo()
{
    QModelIndex index = m_tree->currentIndex();
    if (!index.isValid())
    {
        qWarning() << "Invalid index.";
        return QFileInfo();
    }

    return QFileInfo(m_model->filePath(index));
}

void Tree::isSuccessful(OperationResult result)
{
    if (result.success)
    {
        qInfo() << QString::fromStdString(result.message);
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", QString::fromStdString(result.message));
    }
}