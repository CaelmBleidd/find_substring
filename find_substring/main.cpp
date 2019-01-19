#include "mainwindow.h"
#include <QApplication>
#include "Tests/tests.h"

#include <QTest>

#define QT_NO_DEBUG_OUTPUT

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTest::qExec(new Tests, argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

