#pragma once

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <memory>

class CodeEditor;
class Syntax;
class Tree;
class FileManager;

/**
 * @class MainWindow
 * @brief The MainWindow class represents the main UI window of the application.
 * 
 * This class is responsible for initializing and managing the main components
 * of the application, including the file tree view, code editor, and menu bar.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    // Initialize the file tree view and set it as the central widget
    // of the main window, alongside the code editor
    void initTree();

    QAction *createAction(const QIcon &icon, const QString &text,
                          const QKeySequence &shortcut, const QString &statusTip,
                          const std::function<void()> &slot);

private slots:
    void showAbout();

private:
    void createMenuBar();
    void createFileActions(QMenu *fileMenu);
    void createHelpActions(QMenu *helpMenu);
    void createAppActions(QMenu *appMenu);

    std::unique_ptr<CodeEditor> m_editor;
    std::unique_ptr<Syntax> m_syntax;
    std::unique_ptr<Tree> m_tree;

    FileManager *m_fileManager;
};