#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("andreag");
    a.setApplicationName("KillManagerQt");
    MainWindow w;
    w.show();
    return a.exec();
}
