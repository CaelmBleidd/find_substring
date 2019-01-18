#include "searcher.h"

Searcher::Searcher(std::string const &pattern) : pattern(pattern) {}

bool Searcher::process(Indexer &file) {
    size_t length = pattern.length();
    uint8_t symbols[length];
    for (size_t i = 0; i < length; ++i) {
        symbols[i] = static_cast<uint8_t>(pattern[i]);
    }

    uint32_t trigram = 0 | (symbols[0] << 8) | symbols[1];
    for (size_t i = 2; i < length; ++i) {
        trigram <<= 8;
        trigram |= symbols[i];
        pattern_trigrams.insert(trigram & 0xFFFFFF);
    }

    bool contains = true;

    for (auto &trigram: pattern_trigrams) {
        if (!std::binary_search(file.trigrams.begin(), file.trigrams.end(), trigram))
            contains = false;
    }

    if (contains) {
        if (find_substring(file.get_file_path()))
            return true;
    }
    return false;
}

bool Searcher::find_substring(QString const &file_path) {

    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << QString("File %1 can't be opened").arg(file_path);
        return false;
    }
    while (!file.atEnd()) {
//            std::string line = in.read(4194304).toStdString(); // 4 * 1024 * 1024


//            auto it = std::search(line.begin(), line.end(), std::boyer_moore_horspool_searcher(pattern.begin(), pattern.end()));

//            if (it != line.end()) {
//                return true;
//            }


//            auto q_line = QString().fromStdString(line);

        QString q_line = file.read(4194304);
        if (q_line.indexOf(QString().fromStdString(pattern)) > 0)
            return true;
    }
    return false;

}
