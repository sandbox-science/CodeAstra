#include "../include/FileManager.h"
#include "../include/CodeEditor.h"
#include "../include/MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>

FileManager::FileManager(CodeEditor *editor, MainWindow *mainWindow)
    : m_editor(editor), m_mainWindow(mainWindow)
{
    qDebug() << "FileManager initialized.";
}

FileManager::~FileManager() {}

void FileManager::initialize(CodeEditor *editor, MainWindow *mainWindow)
{
    m_editor     = editor;
    m_mainWindow = mainWindow;
}

QString FileManager::getCurrentFileName() const
{
    return m_currentFileName;
}

void FileManager::setCurrentFileName(const QString fileName)
{
    m_currentFileName = fileName;
}

void FileManager::newFile()
{
    // Logic to create a new file
}

void FileManager::saveFile()
{
    if (m_currentFileName.isEmpty())
    {
        saveFileAs();
        return;
    }

    qDebug() << "Saving file:" << m_currentFileName;

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
    QString fileExtension = getFileExtension();
    QString filter        = "All Files (*);;C++ Files (*.cpp *.h);;Text Files (*.txt)";
    if (!fileExtension.isEmpty())
    {
        filter = QString("%1 Files (*.%2);;%3").arg(fileExtension.toUpper(), fileExtension, filter);
    }

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save File As", QString(), filter);

    if (!fileName.isEmpty())
    {
        m_currentFileName = fileName;
        saveFile();
    }
}

void FileManager::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", QString(),
                                                    "All Files (*);;C++ Files (*.cpp *.h);;Text Files (*.txt)");
    if (!fileName.isEmpty())
    {
        qDebug() << "Opening file: " << fileName;
        m_currentFileName = fileName;
        loadFileInEditor(fileName);
    }
    else
    {
        qDebug() << "No file selected.";
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

    if (m_mainWindow)
    {
        m_mainWindow->setWindowTitle("CodeAstra ~ " + QFileInfo(filePath).fileName());
    }
    else
    {
        qWarning() << "MainWindow is initialized in FileManager.";
    }
}

QString FileManager::getFileExtension() const
{
    if (m_currentFileName.isEmpty())
    {
        qDebug() << "Error: No File name set!";
        return QString();
    }

    return QFileInfo(m_currentFileName).suffix().toLower();
}
