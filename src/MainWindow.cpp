#include "../include/MainWindow.h"
#include "../include/Syntax.h"
#include "../include/Tree.h"
#include "../include/CodeEditor.h"
#include "../include/FileManager.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_editor(std::make_unique<CodeEditor>(this)),
      m_syntax(std::make_unique<Syntax>(m_editor->document())),
      m_tree(nullptr),
      m_fileManager(&FileManager::getInstance())
{
    m_fileManager->initialize(m_editor.get(), this);
    setWindowTitle("CodeAstra ~ Code Editor");

    connect(m_editor.get(), &CodeEditor::statusMessageChanged, this, [this](const QString &message)
    {
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        statusBar()->showMessage("[" + timestamp + "] " + message, 4000);
    });

    // Set tab width to 4 spaces
    QFontMetrics metrics(m_editor->font());
    int spaceWidth = metrics.horizontalAdvance(" ");
    m_editor->setTabStopDistance(spaceWidth * 4);
    m_editor->setLineWrapMode(QPlainTextEdit::NoWrap);

    initTree();
    createMenuBar();
    showMaximized();
}

MainWindow::~MainWindow() {}

void MainWindow::initTree()
{
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(splitter);

    m_tree = std::make_unique<Tree>(splitter, m_fileManager);

    splitter->addWidget(m_editor.get());
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->setHandleWidth(5);
    splitter->setSizes(QList<int>() << 150 << 800);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    splitter->setChildrenCollapsible(false);
    splitter->setOpaqueResize(true);
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *fileMenu = menuBar->addMenu("File");
    fileMenu->setObjectName("File");

    QMenu *helpMenu = menuBar->addMenu("Help");
    helpMenu->setObjectName("Help");

    QMenu *appMenu = menuBar->addMenu("CodeAstra");
    appMenu->setObjectName("CodeAstra");

    createFileActions(fileMenu);
    createHelpActions(helpMenu);
    createAppActions(appMenu);

    setMenuBar(menuBar);
}

void MainWindow::createFileActions(QMenu *fileMenu)
{
    fileMenu->addAction(createAction(QIcon(), tr("&New"), QKeySequence::New, tr("Create a new file"), [this]() { m_fileManager->newFile(); }));
    fileMenu->addAction(createAction(QIcon(), tr("&Open"), QKeySequence::Open, tr("Open an existing file"), [this]() { m_fileManager->openFile(); }));
    fileMenu->addSeparator();
    fileMenu->addAction(createAction(QIcon(), tr("&Save"), QKeySequence::Save, tr("Save the current file"), [this]() { m_fileManager->saveFile(); }));
    fileMenu->addAction(createAction(QIcon(), tr("Save &As"), QKeySequence::SaveAs, tr("Save the file with a new name"), [this]() { m_fileManager->saveFileAs(); }));
}

void MainWindow::createHelpActions(QMenu *helpMenu)
{
    QAction *helpDoc = new QAction(tr("Documentation"), this);
    connect(helpDoc, &QAction::triggered, this, []()
    {
        QDesktopServices::openUrl(QUrl("https://github.com/sandbox-science/CodeAstra/wiki"));
    });

    helpDoc->setStatusTip(tr("Open Wiki"));
    helpMenu->addAction(helpDoc);
}

void MainWindow::createAppActions(QMenu *appMenu)
{
    QAction *aboutAction = new QAction("About CodeAstra", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    appMenu->addAction(aboutAction);
}

QAction *MainWindow::createAction(const QIcon &icon, const QString &text, const QKeySequence &shortcut, const QString &statusTip, const std::function<void()> &slot)
{
    QAction *action = new QAction(icon, text, this);

    action->setShortcuts(QList<QKeySequence>{shortcut});
    action->setStatusTip(statusTip);
    connect(action, &QAction::triggered, this, slot);

    return action;
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

    QMessageBox::about(this, tr("About"), aboutText);
}
