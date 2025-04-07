#pragma once

#include <QObject>
#include <memory>
#include <QSyntaxHighlighter>
#include <QFileInfo>

class CodeEditor;
class MainWindow;

struct OperationResult
{
    bool success;
    std::string message;
};

/**
 * @class FileManager
 * @brief Manages file operations such as creating, saving, and opening files.
 * 
 * The FileManager class is a singleton that handles multiple file-related operations
 * within the application. It interacts with the CodeEditor and MainWindow classes
 * to perform tasks such as creating new files, saving existing files, and opening
 * files from the filesystem. The class ensures that only one instance of FileManager
 * exists and provides a global point of access to it.
 */
class FileManager : public QObject
{
    Q_OBJECT

public:
    static FileManager &getInstance(CodeEditor *editor = nullptr, MainWindow *mainWindow = nullptr)
    {
        static FileManager instance(editor, mainWindow);
        if (editor && mainWindow) {
            instance.initialize(editor, mainWindow);
        }
        return instance;
    }
    FileManager(const FileManager &) = delete;
    FileManager &operator=(const FileManager &) = delete;

    QString getFileExtension() const;
    QString getCurrentFileName() const;

    void setCurrentFileName(const QString fileName);
    void initialize(CodeEditor *editor, MainWindow *mainWindow);

    bool renamePath(const QFileInfo &pathInfo, const QString &newName);
    bool newFile(const QFileInfo &pathInfo, QString newFilePath);
    static OperationResult newFolder(const QFileInfo &pathInfo, QString newFolderPath);
    static OperationResult duplicatePath(const QFileInfo &pathInfo);
    bool deletePath(const QFileInfo &pathInfo);

public slots:
    void newFile();
    void saveFile();
    void saveFileAs();
    void openFile();
    void loadFileInEditor(const QString &filePath);

    QString getDirectoryPath() const;

private:
    FileManager(CodeEditor *editor, MainWindow *mainWindow);
    ~FileManager();

    CodeEditor *m_editor;
    MainWindow *m_mainWindow;
    QSyntaxHighlighter *m_currentHighlighter = nullptr;
    QString m_currentFileName;
};