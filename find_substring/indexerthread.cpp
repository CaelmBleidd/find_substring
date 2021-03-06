#include "indexerthread.h"

IndexerThread::IndexerThread(QString const &directory) : directory(directory) {}

IndexerThread::~IndexerThread() {
    files.clear();
}

void IndexerThread::process() {
    files.clear();
    QDirIterator iter(QDir::currentPath(), QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                      QDirIterator::Subdirectories);
    QTime one_timer;
    one_timer.start();
    while (iter.hasNext()) {

        if (QThread::currentThread()->isInterruptionRequested()) {
            qDebug() << "Interruption requested";
            emit change_status();
            emit show_home();
            emit finished();
            return;
        }

        iter.next();
        files.push_back(Indexer(iter.filePath()));
    }


    qDebug() << one_timer.elapsed() / 1000.0;
    qDebug() << QString("Vector size for indexing: %1").arg(files.size());
    emit change_progress_max_value(files.size());

    qint64 count = 0;
    for (auto &file: files) {
        file.process();

        if (QThread::currentThread()->isInterruptionRequested()) {
            qDebug() << "Interruption requested";
            emit change_status();
            emit show_home();
            break;
        }

        if (file.is_text()) {
            ++count;
            qDebug() << QString("%1 was indexed").arg(file.get_file_name()) << "Total files indexed: " << count;
        } else {
            qDebug() << QString("%1 wasn't indexed").arg(file.get_file_name());
        }

        emit increase_progress_bar_status();
    }
    qDebug() << one_timer.elapsed() / 1000.0;
    qDebug() << "Total indexed: " << count;
    files.erase(std::remove_if(files.begin(), files.end(), [](const Indexer &index) { return !index.is_text(); }),
                files.end());

    qDebug() << one_timer.elapsed() / 1000.0;
    if (!QThread::currentThread()->isInterruptionRequested()) {
        emit show_files(files);
    }
    emit finished();
}
