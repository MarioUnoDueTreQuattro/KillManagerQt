#include "mainwindow.h"
#include "utility.h"
#include <QApplication>
#include <QDateTime>
#include <QSettings>
#include <QDir>

//QByteArray setupLog(){
// QString sLogFilePath = QCoreApplication::applicationDirPath();
// sLogFilePath = QDir::toNativeSeparators (sLogFilePath);
// sLogFilePath.append ("\\KillManagerQt.log");
//    //QByteArray logPathByteArray = sLogFilePath.toUtf8();
////    cLogFilePath = logPathByteArray.data();
////    LOG_MSG(sLogFilePath);
// return sLogFilePath.toUtf8();
//}
QFile *logFile = nullptr;
QMutex g_mutex;

void myCustomMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&g_mutex);
    // 1. Formattazione del messaggio
    QString formattedMessage = QString("[%1] ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    //formattedMessage += " ";
    // Identifichiamo il tipo di messaggio
    // switch (type)
    // {
    // case QtDebugMsg:
    // formattedMessage += "[DEBUG] ";
    // break;
    // case QtInfoMsg:
    // formattedMessage += "[INFO] ";
    // break;
    // case QtWarningMsg:
    // formattedMessage += "[WARNING] ";
    // break;
    // case QtCriticalMsg:
    // formattedMessage += "[CRITICAL] ";
    // break;
    // case QtFatalMsg:
    // formattedMessage += "[FATAL] ";
    // break;
    // }
    formattedMessage += msg;
    //formattedMessage += QString("%1 (%2:%3)").arg(msg).arg(context.file).arg(context.line);
    //formattedMessage +=QString("%1 (%2:%3)").arg(msg).arg(__FILE__).arg(__LINE__);
    // 2. Scrittura del messaggio sul file di log
    QString logFilePath = "KillManagerQt.log";
    // Se il file di log non è aperto, non fare nulla
    if (!logFile || !logFile->isOpen())
        return;
    QTextStream out(logFile);
    out << formattedMessage << Qt::endl;
    out.flush();
    // QFile logFile(logFilePath);
    // if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    // {
    // QTextStream out(&logFile);
    // out << formattedMessage << Qt::endl;
    // logFile.close();
    // }
    // else
    // {
    // QTextStream(stderr) << "Errore: Impossibile aprire il file di log " << logFilePath << Qt::endl;
    // }
    // 3. Stampa del messaggio a terminale (separando stdout e stderr)
    // Per i messaggi di warning, critical e fatal, usiamo stderr
    if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg)
    {
        QTextStream terminalOut(stderr);
        terminalOut << formattedMessage << Qt::endl;
        terminalOut.flush();
    }
    // Per tutti gli altri (debug, info), usiamo stdout
    else
    {
        QTextStream terminalOut(stdout);
        terminalOut << formattedMessage << Qt::endl;
        terminalOut.flush();
    }
    if (type == QtFatalMsg)
    {
        abort();
    }
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("andreag");
    a.setApplicationName("KillManagerQt");
    QSettings settings;
    bool bLogToFile = settings.value("Dialog/UseLogFile", false).toBool ();
    if (bLogToFile)
    {
        // backup old log files to Log_Backup folder
        QStringList nameFilters;
        nameFilters << "*.log";
        QString sAppPath = QCoreApplication::applicationDirPath();
        sAppPath = QDir::toNativeSeparators (sAppPath);
        QString backupDirName = sAppPath + "\\Log_Backup";
        //qDebug() << "backupDirName" << backupDirName;
        QDir backupDir(backupDirName);
        if (!backupDir.exists())
        {
            backupDir.mkpath("."); // Crea la cartella se non esiste
        }
        QString filePath = sAppPath;
        QDir dir(filePath);
        QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        int iFilesCount = files.count();
        //qDebug() << QString::number(iFilesCount);
        //iFilesCount = files.size();
        //qDebug() << QString::number(iFilesCount);
        QString logFilePath;
        for (int i = 0; i < iFilesCount; ++i)
        {
            logFilePath = files[i].absoluteFilePath();
            //qDebug() << "logFilePath" << logFilePath;
            QFile file(logFilePath);
            QString fileName = QFileInfo(file).fileName();
            QString targetPath = backupDir.filePath(fileName);
            if (!file.rename(targetPath))
            {
                qWarning() << "Failed to move file to:" << targetPath;
                return false;
            }
            else qDebug() << fileName << "successfully moved";
        }
        // QObject::connect(&a, &QApplication::aboutToQuit, closeLogFile);
        // sLogFilePath = QCoreApplication::applicationDirPath();
        // sLogFilePath = QDir::toNativeSeparators (sLogFilePath);
        // sLogFilePath.append ("\\KillManagerQt.log");
        // QByteArray logPathByteArray;
        // logPathByteArray = sLogFilePath.toUtf8();
        // cLogFilePath = logPathByteArray.data();
        //LOG_MSG(sLogFilePath);
        // message hanlder
        // Creazione di un nome file unico basato sul timestamp
        QString logFileName = sAppPath;
        logFileName += "\\";
        logFileName += QDateTime::currentDateTime().toString("'KillManagerQt_'yyyy-MM-dd_hh-mm-ss'.log'");
        qDebug() << "New Log FileName " << logFileName;
        settings.setValue ("LogFileName",logFileName);
        settings.sync ();
        // Inizializza il file di log
        logFile = new QFile(logFileName);
        // Se il file si apre correttamente, installa il gestore
        if (logFile->open(QIODevice::WriteOnly | QIODevice::Append))
        {
            qInstallMessageHandler(myCustomMessageHandler);
        }
        // qInstallMessageHandler(myCustomMessageHandler);
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
    //return a.exec();
    int result = a.exec();
    if (bLogToFile)
    {
        // Rimuovi il gestore prima di uscire
        qInstallMessageHandler(nullptr);
        // Chiudi e dealloca il file
        logFile->close();
        delete logFile;
    }
    return result;
}
