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
    void testNewFolder();
    void testDuplicatePath();
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
    QString originalFileName = QDir::temp().filePath("testFile.cpp");
    QString newFileName      = QDir::temp().filePath("renamedTestFile.cpp");

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
    QVERIFY2(model != nullptr, "Tree model should not be null.");

    QModelIndex index = model->index(tempFilePath);
    QVERIFY2(index.isValid(), "Model index should be valid for the temporary file.");

    QString filePath = model->filePath(index);

    FileManager::getInstance().deleteFile(QFileInfo(filePath));

    QVERIFY2(!QFile::exists(tempFilePath), "Temporary file should be deleted.");
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

    // Cleanup
    QFile::remove(newFilePath);
}

void TestTree::testNewFolder()
{
    QString newFolderPath = QDir::temp().absolutePath() + "/testNewDir";
    bool folderCreated    = FileManager::getInstance().newFolder(newFolderPath);

    QVERIFY2(folderCreated, "New folder should be created.");
    QVERIFY2(QFile::exists(newFolderPath), "Newly created folder should exist.");

    // Cleanup
    QDir dir(newFolderPath);
    dir.removeRecursively();
}

void TestTree::testDuplicatePath()
{
    QString basePath = QDir::temp().absolutePath() + "/testDuplicateDir";
    QDir().mkpath(basePath);

    bool pathDuplicated = FileManager::getInstance().duplicatePath(QFileInfo(basePath));

    QVERIFY2(pathDuplicated, "Path should be duplicated successfully.");

    // Find the duplicated path created for this test and clean it up
    QDir tempDir(QDir::tempPath());
    QStringList entries = tempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &entry : entries)
    {
        if (entry.startsWith("testDuplicateDir_copy"))
        {
            QDir(tempDir.absoluteFilePath(entry)).removeRecursively();
        }
    }

    // Clean up the original path
    QDir(basePath).removeRecursively();
}

QTEST_MAIN(TestTree)
#include "test_tree.moc"
