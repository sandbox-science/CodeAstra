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
    QString originalFileName = QDir::temp().filePath("testFile.cpp");
    QString newFileName      = QDir::temp().filePath("renamedTestFile.cpp");

    QFile originalFile(originalFileName);
    if (!originalFile.exists() && !originalFile.open(QIODevice::WriteOnly))
    {
        QFAIL("Failed to create test file.");
    }
    originalFile.close();

    OperationResult fileRenamed = FileManager::getInstance().renamePath(QFileInfo(originalFileName), newFileName);

    QVERIFY2(fileRenamed.success, "File should be renamed successfully.");
    QVERIFY2(QFile::exists(newFileName), "Renamed file should exist.");
    QVERIFY2(!QFile::exists(originalFileName), "Original file should no longer exist.");

    QFile::remove(newFileName);
}

void TestFileManager::testDeleteFile()
{
    QString tempFilePath = QDir::temp().filePath("testDeleteFile.cpp");
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly))
    {
        QFAIL("Failed to create temporary test file for deletion.");
    }
    tempFile.write("// test content");
    tempFile.close();

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
    QString directory = QDir::temp().absolutePath() + "/testDeleteDir";
    QDir tempDir(directory);
    if (!tempDir.exists() && !tempDir.mkpath("."))
    {
        QFAIL("Failed to create test directory.");
    }

    QVERIFY2(QFile::exists(directory), "Test directory should exist before deletion.");

    QFileSystemModel *model = tree->getModel();
    QVERIFY2(model, "Tree model should not be null.");

    QModelIndex index = model->index(directory);
    QVERIFY2(index.isValid(), "Model index should be valid for the test directory.");

    FileManager::getInstance().deletePath(QFileInfo(model->filePath(index)));

    QVERIFY2(!QFile::exists(directory), "Directory should be deleted.");
}

void TestFileManager::testNewFile()
{
    QString folderPath = QDir::temp().absolutePath() + "/testNewDir";
    QDir dir(folderPath);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }
    QVERIFY2(QFile::exists(folderPath), "Temporary directory should exist.");

    OperationResult fileCreated = FileManager::getInstance().newFile(QFileInfo(folderPath), "newFileTest1.c");

    QVERIFY2(fileCreated.success, "New file should be created.");
    QVERIFY2(QFile::exists(folderPath + "/newFileTest1.c"), "Newly created file should exist.");

    QFile::remove(folderPath + "/newFileTest1.c");
    QVERIFY2(!QFile::exists(folderPath + "/newFileTest1.c"), "Newly created file should be deleted.");

    dir.removeRecursively();
    QVERIFY2(!QDir(folderPath).exists(), "Directory should not exist after deletion.");
}

void TestFileManager::testNewFolder()
{
    QString folderPath = QDir::temp().absolutePath() + "/testNewDir";
    QDir dir(folderPath);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }
    QVERIFY2(QFile::exists(folderPath), "Temporary directory should exist.");

    OperationResult folderCreated = FileManager::getInstance().newFolder(QFileInfo(folderPath), "newDirTest");

    QVERIFY2(folderCreated.success, "New folder should be created.");
    QVERIFY2(QFile::exists(folderPath + "/newDirTest"), "Newly created folder should exist.");

    dir.removeRecursively();
    QVERIFY2(!QDir(folderPath).exists(), "Directory should not exist after deletion.");
}

void TestFileManager::testNewFolderFail()
{
    QString folderPath            = QDir::temp().absolutePath() + "../testNewDir";
    OperationResult folderCreated = FileManager::getInstance().newFolder(QFileInfo(folderPath), "");

    QVERIFY2(!folderCreated.success, "Folder creation should fail.");

    QDir(folderPath).removeRecursively();
    QVERIFY2(!QDir(folderPath).exists(), "Directory should not exist after deletion.");
}

void TestFileManager::testDuplicatePath()
{
    QString basePath = QDir::temp().absolutePath() + "/testDuplicateDir";
    QDir().mkpath(basePath);

    OperationResult pathDuplicated = FileManager::getInstance().duplicatePath(QFileInfo(basePath));

    QVERIFY2(pathDuplicated.success, "Path should be duplicated successfully.");

    QDir tempDir(QDir::tempPath());
    QStringList entries = tempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &entry : entries)
    {
        if (entry.startsWith("testDuplicateDir_copy"))
        {
            QDir(tempDir.absoluteFilePath(entry)).removeRecursively();
        }
    }

    QDir(basePath).removeRecursively();
}

QTEST_MAIN(TestFileManager)
#include "test_filemanager.moc"
