#pragma once

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <memory>
#include <QFile>

class CodeEditor;
class Syntax;
class Tree;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    void loadFileInEditor(const QString &filePath);
    // Initialize the file tree view and set it as the central widget
    // of the main window, alongside the code editor
    void initTree();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void showAbout();

private:
    void createMenuBar();
    void createFileActions(QMenu *fileMenu);
    void createHelpActions(QMenu *helpMenu);
    void createAppActions(QMenu *appMenu);
    QAction *createAction(const QIcon &icon, const QString &text,
                          const QKeySequence &shortcut, const QString &statusTip,
                          void (MainWindow::*slot)());
    std::unique_ptr<CodeEditor> editor;
    QString currentFileName;
    std::unique_ptr<Syntax> syntax;
    std::unique_ptr<Tree> tree;
};