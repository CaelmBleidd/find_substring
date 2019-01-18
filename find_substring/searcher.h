#ifndef SEARCHER_H
#define SEARCHER_H

#define QT_NO_DEBUG_OUTPUT


#include <QObject>
#include <QSet>
#include <string>
#include <algorithm>
#include <functional>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>


#include "indexer.h"

class Searcher {
public:
    Searcher(std::string const &);

    QSet<uint32_t> pattern_trigrams;

    bool process(Indexer &file);

private:
    std::string pattern;

    void contains_same_trigrams();
    bool find_substring(QString const &file_path);

signals:
    void add_file(QString path);
};

#endif // SEARCHER_H
