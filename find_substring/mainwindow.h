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


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QDir actual_directory;
    void set_data(QTreeWidgetItem*, QString const&);
    void show_directory(QString const&);


private slots:
    void select_directory();
    void indexing();
    void go_home();
    void go_back();
    void cancel();


};



#endif // MAINWINDOW_H
