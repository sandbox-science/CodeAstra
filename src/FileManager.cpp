#include "FileManager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include "CodeEditor.h"
#include "MainWindow.h"

FileManager::FileManager(CodeEditor *editor, MainWindow *mainWindow)
    : m_editor(editor),
      m_currentFileName(""),
      m_mainWindow(mainWindow)
{
    if (!m_editor)
    {
        qWarning() << "Editor is NOT initialized in FileManager!";
    }
    else
    {
        qDebug() << "Editor is properly initialized in FileManager.";
    }
}

FileManager::~FileManager() {}

void FileManager::newFile()
{
    // Logic to create a new file
}

void FileManager::saveFile()
{
    qDebug() << "Saving file:" << m_currentFileName;
    if (m_currentFileName.isEmpty())
    {
        saveFileAs();
        return;
    }

    QFile file(m_currentFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    if (m_editor)
    {
        out << m_editor->toPlainText();
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "Editor is not initialized.");
        return;
    }
    file.close();

    emit m_editor->statusMessageChanged("File saved successfully.");
}

void FileManager::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save File As", QString(),
                                                    "C++ Files (*.cpp *.h);;Text Files (*.txt);;All Files (*)");

    if (!fileName.isEmpty())
    {
        m_currentFileName = fileName;
        saveFile();
    }
}

void FileManager::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", QString(),
                                                    "C++ Files (*.cpp *.h);;Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty())
    {
        loadFileInEditor(fileName);
    }
}

void FileManager::loadFileInEditor(const QString &filePath)
{
    qDebug() << "Loading file:" << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    if (m_editor)
    {
        m_editor->setPlainText(in.readAll());
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "Editor is not initialized.");
        return;
    }
    file.close();

    m_currentFileName = filePath;
    m_mainWindow->setWindowTitle("CodeAstra ~ " + QFileInfo(filePath).fileName());
}