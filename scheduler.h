#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>

#include <QObject>
#include <QDateTime>
#include <QProcess>
#include <QTimer>
#include "zipfiles.h"

class Scheduler : public QObject
{
    Q_OBJECT
public:
    explicit Scheduler(QObject *parent = nullptr);
    ~Scheduler();
public slots:
    void checkForExecution();
private:
    void executeProgram();
    void zipLogFiles();
    bool shouldExecute();
    QProcess *m_ZipProcess;
    QDateTime m_LastExecutionTime;
    QTimer *m_ZipTimer;
    int m_iInitialDelay;
signals:
    void logCompressionExecuted();
    void logCompressionChecked();
};

#endif // SCHEDULER_H
