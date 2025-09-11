#include "scheduler.h"
#include <QDebug>
#include <QSettings>
#include "utility.h"

Scheduler::Scheduler(QObject *parent)
    : QObject(parent)
{
    m_iInitialDelay = 10000;
    QSettings settings;
    bool bLogIsEnabled = settings.value("Dialog/UseLogFile", false).toBool ();
    if (bLogIsEnabled)
    {
        m_LastExecutionTime = settings.value("LogCompressionLastExecutionTime").toDateTime();
        LOG_MSG("Last backup of log files time:" << m_LastExecutionTime);
        m_ZipProcess = new QProcess(this);
        m_ZipTimer = new QTimer(this);
        connect(m_ZipTimer, &QTimer::timeout, this, &Scheduler::checkForExecution);
        //timer->start(1000 * 60 * 60); // 1 hour in milliseconds
        // We start the timer with an initial delay of 10 seconds, then repeat every hour.
        m_ZipTimer->start(m_iInitialDelay); // 10 seconds in milliseconds for the first shot
    }
}

Scheduler::~Scheduler()
{
    // NON FARLO!
    // delete process;
    // delete timer;
}

void Scheduler::checkForExecution()
{
    // The first time this is called, we want to reset the timer for the
    // periodic check.
    emit logCompressionChecked();
    if (m_ZipTimer->interval() == (m_iInitialDelay))
    {
        m_ZipTimer->stop();
        m_ZipTimer->start(1000 * 60 * 60 * 6); // Set to 6 hours
        //LOG_MSG("Timer to execute KillManagerQtZip.exe: interval changed to 6 hours.");
    }
    if (shouldExecute())
    {
        //executeProgram();
        zipLogFiles();
    }
    else
    {
        LOG_MSG("It's not yet time to backup log files");
    }
}

bool Scheduler::shouldExecute()
{
    // The interval is defined here in seconds.
    const qint64 delayInSeconds = 24 * 60 * 60;
    // If it's the first time running, or if delayInSeconds have passed, we should execute.
    QDateTime now = QDateTime::currentDateTime();
    if (!m_LastExecutionTime.isValid() || m_LastExecutionTime.secsTo(now) >= delayInSeconds)
    {
        return true;
    }
    return false;
}

void Scheduler::executeProgram()
{
    LOG_MSG("Executing KillManagerQtZip.exe...");
    QString programPath = "KillManagerQtZip.exe";
    m_ZipProcess->start(programPath);
    if (m_ZipProcess->waitForStarted())
    {
        LOG_MSG("KillManagerQtZip.exe started successfully.");
        // Save the current time as the last execution time.
        QSettings settings;
        m_LastExecutionTime = QDateTime::currentDateTime();
        settings.setValue("LogCompressionLastExecutionTime", m_LastExecutionTime);
        // Emit the signal
        emit logCompressionExecuted();
    }
    else
    {
        LOG_MSG("Failed to start program. Error:" << m_ZipProcess->errorString());
    }
}

void Scheduler::zipLogFiles()
{
    LOG_MSG("Executing zipLogFiles...");
    ZipFiles *zip = new ZipFiles(); // parent = MainWindow o un QWidget
    connect(zip, SIGNAL(zipLogFinished(bool)), this, SLOT(onLogFinished()));
    zip->startLogZip();
    // ZipFiles zipLogs(this->parent ());
    bool bZipped = true;
    // zipLogs.startLogZip ();
    if (bZipped)
    {
        // LOG_MSG("zipLogFiles finished successfully.");
        // Save the current time as the last execution time.
        QSettings settings;
        m_LastExecutionTime = QDateTime::currentDateTime();
        settings.setValue("LogCompressionLastExecutionTime", m_LastExecutionTime);
        //        // Emit the signal
        // emit logCompressionExecuted();
    }
    else
    {
        // LOG_MSG("Failed to execute zipLogFiles. Error." );
    }
}

void Scheduler::zipBatchFiles()
{
    LOG_MSG("Executing zipBatchFiles...");
    ZipFiles *zip = new ZipFiles(); // parent = MainWindow o un QWidget
    connect(zip, SIGNAL(zipBatchFinished(bool)), this, SLOT(onBatchFinished()));
    zip->startBatchZip ();
    // ZipFiles zipLogs(this->parent ());
    bool bZipped = true;
    // zipLogs.startLogZip ();
    if (bZipped)
    {
        // LOG_MSG("zipBatchFiles finished successfully.");
        // emit batchCompressionExecuted();
    }
    else
    {
        // LOG_MSG("Failed to execute zipBatchFiles. Error." );
    }
}

void Scheduler::onLogFinished()
{
    //        // Emit the signal
    emit logCompressionExecuted();
}

void Scheduler::onBatchFinished()
{
    emit batchCompressionExecuted();
}
