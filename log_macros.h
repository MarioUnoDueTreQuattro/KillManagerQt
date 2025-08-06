#ifndef LOG_MACROS_H
#define LOG_MACROS_H

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QApplication>

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
