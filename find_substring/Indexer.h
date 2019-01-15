#ifndef INDEXER_H
#define INDEXER_H

#include <QObject>
#include <Qvector>
#include <QFileInfo>
#include <QDebug>
#include <QTextStream>

class Indexer {
public:
    static const qint64 MAX_TRIGRAMS_AMOUNT = 20000;
    static const qint64 MAX_LINE_LENGTH = 2000;
    static const qint64 MAX_FILE_SIZE = 1 << 30;

    Indexer();
    Indexer(QString const &file_path);

    void process();
    bool is_text();

public slots:


private:
    bool was_indexed;
    bool indexing; 
    bool is_ok;
    QString file_path;
    QFileInfo file_info;
    QVector<uint32_t> trigrams;
    

    void make_trigrams_set(QSet<uint32_t> &, QString const&, qint64);
    void print_error(QString const &error);
    bool is_valid_utf8(const uint8_t* to_check, qint64 length);

private slots:


signals:


};

#endif // INDEXER_H
