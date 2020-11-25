#include "mainwindow.h"

#include <QApplication>
#include<timemaster.h>


int main(int argc, char *argv[])
{
    qputenv("QT_STYLE_OVERRIDE","Fusion");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
