#pragma once

#include "FileManager.h"

#include <QSplitter>
#include <QObject>
#include <memory>
#include <QFileInfo>

// Forward declarations
class QTreeView;
class QFileSystemModel;
class QFileIconProvider;

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
    explicit Tree(QSplitter *splitter);
    ~Tree();

    void initialize(const QString &directory);
    void setupModel(const QString &directory);
    void setupTree();
    void openFile(const QModelIndex &index);

    QFileSystemModel* getModel() const;

private:
    void showContextMenu(const QPoint &pos);
    QFileInfo getPathInfo();
    void isSuccessful(OperationResult result);

    std::unique_ptr<QFileIconProvider> m_iconProvider;
    std::unique_ptr<QFileSystemModel> m_model;
    std::unique_ptr<QTreeView> m_tree;
};