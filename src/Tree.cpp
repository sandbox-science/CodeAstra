#include "Tree.h"
#include "MainWindow.h"
#include "CodeEditor.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QTreeView>

Tree::Tree(QSplitter *splitter, MainWindow *mainWindow)
    : QObject(splitter),
      model(new QFileSystemModel()),
      tree(new QTreeView(splitter)),
      mainWindow(mainWindow)
{
  setupModel();
  setupTree();

  connect(tree, &QTreeView::doubleClicked, this, &Tree::openFile);
}

Tree::~Tree() {}

void Tree::setupModel()
{
  model->setRootPath(getDirectoryPath());
  model->setIconProvider(new QFileIconProvider);
  model->setFilter(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
}

void Tree::setupTree()
{
  tree->setModel(model);
  tree->setRootIndex(model->index(model->rootPath()));
  tree->setRootIsDecorated(true);
  tree->setAnimated(true);
  tree->setIndentation(20);
  tree->setSortingEnabled(false);
  tree->sortByColumn(1, Qt::AscendingOrder);
  tree->setHeaderHidden(true);

  for (int i = 1; i <= model->columnCount(); ++i)
  {
    tree->setColumnHidden(i, true);
  }

  tree->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(tree, &QTreeView::customContextMenuRequested, this, &Tree::showContextMenu);
}

QString Tree::getDirectoryPath()
{
  return QFileDialog::getExistingDirectory(
      nullptr, QObject::tr("Open Directory"), QDir::homePath(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void Tree::openFile(const QModelIndex &index)
{
  QString filePath = model->filePath(index);
  QFileInfo fileInfo(filePath);

  // Ensure it's a file, not a folder before loading
  if (fileInfo.isFile())
  {
    mainWindow->loadFileInEditor(filePath);
  }
}

void Tree::showContextMenu(const QPoint &pos)
{
  // TO_DO: Implement delete a file
  // TO_DO: Implement rename a file
  // TO_DO: Implement create a new file
  // TO_DO: Implement create a new folder

  // use pos param for testing purpose for now
  tree->indexAt(pos);
}
