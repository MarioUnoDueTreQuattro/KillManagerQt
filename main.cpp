#include "mainwindow.h"
#include <QApplication>
#include "Utility.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("andreag");
    a.setApplicationName("KillManagerQt");
    // message hanlder
    //qInstallMessageHandler(customMessageHandler);
    //qDebug() << "Hello from main";
    //QString allMessages = g_debugMessages.join("\n");
    //qDebug() << "Collected messages:\n" << allMessages;
    MainWindow w;
    w.show();
    return a.exec();
}
