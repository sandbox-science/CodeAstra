#pragma once

#include <QSplitter>
#include <QObject>
#include <memory>

// Forward declarations
class MainWindow;
class QTreeView;
class QFileSystemModel;
class QFileIconProvider;

class Tree : public QObject
{
    Q_OBJECT

public:
    explicit Tree(QSplitter *splitter, MainWindow *mainWindow);
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

    MainWindow *m_mainWindow;
};