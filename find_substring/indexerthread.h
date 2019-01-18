#ifndef INDEXERTHREAD_H
#define INDEXERTHREAD_H

#include <QObject>
#include <QDir>
#include <QVector>
#include <QDirIterator>
#include <QThread>
#include <algorithm>

#include "indexer.h"

class IndexerThread: public QObject {
    Q_OBJECT

public:
    IndexerThread(QString const&);
    ~IndexerThread();

public slots:
    void process();

private:
    QString const directory;
    QVector<Indexer> files;

signals:
    void show_files(QVector<Indexer>);
    void finished();
    void show_home();
    void change_status();

    void change_progress_max_value(qint64);
    void increase_progress_bar_status();

};

#endif // INDEXERTHREAD_H
