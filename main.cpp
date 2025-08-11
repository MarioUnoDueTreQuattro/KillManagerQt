#include "mainwindow.h"
#include "utility.h"
#include <QApplication>
#include <QDateTime>
#include <QSettings>

//QByteArray setupLog(){
// QString sLogFilePath = QCoreApplication::applicationDirPath();
// sLogFilePath = QDir::toNativeSeparators (sLogFilePath);
// sLogFilePath.append ("\\KillManagerQt.log");
//    //QByteArray logPathByteArray = sLogFilePath.toUtf8();
////    cLogFilePath = logPathByteArray.data();
////    LOG_MSG(sLogFilePath);
// return sLogFilePath.toUtf8();
//}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("andreag");
    a.setApplicationName("KillManagerQt");
    QSettings settings;
    bool bLogToFile = settings.value("Dialog/UseLogFile", false).toBool ();
    if (bLogToFile)
    {
//        QObject::connect(&a, &QApplication::aboutToQuit, closeLogFile);
//        sLogFilePath = QCoreApplication::applicationDirPath();
//        sLogFilePath = QDir::toNativeSeparators (sLogFilePath);
//        sLogFilePath.append ("\\KillManagerQt.log");
//        QByteArray logPathByteArray;
//        logPathByteArray = sLogFilePath.toUtf8();
//        cLogFilePath = logPathByteArray.data();
        //LOG_MSG(sLogFilePath);
        // message hanlder
        qInstallMessageHandler(myCustomMessageHandler);
        QDate today = QDate::currentDate();
        QTime currentTime = QTime::currentTime();
        QString repeatedChar;
        QString sMsg = "Starting KillManagerQt " + today.toString () + " " + currentTime.toString () ;
        repeatedChar = QString("*").repeated(80 );
        qDebug().noquote () << "\n\n" << repeatedChar << "\n" << sMsg << "\n" << repeatedChar << "\n\n";
        //QString allMessages = g_debugMessages.join("\n");
        //qDebug() << "Collected messages:\n" << allMessages;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
