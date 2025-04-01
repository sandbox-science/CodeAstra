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
    void testDeleteDir();
    void testRenamePath();
    void testNewFile();
};

void TestTree::initTestCase()
{
    qDebug() << "Initializing TestTree tests...";
    splitter = new QSplitter;
    tree     = new Tree(splitter);

    // Create a temporary test file for the Unit Tests
    testFilePath = "test_file.cpp";
    QFile testFile(testFilePath);
    if (!testFile.exists())
    {
        if (!testFile.open(QIODevice::WriteOnly))
        {
            QFAIL("Failed to create test file.");
        }
        testFile.close();
    }

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

void TestTree::testRenamePath()
{
    QString originalFileName = "test_file.cpp";
    QString newFileName      = "renamed_file.cpp";

    QFile originalFile(originalFileName);
    if (!originalFile.exists())
    {
        if (!originalFile.open(QIODevice::WriteOnly))
        {
            QFAIL("Failed to create test file.");
        }
        originalFile.close();
    }

    bool fileRenamed = FileManager::getInstance().renamePath(QFileInfo(originalFileName), newFileName);

    QVERIFY2(fileRenamed, "File should be renamed successfully.");
    QVERIFY2(QFile::exists(newFileName), "Renamed file should exist.");
    QVERIFY2(!QFile::exists(originalFileName), "Original file should no longer exist.");

    QFile::remove(newFileName);
}

void TestTree::testDeleteFile()
{
    QVERIFY2(QFile::exists(testFilePath), "Test file should exist before deletion.");

    QFileSystemModel *model = tree->getModel();
    QVERIFY2(model != nullptr, "getModel() should not return nullptr.");

    QModelIndex index = model->index(testFilePath);
    QVERIFY2(index.isValid(), "QModelIndex should be valid for the test file.");

    QString filePath = model->filePath(index);
    FileManager::getInstance().deleteFile(QFileInfo(filePath));

    QVERIFY2(!QFile::exists(testFilePath), "File should be deleted.");
}

void TestTree::testDeleteDir()
{
    // Temporary directory for deletion test
    QString directory = QDir::temp().absolutePath() + "/testDeleteDir";
    QDir tempDir(directory);
    if (!tempDir.exists())
    {
        if (!tempDir.mkpath("."))
        {
            QFAIL("Failed to create test directory.");
        }
    }

    QVERIFY2(QFile::exists(directory), "Test directory should exist before deletion.");

    QFileSystemModel *model = tree->getModel();
    QVERIFY2(model != nullptr, "getModel() should not return nullptr.");

    QModelIndex index = model->index(directory);
    QVERIFY2(index.isValid(), "QModelIndex should be valid for the test directory.");

    QString dirPath = model->filePath(index);
    FileManager::getInstance().deleteFolder(QFileInfo(dirPath));

    QVERIFY2(!QFile::exists(directory), "Directory should be deleted.");
}

void TestTree::testNewFile()
{
    QString newFilePath = QDir::temp().filePath("new_test_file.cpp");
    bool fileCreated    = FileManager::getInstance().newFile(newFilePath);

    QVERIFY2(fileCreated, "New file should be created.");
    QVERIFY2(QFile::exists(newFilePath), "Newly created file should exist.");
}

QTEST_MAIN(TestTree)
#include "test_tree.moc"
