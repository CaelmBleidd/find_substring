#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <memory>
#include <QDirIterator>
#include <QTreeWidgetItem>
#include <QCommonStyle>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDateTime>
#include <QFileSystemModel>
#include <QCryptographicHash>
#include <QIODevice>
#include <QMainWindow>
#include <memory>
#include <QDir>
#include <QWidgetItem>
#include <QMap>
#include <QTreeWidget>
#include <QKeyEvent>
#include <QDirIterator>
#include <QVector>
#include <QThread>

#include "indexer.h"
#include <indexerthread.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QDir actual_directory;
    QVector<Indexer> files;

    void set_data(QTreeWidgetItem*, QString const&);
    void show_directory(QString const&);
    void process_indexing();

    QThread *thread = nullptr;

private slots:
    void select_directory();
    void indexing();
    void go_home();
    void go_back();
    void cancel();




};



#endif // MAINWINDOW_H
