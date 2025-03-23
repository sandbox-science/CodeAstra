#include "Tree.h"
#include "CodeEditor.h"
#include "FileManager.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QTreeView>
#include <QMenu>

Tree::Tree(QSplitter *splitter, FileManager *FileManager)
    : QObject(splitter),
      m_iconProvider(std::make_unique<QFileIconProvider>()),
      m_model(std::make_unique<QFileSystemModel>()),
      m_tree(std::make_unique<QTreeView>(splitter)),
      m_FileManager(FileManager)
{
    setupModel();
    setupTree();

    connect(m_tree.get(), &QTreeView::doubleClicked, this, &Tree::openFile);
}

Tree::~Tree() {}

void Tree::setupModel()
{
    m_model->setRootPath(getDirectoryPath());
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

QString Tree::getDirectoryPath() const
{
    return QFileDialog::getExistingDirectory(
        nullptr, QObject::tr("Open Directory"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
        // TO-DO: implement file deletion
    }
}
