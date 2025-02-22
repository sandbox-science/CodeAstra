#include "../include/mainwindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QShortcut>
#include <QMessageBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("CodeAstra ~ Code Editor");
    resize(800, 600);

    editor = new CodeEditor(this);
    setCentralWidget(editor);

    createMenuBar();

    QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(saveShortcut, &QShortcut::activated, this, &MainWindow::saveFile);
}

MainWindow::~MainWindow() {}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu   = menuBar->addMenu("File");

    QAction *openAction = new QAction("Open", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);

    QAction *saveAction = new QAction("Save", this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAction);

    QAction *saveAsAction = new QAction("Save As", this);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    fileMenu->addAction(saveAsAction);

    setMenuBar(menuBar);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
            return;
        }

        QTextStream in(&file);
        if (editor)
        {
            editor->setPlainText(in.readAll());
        }
        else
        {
            QMessageBox::critical(this, "Error", "Editor is not initialized.");
        }
        file.close();

        currentFileName = fileName;

        setWindowTitle("CodeAstra ~ " + QFileInfo(fileName).fileName());
    }
}

void MainWindow::saveFile()
{
    if (currentFileName.isEmpty())
    {
        saveFileAs();
        return;
    }

    QFile file(currentFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    if (editor)
    {
        out << editor->toPlainText();
    }
    file.close();

    statusBar()->showMessage("File saved successfully.", 2000);
}

void MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File As");
    if (!fileName.isEmpty())
    {
        currentFileName = fileName;
        saveFile();
    }
}