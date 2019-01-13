#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    connect(ui->actionIndexing_directory, &QPushButton::clicked, this, &MainWindow::indexing);
    connect(ui->actionGo_home, &QPushButton::clicked, this, &MainWindow::go_home);
    connect(ui->actionGo_back, &QPushButton::clicked, this, &MainWindow::go_back);
    connect(ui->actionCancel, &QPushButton::clicked, this, &MainWindow::cancel);
    connect(ui->actionShow_directory, &QPushButton::clicked, this, &MainWindow::show_directory);
    connect(ui->actionChoose_directory, &QPushButton::clicked, this, &MainWindow::choose_directory);
}

MainWindow::~MainWindow() {

}

void MainWindow::show_directory()
{

}

void MainWindow::indexing()
{

}

void MainWindow::choose_directory()
{

}

void MainWindow::go_home()
{

}

void MainWindow::go_back()
{

}

void MainWindow::cancel()
{

}
