#include "Tree.h"
#include "FileManager.h"
#include "CodeEditor.h"

#include <QtTest>
#include <QCoreApplication>
#include <QAction>
#include <QMenu>
#include <QFileSystemModel>
#include <QSplitter>
#include <QDir>
#include <QDebug>

class TestTree : public QObject
{
    Q_OBJECT

private:
    QSplitter *splitter;
    Tree *tree;
    QString testFilePath;
    QString filePath;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testOpenFile_invalid();
    void testDeleteFile();
    void testRenameFile();
    void testNewFile();
};

void TestTree::initTestCase()
{
    qDebug() << "Initializing TestTree tests...";
    splitter = new QSplitter;
    tree     = new Tree(splitter);

    // Create a temporary test file for the Unit Tests
    testFilePath = QDir::temp().filePath("test_file.cpp");
    QFile testFile(testFilePath);
    if (!testFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to create test file at" << testFilePath;
        QFAIL("Failed to create test file.");
    }
    testFile.close();

    QFileSystemModel *model = tree->getModel();
    if (model)
    {
        QModelIndex index = model->index(testFilePath);
        filePath = model->filePath(index);
    }
}

void TestTree::cleanupTestCase()
{
    qDebug() << "Cleaning up TestTree tests...";
    delete tree;
    delete splitter;

    if (QFile::exists(testFilePath))
    {
        QFile::remove(testFilePath);
    }
}

void TestTree::testOpenFile_invalid()
{
    QModelIndex index;
    tree->openFile(index);

    QVERIFY2(FileManager::getInstance().getCurrentFileName().isEmpty(), "FileManager should not process an invalid file.");
}

void TestTree::testDeleteFile()
{
    QVERIFY2(QFile::exists(testFilePath), "Test file should exist before deletion.");

    QFileSystemModel *model = tree->getModel();
    QVERIFY2(model != nullptr, "getModel() should not return nullptr.");

    QModelIndex index = model->index(testFilePath);
    QVERIFY2(index.isValid(), "QModelIndex should be valid for the test file.");

    QString filePath = model->filePath(index);
    tree->deleteFile(QFileInfo(filePath));

    QVERIFY2(!QFile::exists(testFilePath), "File should be deleted.");
}

void TestTree::testRenameFile()
{
    QString newFilePath = QDir::temp().filePath("renamed_file.cpp");
    bool fileRenamed    = tree->renameFile(QFileInfo(testFilePath), newFilePath);

    QVERIFY2(fileRenamed, "File should be renamed successfully.");
    QVERIFY2(QFile::exists(newFilePath), "Renamed file should exist.");
}

void TestTree::testNewFile()
{
    QString newFilePath = QDir::temp().filePath("new_test_file.cpp");
    bool fileCreated    = tree->newFile(newFilePath);

    QVERIFY2(fileCreated, "New file should be created.");
    QVERIFY2(QFile::exists(newFilePath), "Newly created file should exist.");
}

QTEST_MAIN(TestTree)
#include "test_tree.moc"
