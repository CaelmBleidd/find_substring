#include "mainwindow.h"
#include <QApplication>

#define QT_NO_DEBUG_OUTPUT

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
