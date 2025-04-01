#include "Tree.h"
#include "CodeEditor.h"
#include "FileManager.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QTreeView>
#include <QMenu>
#include <filesystem>
#include <QMessageBox>

Tree::Tree(QSplitter *splitter)
    : QObject(splitter),
      m_iconProvider(std::make_unique<QFileIconProvider>()),
      m_model(std::make_unique<QFileSystemModel>()),
      m_tree(std::make_unique<QTreeView>(splitter))
{
    connect(m_tree.get(), &QTreeView::doubleClicked, this, &Tree::openFile);
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
}

void Tree::setupTree()
{
    m_tree->setModel(m_model.get());
    m_tree->setRootIndex(m_model->index(m_model->rootPath()));
    m_tree->setRootIsDecorated(true);
    m_tree->setAnimated(true);
    m_tree->setIndentation(20);
    m_tree->setSortingEnabled(false);
    m_tree->sortByColumn(1, Qt::AscendingOrder);
    m_tree->setHeaderHidden(true);

    for (int i = 1; i <= m_model->columnCount(); ++i)
    {
        m_tree->setColumnHidden(i, true);
    }

    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tree.get(), &QTreeView::customContextMenuRequested, this, &Tree::showContextMenu);
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

    FileManager::getInstance().setCurrentFileName(filePath);
    FileManager::getInstance().loadFileInEditor(filePath);
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
    QAction *renameAction    = contextMenu.addAction("Rename");
    QAction *deleteAction    = contextMenu.addAction("Delete");

    QAction *selectedAction = contextMenu.exec(m_tree->viewport()->mapToGlobal(pos));

    if (selectedAction == newFileAction)
    {
        // TO-DO: implement file creation
    }
    else if (selectedAction == newFolderAction)
    {
        // TO-DO: implement folder creation
    }
    else if (selectedAction == renameAction)
    {
        // TO-DO: implement rename file/folder
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
            if(pathInfo.isDir())
            {
                deleteFolder(pathInfo);
            }
            else
            {
                deleteFile(pathInfo);
            }
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

bool Tree::deleteFile(const QFileInfo &pathInfo)
{
    std::error_code err;
    QString filePath = pathInfo.absoluteFilePath();

    if (std::filesystem::remove(filePath.toStdString(), err))
    {
        qInfo() << "Successfully deleted" << pathInfo.fileName();
    }
    else
    {
        qWarning() << "Failed to delete" << pathInfo.fileName() << "- Error:" << QString::fromStdString(err.message());
        return false;
    }

    return true;
}

bool Tree::deleteFolder(const QFileInfo &pathInfo)
{
    std::error_code err;
    QString filePath = pathInfo.absoluteFilePath();

    if (std::filesystem::remove_all(filePath.toStdString(), err))
    {
        qInfo() << "Successfully deleted" << pathInfo.fileName();
    }
    else
    {
        qWarning() << "Failed to delete" << pathInfo.fileName() << "- Error:" << QString::fromStdString(err.message());
        return false;
    }
    return true;
}

bool Tree::renameFile(const QFileInfo &filePath, QString newFileName)
{
    qWarning() << "renamedFile Function not yet implemented";
    return false;
}

bool Tree::newFile(QString newFilePath)
{
    qWarning() << "newFile Function not yet implemented";
    return false;
}