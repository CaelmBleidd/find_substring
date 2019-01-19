#include <indexer.h>

#include "searcher.h"
#include "tests.h"

Tests::Tests(QObject *parent): QObject(parent) {}

void Tests::too_long_line()
{
    QFile file("long_line.tst");
    file.open(QFile::WriteOnly);
    for (auto i = 0; i < 2500; ++i)
        file.write("lgdlgdlgdlgd");
    file.close();
    Indexer indexer("long_line.tst");
    indexer.process();
    file.remove();
    QVERIFY(!indexer.is_text());
}

void Tests::big_file()
{
    QFile file("long_line.tst");
    file.open(QFile::WriteOnly);
    for (auto i = 0; i < (1 << 30) + 1000; ++i)
        file.write("a");
    file.close();
    Indexer indexer("big_file.tst");
    indexer.process();
    file.remove();
    QVERIFY(!indexer.is_text());
}

void Tests::contains_not_utf8()
{
    QFile file("not_utf8.tst");
    file.open(QFile::WriteOnly);
    file.write("\xa0\xa1\xa0\xa1\xa0\xa1");
    file.close();
    Indexer indexer("not_utf8.tst");
    indexer.process();
    file.remove();
    QVERIFY(!indexer.is_text());
}

void Tests::deleted_file()
{
    QFile file("deleted_file.tst");
    file.remove();
    Indexer indexer("deleted_file.tst");
    indexer.process();
    QVERIFY(!indexer.is_text());
}

void Tests::all_fine()
{
    QFile file("text_file.tst");
    file.open(QFile::WriteOnly);
    file.write("text_file");
    file.close();
    Indexer indexer("text_file.tst");
    indexer.process();
    file.remove();
    QVERIFY(indexer.is_text());
}

void Tests::image()
{
    QFile file("image.jpg");
    qDebug() << QDir::currentPath();
    if (!file.exists())
        QSKIP("This test requires .jpg file");
    Indexer indexer("image.jpeg");
    indexer.process();
    file.remove();
    QVERIFY(!indexer.is_text());
}

void Tests::substrings()
{
    QFile file("substrings.tst");
    file.open(QFile::WriteOnly);
    file.write("Тест русского языка\n");
    file.write("Тест української мови'n");
    file.write("Uchunguzi wa lugha Kiukreni\n"); //суахили
    file.write("უკრაინული ენის გამოცდა"); // грузинский
    file.write("English test");
    file.close();
    Indexer indexer("substrings.tst");
    indexer.process();

    Searcher searcher("усск");
    Searcher searcher2("аїнсько");
    Searcher searcher3("hung");
    Searcher searcher4("რაინუ");
    Searcher searcher5("oooooooo");

    QVERIFY2(indexer.is_text(), "It's a text file");
    QVERIFY2(searcher.process(indexer), "Substring in russian");
    QVERIFY2(searcher2.process(indexer), "Substring in ukranian");
    QVERIFY2(searcher3.process(indexer), "Substring in suakhili");
    QVERIFY2(searcher4.process(indexer), "Substring in georgian");
    QVERIFY2(!searcher5.process(indexer), "Not contains");
}

