#include "mainwindow.h"
#include <QApplication>
#include "utility.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QObject::connect(&a, &QApplication::aboutToQuit, closeLogFile);
    a.setOrganizationName("andreag");
    a.setApplicationName("KillManagerQt");
    // message hanlder
    qInstallMessageHandler(customMessageHandler);
    QString repeatedChar;
    repeatedChar = QString("*").repeated(80 );
    qDebug() << "\n" << repeatedChar << "\n" "Starting KillManagerQt " << __DATE__ << " " << __TIME__ << "\n" << repeatedChar;
    //QString allMessages = g_debugMessages.join("\n");
    //qDebug() << "Collected messages:\n" << allMessages;
    MainWindow w;
    w.show();
    return a.exec();
}
