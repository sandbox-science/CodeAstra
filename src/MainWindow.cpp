#include "MainWindow.h"
#include "Syntax.h"
#include "Tree.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>
#include <QDesktopServices>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("CodeAstra ~ Code Editor");

    editor = new CodeEditor(this);
    syntax = new Syntax(editor->document());

    QFontMetrics metrics(editor->font());
    int spaceWidth = metrics.horizontalAdvance(" ");
    editor->setTabStopDistance(spaceWidth * 4);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(splitter);

    tree = new Tree(splitter, this);

    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->setHandleWidth(5);
    splitter->setSizes(QList<int>() << 20 << 950);
    splitter->addWidget(editor);

    createMenuBar();
    showMaximized();
}

MainWindow::~MainWindow() {}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *fileMenu = menuBar->addMenu("File");
    QMenu *helpMenu = menuBar->addMenu("Help");
    QMenu *appMenu  = menuBar->addMenu("CodeAstra");

    createFileActions(fileMenu);
    createHelpActions(helpMenu);
    createAppActions(appMenu);

    setMenuBar(menuBar);
}

void MainWindow::createFileActions(QMenu *fileMenu)
{
    QAction *newAction = createAction(QIcon::fromTheme("document-new"), tr("&New File..."), QKeySequence::New, tr("Create a new file"), &MainWindow::newFile);
    fileMenu->addAction(newAction);

    QAction *openAction = createAction(QIcon::fromTheme("document-open"), tr("&Open..."), QKeySequence::Open, tr("Open an existing file"), &MainWindow::openFile);
    fileMenu->addAction(openAction);

    QAction *saveAction = createAction(QIcon::fromTheme("document-save"), tr("&Save"), QKeySequence::Save, tr("Save your file"), &MainWindow::saveFile);
    fileMenu->addAction(saveAction);

    QAction *saveAsAction = createAction(QIcon::fromTheme("document-saveAs"), tr("&Save As"), QKeySequence::SaveAs, tr("Save current file as..."), &MainWindow::saveFileAs);
    fileMenu->addAction(saveAsAction);
}

void MainWindow::createHelpActions(QMenu *helpMenu)
{
    QAction *helpDoc = new QAction(tr("Documentation"), this);
    connect(helpDoc, &QAction::triggered, this, []()
            { QDesktopServices::openUrl(QUrl("https://github.com/sandbox-science/CodeAstra/wiki")); });
    helpDoc->setStatusTip(tr("Open Wiki"));
    helpMenu->addAction(helpDoc);
}

void MainWindow::createAppActions(QMenu *appMenu)
{
    QAction *aboutAction = new QAction("About CodeAstra", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    appMenu->addAction(aboutAction);
}

QAction *MainWindow::createAction(const QIcon &icon, const QString &text, const QKeySequence &shortcut, const QString &statusTip, void (MainWindow::*slot)())
{
    QAction *action = new QAction(icon, text, this);

    action->setShortcuts(QList<QKeySequence>{shortcut});
    action->setStatusTip(statusTip);
    connect(action, &QAction::triggered, this, slot);

    return action;
}

void MainWindow::newFile()
{
    // TO-DO: Implement new file function
}

void MainWindow::showAbout()
{
    // Extract the C++ version from the __cplusplus macro
    QString cppVersion;
    if (__cplusplus == 201103L)
    {
        cppVersion = "C++11";
    }
    else if (__cplusplus == 201402L)
    {
        cppVersion = "C++14";
    }
    else if (__cplusplus == 201703L)
    {
        cppVersion = "C++17";
    }
    else if (__cplusplus == 202002L)
    {
        cppVersion = "C++20";
    }
    else
    {
        cppVersion = "C++";
    }

    // Construct the about text
    QString aboutText = QString(
                            "<p style='text-align:center;'>"
                            "<b>%1</b><br>"
                            "Version: %2<br><br>"
                            "Developed by %3.<br>"
                            "Built with %4 and Qt %5.<br><br>"
                            "© 2025 %3. All rights reserved."
                            "</p>")
                            .arg(QApplication::applicationName().toHtmlEscaped(),
                                 QApplication::applicationVersion().toHtmlEscaped(),
                                 QApplication::organizationName().toHtmlEscaped(),
                                 cppVersion,
                                 QString::number(QT_VERSION >> 16) + "." +             // Major version
                                     QString::number((QT_VERSION >> 8) & 0xFF) + "." + // Minor version
                                     QString::number(QT_VERSION & 0xFF));              // Patch version

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

void MainWindow::loadFileInEditor(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    editor->setPlainText(in.readAll());
    file.close();

    currentFileName = filePath;
    setWindowTitle("CodeAstra ~ " + QFileInfo(filePath).fileName());
}
