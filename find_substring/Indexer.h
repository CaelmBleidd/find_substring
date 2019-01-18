#ifndef INDEXER_H
#define INDEXER_H

#include <QObject>
#include <QVector>
#include <QFileInfo>
#include <QDebug>
#include <QTextStream>
#include <string>
#include <fstream>
#include <QThread>

class Indexer {
public:
    static const qint64 MAX_TRIGRAMS_AMOUNT = 20000;
    static const qint64 MAX_LINE_LENGTH = 2000;
    static const qint64 MAX_FILE_SIZE = 1 << 30;

    Indexer();
    Indexer(QString const &file_path);

    ~Indexer();

    void process();
    bool is_text() const;
    QString get_file_name() const;
    QString get_file_path() const;
    QVector<uint32_t> trigrams;

public slots:


private:
    bool indexing; 
    bool is_ok;
    QString file_path;
    QFileInfo file_info;
    

    bool make_trigrams_set(QSet<uint32_t> &, std::string const&, size_t);
    void print_error(QString const &error);
    bool is_valid_utf8(const uint8_t* to_check, size_t length);

private slots:

signals:


};

#endif // INDEXER_H
