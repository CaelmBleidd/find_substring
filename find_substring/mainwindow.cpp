#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    connect(ui->actionIndexing_directory, &QPushButton::clicked, this, &MainWindow::indexing);
    connect(ui->actionGo_home, &QPushButton::clicked, this, &MainWindow::go_home);
    connect(ui->actionGo_back, &QPushButton::clicked, this, &MainWindow::go_back);
    connect(ui->actionCancel, &QPushButton::clicked, this, &MainWindow::cancel);
    connect(ui->actionChoose_directory, &QPushButton::clicked, this, &MainWindow::select_directory);

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);

    show_directory(QDir::homePath());
}

MainWindow::~MainWindow() {}

void MainWindow::select_directory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select directory", QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        MainWindow::show_directory(dir);
    } else {
        QMessageBox warning;
        warning.setText("Empty path");
        warning.setEscapeButton(QMessageBox::Ok);
        warning.exec();
    }
}


void MainWindow::show_directory(QString const &dir) {
    ui->treeWidget->clear();

    QDirIterator iter(dir, QDir::NoDot | QDir::AllEntries);
    QDir::setCurrent(dir);

    setWindowTitle(QDir::currentPath());

    while (iter.hasNext()) {
        iter.next();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        set_data(item, iter.filePath());
    }

    ui->treeWidget->sortItems(0, Qt::SortOrder::AscendingOrder);
}


void MainWindow::set_data(QTreeWidgetItem *item, QString const &path) {
    QFileInfo file(path);

    item->setTextColor(0, Qt::black);
    item->setTextColor(1, Qt::black);

    item->setText(0, file.fileName());
    item->setText(1, file.filePath());
}

void MainWindow::indexing() {

}


void MainWindow::go_home() {
    QDir::setCurrent(QDir::homePath());
    show_directory(QDir::homePath());
}

void MainWindow::go_back() {
    QDir::setCurrent("..");
    show_directory(QDir::currentPath());
}

void MainWindow::cancel() {

}
