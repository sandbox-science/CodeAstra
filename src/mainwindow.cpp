#include "../include/mainwindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QShortcut>
#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>

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

    QAction *openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);

    QAction *saveAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave), tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save your file"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAction);

    QAction *saveAsAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs), tr("&Save As"), this);
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save current file as..."));
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    fileMenu->addAction(saveAsAction);

    QMenu *appMenu = menuBar->addMenu("Code Astra");
    QAction *aboutAction = new QAction("About Code Astra", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    appMenu->addAction(aboutAction);

    setMenuBar(menuBar);
}

void MainWindow::showAbout()
{
    QString aboutText = QString(
        "<div align='center'>"
        "<b>%1</b><br>"
        "Version: %2<br><br>"
        "Developed by %3.<br>"
        "© 2025 %3. All rights reserved."
        "</div>").arg(QApplication::applicationName(),
                      QApplication::applicationVersion(),
                      QApplication::organizationName());

    QMessageBox::about(this, "About Code Astra", aboutText);
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