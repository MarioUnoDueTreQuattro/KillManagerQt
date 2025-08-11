#ifndef DEBUGMESSAGEHANDLER_H
#define DEBUGMESSAGEHANDLER_H

#include <QStringList>
#include <QtGlobal>
#include <QDebug>

//extern QStringList g_debugMessages;
//extern FILE *f;
//extern QString sLogFilePath;
//extern char * cLogFilePath;

//void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
//void closeLogFile();
void myCustomMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // DEBUGMESSAGEHANDLER_H

#ifndef LOG_MACROS_H
#define LOG_MACROS_H

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QApplication>
#include <QStringList>
#include <QString>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <QObject>
#include <shellapi.h>
#include <string>
#include <vector>
#include <QPixmap>
#include <QIcon>
//#include <QtWinExtras/QtWin>
#include <QtWinExtras/QtWinExtras>
#include <qt_windows.h>

// Basic log with file, line, and function
#define LOG() \
    qDebug() << "File:" << __FILE__ \
             << "Line:" << __LINE__ \
             << "Function:" << __FUNCTION__

// Log with custom message
#define LOG_MSG(msg) \
    qDebug() << "File:" << __FILE__ \
             << "Line:" << __LINE__ \
             << "Function:" << __FUNCTION__ \
             << "Msg:" \
             << msg

// IF Condition Log with custom message
#define LOG_MSG_IF(cond, msg) \
             if (cond) qDebug() \
             << "File:" << __FILE__ \
             << "Line:" << __LINE__ \
             << "Function:" << __FUNCTION__ \
             << "Msg:" \
             << msg

// IF Condition==false Log with custom message
#define LOG_MSG_IF_FALSE(cond, msg) \
             if (cond==false) qDebug() \
             << "File:" << __FILE__ \
             << "Line:" << __LINE__ \
             << "Function:" << __FUNCTION__ \
             << "Msg:" \
             << msg

// Log a variable name and value
#define LOG_VAR(var) \
    qDebug() << "File:" << __FILE__ \
             << "Line:" << __LINE__ \
             << "Function:" << __FUNCTION__ \
             << "Var:" << #var << "=" << var

// Log to file
#define LOG_FILE(msg) \
    writeToLogFile( msg, __FILE__, __FUNCTION__, __LINE__)

// Internal helper for "Log to file"
inline void writeToLogFile(const QString& message, const char* fileName, const char* function, int line)
{
    QString sFile = qApp->applicationName();
    sFile.append (".log");
    QFile file(sFile);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " | File: " << fileName << " | Function: " << function << " | Line: " << line << " | " << message << "\n";
        file.close();
    }
}

#endif // LOG_MACROS_H

#ifndef MYUTILITY_H
#define MYUTILITY_H

// Include Windows API headers
#ifdef Q_OS_WIN
    #include <windows.h>
    #include <psapi.h> // For EnumProcesses, EnumProcessModules, GetModuleBaseName
#endif

class MyUtility
{
private:
    QStringList m_ProcessesList;
public:

    bool KillRunningProcesses();
    QStringList getRunningProcesses();

};

#endif // MYUTILITY_H

#ifndef RUNNINGPROCESSESLISTEX_H
#define RUNNINGPROCESSESLISTEX_H

class RunningProcessesListEx : public QObject
{
    Q_OBJECT
public:
    explicit RunningProcessesListEx(QObject *parent = 0);
    void populateProcessList();
    QStringList getProcessList() const;
    bool isRunning (QString);
    bool killProcessByName(const std::wstring& targetName);
    bool killProcessByName(QString);
    QString getProcessPath(HANDLE hProcess);
    HANDLE getProcessHandle(const std::string& executablePath);
    int debugProcessesMemory();
    QIcon getProcessIcon(std::string,bool);
    HICON getIconFromExecutable(const std::string& executablePath);

private:
    QStringList processList;
signals:

public slots:
};

#endif // RUNNINGPROCESSESLISTEX_H
