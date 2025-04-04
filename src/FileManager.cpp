#include "FileManager.h"
#include "CodeEditor.h"
#include "MainWindow.h"
#include "SyntaxManager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <filesystem>
#include <iostream>

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

        delete m_currentHighlighter;

        // Create and assign a new syntax highlighter based on language extension
        m_currentHighlighter = SyntaxManager::createSyntaxHighlighter(getFileExtension(), m_editor->document()).release();
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
        qWarning() << "MainWindow is not initialized in FileManager.";
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

QString FileManager::getDirectoryPath() const
{
    return QFileDialog::getExistingDirectory(
        nullptr, QObject::tr("Open Directory"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

// Check for invalid char or pattern to prevent path traversal attack
bool isValidPath(const std::filesystem::path &path)
{
    std::string pathStr = path.string();
    if (pathStr.find("..") != std::string::npos)
    {
        return false;
    }

    return true;
}

bool FileManager::renamePath(const QFileInfo &pathInfo, const QString &newName)
{
    if (!pathInfo.exists())
    {
        qWarning() << "Path does not exist: " << pathInfo.fileName();
        return false;
    }

    std::filesystem::path oldPath = pathInfo.absoluteFilePath().toStdString();

    // Validate the input path
    if (!isValidPath(oldPath))
    {
        QMessageBox::critical(nullptr, "Error", "Invalid file path.");
        return false;
    }

    std::filesystem::path newPath = oldPath.parent_path() / newName.toStdString();

    if (QFileInfo(newPath).exists())
    {
        QMessageBox::critical(nullptr, "Error", QString("%1 already takken.").arg(QString::fromStdString(newPath.filename())));
        return false;
    }

    try
    {
        std::filesystem::rename(oldPath, newPath);
        return true;
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        QMessageBox::critical(nullptr, "Error", QString(e.what()));
        return false;
    }
}

// TO-DO: use QFile moveToTrash instead
// to avoid permanently deleting files and folders
// by moving them to recycling bin
bool FileManager::deleteFile(const QFileInfo &pathInfo)
{
    std::error_code err;
    std::filesystem::path filePath = pathInfo.absoluteFilePath().toStdString();

    // Validate the input path
    if (!isValidPath(filePath))
    {
        QMessageBox::critical(nullptr, "Error", "Invalid file path.");
        return false;
    }

    try
    {
        std::filesystem::remove(filePath, err);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        qWarning() << "Failed to delete: " << e.what();
        return false;
    }

    return true;
}

bool FileManager::deleteFolder(const QFileInfo &pathInfo)
{
    std::error_code err;
    std::filesystem::path dirPath = pathInfo.absolutePath().toStdString();

    // Validate the input path
    if (!isValidPath(dirPath))
    {
        QMessageBox::critical(nullptr, "Error", "Invalid file path.");
        return false;
    }

    try
    {
        std::filesystem::remove_all(dirPath, err);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        qWarning() << "Failed to delete: " << e.what();
        return false;
    }

    return true;
}

bool FileManager::newFile(QString newFilePath)
{
    qWarning() << "newFile Function not yet implemented";
    return false;
}

bool FileManager::newFolder(QString newFolderPath)
{
    qWarning() << "newFolder Function not yet implemented";
    return false;
}

bool FileManager::duplicatePath(const QFileInfo &pathInfo)
{
    std::error_code err;
    std::filesystem::path filePath = pathInfo.absoluteFilePath().toStdString();

    // Validate the input path
    if (!isValidPath(filePath))
    {
        QMessageBox::critical(nullptr, "Error", "Invalid file path.");
        return false;
    }

    std::string fileName           = filePath.filename();
    size_t extensionPosition       = fileName.find_last_of(".");
    fileName                       = (std::string::npos == extensionPosition) ? fileName : fileName.substr(0, extensionPosition);

    std::filesystem::path dupPath  = filePath.parent_path() / (fileName + "_copy" + filePath.extension().c_str());

    int counter = 1;
    while (QFileInfo(dupPath).exists())
    {
        dupPath = filePath.parent_path() / (fileName + "_copy" + std::to_string(counter) + filePath.extension().c_str());
        counter++;
    }

    try
    {
        std::filesystem::copy(filePath, dupPath);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        QMessageBox::critical(nullptr, "Error", QString("Failed to duplicate: ") + e.what());
        return false;
    }

    return true;
}
