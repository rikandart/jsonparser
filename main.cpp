#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Regular.ttf");
    MainWindow w;
    if(MainWindow::close)
        return 1;
    w.show();
    return a.exec();
}
