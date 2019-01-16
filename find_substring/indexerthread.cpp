#include "indexerthread.h"

IndexerThread::IndexerThread(QString const &directory): directory(directory) {}

void IndexerThread::process() {
    files.clear();
    QDirIterator iter(QDir::currentPath(), QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (iter.hasNext()) {
        iter.next();
        files.push_back(Indexer(iter.filePath()));
    }
    qDebug() << QString("Vector size for indexing: %1").arg(files.size());

    qint64 count = 0;
    for (auto file: files) {
        file.process();

        if (file.is_text()) {
            ++count;
            qDebug() << QString("%1 was indexed").arg(file.get_file_name()) << "Total files indexed: " << count;
        } else {
            qDebug() << QString("%1 wasn't indexed").arg(file.get_file_name());
        }
    }
    qDebug() << "Total indexed: " << count;
    emit finished();
}
