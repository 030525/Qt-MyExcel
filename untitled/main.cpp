#include "mainwindow.h"

#include <QApplication>

#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << QFont();

    MainWindow w;
    w.show();



    return a.exec();
}
