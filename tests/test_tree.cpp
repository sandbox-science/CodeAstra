#include <QtTest>
#include <QCoreApplication>
#include "Tree.h"
#include "FileManager.h"
#include "CodeEditor.h"

#include <QAction>
#include <QMenu>
#include <QTest>

class TestTree : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();
  void cleanupTestCase();
  void testOpenFile_invalid();
};

void TestTree::initTestCase()
{
  qDebug() << "Initializing TestTree tests...";
}

void TestTree::cleanupTestCase()
{
  qDebug() << "Cleaning up TestTree tests...";
}

void TestTree::testOpenFile_invalid()
{
  QSplitter *splitter = new QSplitter;
  Tree tree(splitter);

  QModelIndex index;

  tree.openFile(index);

  QVERIFY2(FileManager::getInstance().getCurrentFileName().isEmpty(), "FileManager should not process an invalid file.");
}

QTEST_MAIN(TestTree)
#include "test_tree.moc"
