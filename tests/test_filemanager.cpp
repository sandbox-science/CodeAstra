#include "Tree.h"
#include "FileManager.h"

#include <QtTest>
#include <QCoreApplication>
#include <QAction>
#include <QMenu>
#include <QFileSystemModel>
#include <QSplitter>
#include <QDir>
#include <QDebug>

class TestFileManager : public QObject
{
    Q_OBJECT

private:
    QSplitter *splitter = nullptr;
    Tree *tree          = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testOpenFile_invalid();
    void testDeleteFile();
    void testDeleteDir();
    void testRenamePath();
    void testNewFile();
    void testNewFolder();
    void testNewFolderFail();
    void testDuplicatePath();
};

void TestFileManager::initTestCase()
{
    qDebug() << "Initializing TestFileManager tests...";
    splitter = new QSplitter;
    tree     = new Tree(splitter);
}

void TestFileManager::cleanupTestCase()
{
    qDebug() << "Cleaning up TestFileManager tests...";
    delete tree;
    delete splitter;
}

void TestFileManager::testOpenFile_invalid()
{
    QModelIndex index;
    tree->openFile(index);

    QVERIFY2(FileManager::getInstance().getCurrentFileName().isEmpty(),
             "FileManager should not process an invalid file.");
}

void TestFileManager::testRenamePath()
{
    QTemporaryDir tempDir;
    QVERIFY2(tempDir.isValid(), "Temporary directory should be valid.");

    QString originalFilePath = tempDir.path() + "/testFile.cpp";
    QFile file(originalFilePath);
    QVERIFY2(file.open(QIODevice::WriteOnly), "File should be created successfully.");
    file.write("// test content");
    file.close();

    QString newFilePath = tempDir.path() + "/renamedTestFile.cpp";
    OperationResult fileRenamed = FileManager::getInstance().renamePath(QFileInfo(originalFilePath), newFilePath);

    QVERIFY2(fileRenamed.success, fileRenamed.message.c_str());
    QVERIFY2(QFile::exists(newFilePath), "Renamed file should exist.");
    QVERIFY2(!QFile::exists(originalFilePath), "Original file should no longer exist.");
}

void TestFileManager::testDeleteFile()
{
    QTemporaryDir tempDir;
    QVERIFY2(tempDir.isValid(), "Temporary directory should be valid.");

    QString tempFilePath = tempDir.path() + "/testDeleteFile.cpp";
    QFile file(tempFilePath);
    QVERIFY2(file.open(QIODevice::WriteOnly), "Temporary file should be created.");
    file.close();

    QVERIFY2(QFile::exists(tempFilePath), "Temporary file should exist before deletion.");

    QFileSystemModel *model = tree->getModel();
    QVERIFY2(model, "Tree model should not be null.");

    QModelIndex index = model->index(tempFilePath);
    QVERIFY2(index.isValid(), "Model index should be valid for the temporary file.");

    FileManager::getInstance().deletePath(QFileInfo(model->filePath(index)));

    QVERIFY2(!QFile::exists(tempFilePath), "Temporary file should be deleted.");
}

void TestFileManager::testDeleteDir()
{
    QTemporaryDir tempDir;
    QVERIFY2(tempDir.isValid(), "Temporary directory should be valid.");

    QString dirPath = tempDir.path() + "/testDeleteDir";
    QDir().mkdir(dirPath);

    QVERIFY2(QFileInfo(dirPath).exists(), "Test directory should exist before deletion.");

    QFileSystemModel *model = tree->getModel();
    QVERIFY2(model, "Tree model should not be null.");

    QModelIndex index = model->index(dirPath);
    QVERIFY2(index.isValid(), "Model index should be valid for the test directory.");

    FileManager::getInstance().deletePath(QFileInfo(model->filePath(index)));

    QVERIFY2(!QFile::exists(dirPath), "Directory should be deleted.");
}

void TestFileManager::testNewFile()
{
    QTemporaryDir tempDir;
    QVERIFY2(tempDir.isValid(), "Temporary directory should be valid.");

    QString folderPath = tempDir.path();
    OperationResult fileCreated = FileManager::getInstance().newFile(QFileInfo(folderPath), "newFileTest1.c");

    QVERIFY2(fileCreated.success, "New file should be created.");
    QVERIFY2(QFile::exists(folderPath + "/newFileTest1.c"), "Newly created file should exist.");
}

void TestFileManager::testNewFolder()
{
    QTemporaryDir tempDir;
    QVERIFY2(tempDir.isValid(), "Temporary directory should be valid.");

    QString folderPath = tempDir.path();
    OperationResult folderCreated = FileManager::getInstance().newFolder(QFileInfo(folderPath), "newDirTest");

    QVERIFY2(folderCreated.success, "New folder should be created.");
    QVERIFY2(QFile::exists(folderPath + "/newDirTest"), "Newly created folder should exist.");
}

void TestFileManager::testNewFolderFail()
{
    QTemporaryDir tempDir;
    QVERIFY2(tempDir.isValid(), "Temporary directory should be valid.");

    QString folderPath = tempDir.path();
    OperationResult folderCreated = FileManager::getInstance().newFolder(QFileInfo(folderPath), "");

    QVERIFY2(!folderCreated.success, "Folder creation should fail.");
}

void TestFileManager::testDuplicatePath()
{
    QTemporaryDir tempDir;
    QVERIFY2(tempDir.isValid(), "Temporary directory should be valid.");

    QString basePath = tempDir.path() + "/testDuplicateDir";
    QDir().mkdir(basePath);

    OperationResult pathDuplicated = FileManager::getInstance().duplicatePath(QFileInfo(basePath));

    QVERIFY2(pathDuplicated.success, "Path should be duplicated successfully.");
}

QTEST_MAIN(TestFileManager)
#include "test_filemanager.moc"
