#include <QtTest/QtTest>
#include <QMenuBar>

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
  QCOMPARE(fileMenu->title(), "File");

  QMenu *helpMenu = menuBar->findChild<QMenu *>("Help");
  QVERIFY2(helpMenu != nullptr, "QMenuBar must contain a 'Help' menu.");
  QCOMPARE(helpMenu->title(), "Help");

  QMenu *appMenu = menuBar->findChild<QMenu *>("CodeAstra");
  QVERIFY2(appMenu != nullptr, "QMenuBar must contain a 'CodeAstra' menu.");
  QCOMPARE(appMenu->title(), "CodeAstra");
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"