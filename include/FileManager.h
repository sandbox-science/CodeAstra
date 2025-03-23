#pragma once

#include <QObject>
#include <memory>

class CodeEditor;
class MainWindow;

/**
 * @class FileManager
 * @brief Manages file operations such as creating, saving, and opening files.
 */
class FileManager : public QObject
{
    Q_OBJECT

public:
    static FileManager &getInstance(CodeEditor *editor = nullptr, MainWindow *mainWindow = nullptr)
    {
        static FileManager instance(editor, mainWindow);
        return instance;
    }

    FileManager(const FileManager &) = delete;
    FileManager &operator=(const FileManager &) = delete;

    QString getFileExtension() const;
    QString getCurrentFileName() const;

    void setCurrentFileName(const QString fileName);
    void initialize(CodeEditor *editor, MainWindow *mainWindow);

public slots:
    void newFile();
    void saveFile();
    void saveFileAs();
    void openFile();
    void loadFileInEditor(const QString &filePath);

private:
    FileManager(CodeEditor *editor, MainWindow *mainWindow);
    ~FileManager();

    CodeEditor *m_editor;
    MainWindow *m_mainWindow;
    QString m_currentFileName;
};
