#include "../include/MainWindow.h"

#include <QtTest/QtTest>
#include <QMenuBar>
#include <QAction>

class TestMainWindow : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();    // Runs before all tests
  void cleanupTestCase(); // Runs after all tests
  void testWindowTitle();
  void testMenuBar();
  void testNewFileAction();
};

void TestMainWindow::initTestCase()
{
  qDebug() << "Initializing MainWindow tests...";
}

void TestMainWindow::cleanupTestCase()
{
  qDebug() << "Cleaning up MainWindow tests...";
}

void TestMainWindow::testWindowTitle()
{
  MainWindow window;
  QCOMPARE(window.windowTitle(), QString("CodeAstra ~ Code Editor"));
}

void TestMainWindow::testMenuBar()
{
  MainWindow window;
  QMenuBar *menuBar = window.menuBar();
  QVERIFY(menuBar != nullptr);
  QCOMPARE(menuBar->actions().size(), 3); // File, Help, CodeAstra
}

void TestMainWindow::testNewFileAction()
{
  MainWindow window;
  QAction *newAction = window.findChild<QAction *>("New File");
  QVERIFY(newAction != nullptr);
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"
