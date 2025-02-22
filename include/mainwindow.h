#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "codeeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    void showAbout();

private:
    CodeEditor *editor;
    QString currentFileName;
    void createMenuBar();
};
#endif // MAINWINDOW_H
