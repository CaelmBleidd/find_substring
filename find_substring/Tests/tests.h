#ifndef TESTS_H
#define TESTS_H

//#define QT_NO_DEBUG_OUTPUT

#include <QObject>
#include <QFile>
#include <QTest>
#include <QDebug>

class Tests: public QObject {
    Q_OBJECT
public:
    explicit Tests(QObject *parent = nullptr);

private slots:
    //indexing
    void too_long_line();
    void big_file();
    void contains_not_utf8();
    void deleted_file();
    void all_fine();
    void image();


    //search
    void substrings();


};

#endif // TESTS_H
