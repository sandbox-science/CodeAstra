#ifndef TREE_H
#define TREE_H

#include <QSplitter>
#include <QTreeView>
#include <QFileSystemModel>
#include <QObject>

class MainWindow; // Forward declaration

class Tree : public QObject
{
    Q_OBJECT

public:
    Tree(QSplitter *splitter, MainWindow *mainWindow);
    ~Tree();

private:
    void setupModel();
    void setupTree();
    void openFile(const QModelIndex &index);
    QString getDirectoryPath();

    QFileSystemModel *model;
    QTreeView *tree;
    MainWindow *mainWindow;
};

#endif // TREE_H
