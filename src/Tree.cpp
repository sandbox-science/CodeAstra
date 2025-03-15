#include "Tree.h"
#include "MainWindow.h"
#include "CodeEditor.h"

#include <QFileDialog>
#include <QAbstractItemView>
#include <QFileInfo>

Tree::Tree(QSplitter *splitter, MainWindow *mainWindow) : QObject(splitter), mainWindow(mainWindow)
{
  model = new QFileSystemModel();
  tree  = new QTreeView(splitter);

  setupModel();
  setupTree();

  connect(tree, &QTreeView::doubleClicked, this, &Tree::openFile);
}

Tree::~Tree() {}

void Tree::setupModel()
{
  model->setRootPath(getDirectoryPath());
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

  for (int i = 1; i <= 3; ++i)
  {
    tree->setColumnHidden(i, true);
  }
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

  if (fileInfo.isFile()) // Ensure it's a file, not a folder
  {
    mainWindow->loadFileInEditor(filePath);
  }
}
