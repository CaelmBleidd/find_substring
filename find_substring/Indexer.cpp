#include "Indexer.h"

Indexer::Indexer(): is_ok(false), indexing(false) {}

Indexer::Indexer(const QString &file_path): file_path(file_path), indexing(false), is_ok(false) {
    file_info = QFileInfo(file_path);
}

void Indexer::process() {
    indexing = true;
    if (file_info.size() > MAX_FILE_SIZE) {
        print_error(QString("Too large file %1").arg(file_info.fileName()));
        return;
    }
    
    QFile file(file_path);
    
    if (!file.open(QIODevice::ReadOnly)) {
        print_error(QString("File %1 can't be opened").arg(file_info.fileName()));
        return;
    }
    
    QTextStream in(&file);
    QString line; 
    QSet<uint32_t> trigrams_set;
    
    while (!in.atEnd()) {
        line = in.readLine().toUtf8();
        auto line_length = file_info.size();

        if (line_length > MAX_LINE_LENGTH) {
            print_error(QString("Too large line in %1").arg(file_info.fileName()));
            return;
        }

        make_trigrams_set(trigrams_set, line, line_length);

        if (trigrams_set.size() > MAX_TRIGRAMS_AMOUNT) {
            print_error(QString("%1 contains too many trigrams").arg(file_info.fileName()));
            return;
        }

        trigrams.clear();
        std::move(trigrams_set.begin(), trigrams_set.end(), std::back_inserter(trigrams));
        std::sort(trigrams.begin(), trigrams.end());
        is_ok = true;
    }
}

void Indexer::make_trigrams_set(QSet<uint32_t> &trigrams_set, QString const &line, qint64 length) {
    auto std_line = line.toStdString();
    uint8_t symbols[length];
    for (auto i = 0; i < length; ++i) {
        symbols[i] = static_cast<uint8_t>(std_line[i]);
    }
    if (!is_valid_utf8(symbols, length)) {
        print_error(QString("%1 contains an invalid UTF-8 sequences").arg(file_info.fileName()));
        return;
    }
    uint32_t trigram = 0 | (symbols[0] << 8) | symbols[1];
    for (auto i = 2; i < length; ++i) {
        trigram <<= 8;
        trigram |= symbols[i];
        trigrams_set.insert(trigram & 0xFFFFFF);
    }
}

bool Indexer::is_valid_utf8(const uint8_t *to_check, qint64 length) {
    qint64 n = 0;
    for (qint64 i = 0; i < length; ++i) {
        if (to_check[i] < 0x80) {
            n = 0;
        } else if ((to_check[i] & 0xE0) == 0xC0) {
            n = 1;
        } else if ((to_check[i] == 0xED) && ((to_check[i + 1] & 0xA0) == 0xA0)) {
            //invalid U+D800 - U+DFFF
            return false;
        } else if((to_check[i] & 0xF0) == 0xE0) {
            n = 2;
        } else if ((to_check[i] & 0xF8) == 0xF0) {
            n = 3;
        } else {
            return false;
        }
        for (qint64 j = 0; j < n && j < length; j++) {
            if (++i == length || (to_check[i] & 0xC0) != 0x80) {
                return false;
            }
        }
    }
    return true;
}

bool Indexer::is_text() {
    return was_indexed && is_ok;
}



void Indexer::print_error(const QString &error) {
    is_ok = false;
    qDebug() << error << "\n";
}
