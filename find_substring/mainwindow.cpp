#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <searcher.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), watcher_for_search(new QFutureWatcher<void>(this)), already_indexed(false),
                                          break_search(false), indexing_in_process(false) {

    ui->setupUi(this);

    qRegisterMetaType<Indexer>("Indexer");

    connect(ui->actionIndexing_directory, &QPushButton::clicked, this, &MainWindow::indexing);
    connect(ui->actionGo_home, &QPushButton::clicked, this, &MainWindow::go_home);
    connect(ui->actionGo_back, &QPushButton::clicked, this, &MainWindow::go_back);
    connect(ui->actionCancel, &QPushButton::clicked, this, &MainWindow::cancel);
    connect(ui->actionChoose_directory, &QPushButton::clicked, this, &MainWindow::select_directory);

    connect(this, SIGNAL(add_file(Indexer const&)), this, SLOT(show_file(Indexer const&)));

    connect(ui->patternLine, &QLineEdit::textChanged, this, &MainWindow::pattern_line_has_changed);

    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);

    ui->listWidget->setStyleSheet("QListWidget { color: black }");
    ui->listWidget->hide();

    ui->patternLine->setDisabled(true);

    ui->patternLine->setStyleSheet("QLineEdit { color: black }");


    ui->treeWidget->setStyleSheet("QTreeWidget { color: black }");

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);

    show_directory(QDir::homePath());
}

MainWindow::~MainWindow() {}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return && !ui->treeWidget->isHidden() && !(ui->treeWidget->selectedItems().size() == 0)) {
        if (indexing_in_process && QMessageBox::question(this, "Attention", "Abort indexing?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
            return;
        }

        already_indexed = false;

        auto path = ui->treeWidget->currentItem()->text(1);
        auto dir = QFileInfo(path);
        if (dir.isDir())
            show_directory(dir.filePath());
        return;
    }
    if (event->key() == Qt::Key_Backspace && !ui->treeWidget->isHidden()) {
        go_back();
        return;
    }
    if (event->key() == Qt::Key_Escape) {
        cancel();
        return;
    }
}

void MainWindow::pattern_line_has_changed() {
    if (indexing_in_process) {
        return;
    }

    ui->listWidget->clear();

    pattern = ui->patternLine->text().toStdString();

    if(pattern.empty()) {
        for (auto &file: text_files) {
            ui->listWidget->addItem(file.get_file_name());
        }
    }

    if (pattern.size() < 3) return;

    if (watcher_for_search->isRunning()) {
        stop_searching();
    }

    break_search = false;

    watcher_for_search->setFuture(QtConcurrent::run(this, &MainWindow::search_pattern));

}


void MainWindow::show_file(Indexer const &file) {
    ui->listWidget->addItem(QString("Name: %1; Path: %2").arg(file.get_file_name()).arg(file.get_file_path()));
}

void MainWindow::search_pattern() {
    Searcher searcher(pattern);

    qRegisterMetaType<QString>("QString");

    for (auto &file: text_files) {
        if (break_search) break;
        if (searcher.process(file)) {
            emit add_file(file);
            qDebug() << file.get_file_name();
        }
    }
}

void MainWindow::select_directory() {
    already_indexed = false;
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


void MainWindow::show_current_directory() {
    show_directory(QDir::currentPath());
}

void MainWindow::change_indexing_status() {
    indexing_in_process = false;
}

void MainWindow::show_directory(QString const &dir) {
    ui->listWidget->clear();
    ui->listWidget->hide();
    ui->treeWidget->clear();
    ui->treeWidget->show();
    ui->patternLine->clear();

    if (indexing_in_process)
        cancel();

    ui->progressBar->setValue(0);

    if (!already_indexed) {
        ui->patternLine->setDisabled(true);
        ui->actionIndexing_directory->setEnabled(true);
    }

    ui->actionCancel->setDisabled(true);

    QDir::setCurrent(dir);

    QDirIterator iter(dir, QDir::NoDot | QDir::AllEntries);

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

    item->setText(0, file.fileName());
    item->setText(1, file.filePath());
}

void MainWindow::change_max_value_progress_bar(qint64 value) {
    ui->progressBar->setMaximum(value);
}

void MainWindow::increase_progress_bar_value() {
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

void MainWindow::indexing() {
    ui->actionCancel->setEnabled(true);
    ui->actionIndexing_directory->setDisabled(true);



    IndexerThread *indexer_thread = new IndexerThread(QDir::currentPath());
    thread_for_indexing = new QThread();
    indexer_thread->moveToThread(thread_for_indexing);

    connect(thread_for_indexing, SIGNAL(started()), indexer_thread, SLOT(process()));

    qRegisterMetaType<QVector<Indexer>>("QVector<Indexer>");
    connect(indexer_thread, SIGNAL(show_files(QVector<Indexer>)), this, SLOT(after_indexing(QVector<Indexer>)));

    ui->actionCancel->setEnabled(true);

    qRegisterMetaType<qint64>("qint64");
    connect(indexer_thread, SIGNAL(change_progress_max_value(qint64)), this, SLOT(change_max_value_progress_bar(qint64)));
    connect(indexer_thread, SIGNAL(increase_progress_bar_status()), this, SLOT(increase_progress_bar_value()));

//            void change_progress_max_value(qint64);
//            void increase_progress_bar_status();

    connect(indexer_thread, SIGNAL(change_status()), this, SLOT(change_indexing_status()));
    connect(indexer_thread, SIGNAL(show_home()), this, SLOT(show_current_directory()));

    connect(indexer_thread, SIGNAL(finished()), thread_for_indexing, SLOT(quit()));
    connect(thread_for_indexing, SIGNAL(finished()), thread_for_indexing, SLOT(deleteLater()));
    connect(indexer_thread, SIGNAL(finished()), indexer_thread, SLOT(deleteLater()));

    indexing_in_process = true;
    thread_for_indexing->start();
}



void MainWindow::after_indexing(QVector<Indexer> files) {
    text_files = std::move(files);

    ui->treeWidget->clear();
    ui->treeWidget->hide();
    ui->listWidget->show();

    indexing_in_process = false;
    already_indexed = true;

    ui->patternLine->setEnabled(true);

    ui->actionCancel->setDisabled(true);
    ui->actionIndexing_directory->setDisabled(true);

    for (auto &file: text_files) {
        ui->listWidget->addItem(file.get_file_name());
    }

}

void MainWindow::go_home() {
    if (indexing_in_process && QMessageBox::question(this, "Attention", "Abort indexing?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }
    already_indexed = false;
    cancel();
    QDir::setCurrent(QDir::homePath());
    show_directory(QDir::homePath());
}


void MainWindow::go_back() {
    if (indexing_in_process && QMessageBox::question(this, "Attention", "Abort indexing?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }
    already_indexed = false;
    cancel();
    QDir::setCurrent("..");
    show_directory(QDir::currentPath());
}

void MainWindow::cancel() {
    if (indexing_in_process && thread_for_indexing != nullptr) {
        thread_for_indexing->requestInterruption();
    }
}


void MainWindow::stop_searching() {
    if (watcher_for_search->isRunning()) {
        break_search = true;
        watcher_for_search->waitForFinished();
    }
}
