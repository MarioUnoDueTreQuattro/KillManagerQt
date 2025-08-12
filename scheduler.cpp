#include "scheduler.h"
#include <QDebug>
#include <QSettings>

Scheduler::Scheduler(QObject *parent)
    : QObject(parent),
      process(new QProcess(this)),
      timer(new QTimer(this))
{
    // Load the last execution time from a persistent storage.
    // We use QSettings, which is great for this.
    QSettings settings;
    lastExecutionTime = settings.value("LogCompressionLastExecutionTime").toDateTime();

    qDebug() << "Last execution time loaded:" << lastExecutionTime;

    // We connect the timer's timeout signal to our checkForExecution slot.
    // The timer is set to check every hour.
    // Feel free to adjust this interval based on your needs.
    connect(timer, &QTimer::timeout, this, &Scheduler::checkForExecution);
    timer->start(1000 * 60 * 60); // 1 hour in milliseconds

    // We start the timer with an initial delay of 10 seconds, then repeat every hour.
       timer->start(1000 * 10); // 10 seconds in milliseconds for the first shot
       }

void Scheduler::checkForExecution()
{
    // The first time this is called, we want to reset the timer for the
       // periodic check.
       if (timer->interval() != (1000 * 60 * 60)) {
           timer->stop();
           timer->start(1000 * 60 * 60); // Set to 1 hour
           qDebug() << "Timer interval changed to 1 hour.";
       }
       if (shouldExecute()) {
        executeProgram();
    } else {
        qDebug() << "It's not yet time to execute the program.";
    }
}

bool Scheduler::shouldExecute()
{
    // The interval is 2 days in seconds.
    const qint64 twoDaysInSeconds = 2 * 24 * 60 * 60;

    // If it's the first time running, or if 2 days have passed, we should execute.
    QDateTime now = QDateTime::currentDateTime();
    if (!lastExecutionTime.isValid() || lastExecutionTime.secsTo(now) >= twoDaysInSeconds) {
        return true;
    }

    return false;
}

void Scheduler::executeProgram()
{
    qDebug() << "Executing the program...";

    // This is the path to your .exe file. Make sure to adjust it.
    QString programPath = "KillManagerQtZip.exe";

    process->start(programPath);

    if (process->waitForStarted()) {
        qDebug() << "Program started successfully.";
        // Save the current time as the last execution time.
        QSettings settings;
        lastExecutionTime = QDateTime::currentDateTime();
        settings.setValue("LogCompressionLastExecutionTime", lastExecutionTime);
    } else {
        qDebug() << "Failed to start program. Error:" << process->errorString();
    }
}


