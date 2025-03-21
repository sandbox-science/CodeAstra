#pragma once

#include <QObject>

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
  FileManager(CodeEditor *editor, MainWindow *mainWindow);
  ~FileManager();

public slots:
  void newFile();
  void saveFile();
  void saveFileAs();
  void openFile();
  void loadFileInEditor(const QString &filePath);

private:
  std::unique_ptr<CodeEditor> m_editor;
  QString m_currentFileName;
  MainWindow *m_mainWindow;
};
