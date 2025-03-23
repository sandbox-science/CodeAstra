#pragma once

#include <QSplitter>
#include <QObject>
#include <memory>

// Forward declarations
class QTreeView;
class QFileSystemModel;
class QFileIconProvider;
class FileManager;

/**
 * @class Tree
 * @brief A class that represents a tree view for displaying the file system.
 *
 * The Tree class is responsible for creating and managing a tree view that displays
 * the file system.
 */
class Tree : public QObject
{
    Q_OBJECT

public:
    explicit Tree(QSplitter *splitter, FileManager *FileManager);
    ~Tree();

private:
    void showContextMenu(const QPoint &pos);
    void setupModel();
    void setupTree();
    void openFile(const QModelIndex &index);

    QString getDirectoryPath() const;

    std::unique_ptr<QFileIconProvider> m_iconProvider;
    std::unique_ptr<QFileSystemModel> m_model;
    std::unique_ptr<QTreeView> m_tree;

    FileManager * m_FileManager;
};