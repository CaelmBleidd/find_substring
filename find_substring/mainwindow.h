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
#include <QtConcurrent/QtConcurrent>

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

    std::atomic_bool break_search;
    std::atomic_bool indexing_in_process;
    std::atomic_bool already_indexed;

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void after_indexing(QVector<Indexer>);
    void show_file(Indexer const &);
    void go_home();
    void show_current_directory();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QDir actual_directory;
    QVector<Indexer> text_files;

    std::string pattern;

    void set_data(QTreeWidgetItem*, QString const&);
    void show_directory(QString const&);

    QThread *thread_for_indexing = nullptr;

    QFutureWatcher<void>* watcher_for_search;

    void search_pattern();

private slots:
    void select_directory();
    void indexing();
    void go_back();
    void cancel();
    void pattern_line_has_changed();

    void stop_searching();


signals:
    void add_file(Indexer const&);

};



#endif // MAINWINDOW_H
