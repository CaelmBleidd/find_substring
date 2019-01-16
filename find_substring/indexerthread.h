#ifndef INDEXERTHREAD_H
#define INDEXERTHREAD_H

#include <QObject>
#include <QDir>
#include <QVector>
#include <QDirIterator>
#include <QThread>

#include "indexer.h"

class IndexerThread: public QObject {
    Q_OBJECT

public:
    IndexerThread(QString const&);

public slots:
    void process();

private:
    QString const directory;
    QVector<Indexer> files;

signals:
    void finished();
};

#endif // INDEXERTHREAD_H
