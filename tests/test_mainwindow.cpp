#include <QtTest/QtTest>
#include <QMenuBar>
#include <QSplitter>

#include "MainWindow.h"
#include "CodeEditor.h"
#include "FileManager.h"

class TestMainWindow : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();
  void cleanupTestCase();
  void testWindowTitle();
  void testEditorInitialization();
  void testMenuBar();
  void testInitTree();
  void testCreateAction();

private:
  std::unique_ptr<MainWindow> mainWindow;
};

void TestMainWindow::initTestCase()
{
  qDebug() << "Initializing MainWindow tests...";
  mainWindow = std::make_unique<MainWindow>();
  mainWindow->show();
}

void TestMainWindow::cleanupTestCase()
{
  qDebug() << "Cleaning up MainWindow tests...";
  mainWindow.reset();
}

void TestMainWindow::testWindowTitle()
{
  QCOMPARE(mainWindow->windowTitle(), "CodeAstra ~ Code Editor");
}

void TestMainWindow::testEditorInitialization()
{
  QVERIFY2(mainWindow->findChild<CodeEditor *>() != nullptr,
           "MainWindow must contain a CodeEditor.");
}

void TestMainWindow::testMenuBar()
{
  QMenuBar *menuBar = mainWindow->menuBar();
  QVERIFY2(menuBar != nullptr, "MainWindow must have a QMenuBar.");
  QCOMPARE(menuBar->actions().size(), 3); // File, Help, CodeAstra

  QMenu *fileMenu = menuBar->findChild<QMenu *>("File");
  QVERIFY2(fileMenu != nullptr, "QMenuBar must contain a 'File' menu.");
  QCOMPARE_H(fileMenu->title(), "File");

  QMenu *helpMenu = menuBar->findChild<QMenu *>("Help");
  QVERIFY2(helpMenu != nullptr, "QMenuBar must contain a 'Help' menu.");
  QCOMPARE_H(helpMenu->title(), "Help");

  QMenu *appMenu = menuBar->findChild<QMenu *>("CodeAstra");
  QVERIFY2(appMenu != nullptr, "QMenuBar must contain a 'CodeAstra' menu.");
  QCOMPARE_H(appMenu->title(), "CodeAstra");
}

void TestMainWindow::testInitTree()
{
  QSplitter *splitter = dynamic_cast<QSplitter *>(mainWindow->centralWidget());
  QVERIFY2(splitter != nullptr, "Central widget should be a QSplitter.");

  QCOMPARE_H(splitter->handleWidth(), 5);
  QCOMPARE_H(splitter->childrenCollapsible(), false);
  QCOMPARE_H(splitter->opaqueResize(), true);

  QList<int> sizes = splitter->sizes();
  QCOMPARE_H(sizes.size(), 2);
}

void TestMainWindow::testCreateAction()
{
  // Mock parameters for createAction
  QIcon icon;
  QString text = "Test Action";
  QKeySequence shortcut = QKeySequence(Qt::CTRL | Qt::Key_T);
  QString statusTip = "This is a test action";
  bool slotCalled = false;

  auto slot = [&slotCalled]()
  { slotCalled = true; };

  QAction *action = mainWindow->createAction(icon, text, shortcut, statusTip, slot);

  QVERIFY2(action != nullptr, "Action should be successfully created.");
  QCOMPARE_H(action->text(), text);
  QCOMPARE_H(action->shortcuts().first(), shortcut);
  QCOMPARE_H(action->statusTip(), statusTip);

  // Simulate triggering the action
  action->trigger();
  QCOMPARE_H(slotCalled, true);
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"