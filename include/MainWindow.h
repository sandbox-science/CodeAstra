#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CodeEditor.h"
#include "Syntax.h"
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QDesktopServices>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

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
    CodeEditor *editor;
    QString currentFileName;
    Syntax *syntax;
};

#endif // MAINWINDOW_H
