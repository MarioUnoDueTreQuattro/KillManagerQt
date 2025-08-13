#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>

#include <QObject>
#include <QDateTime>
#include <QProcess>
#include <QTimer>

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
    bool shouldExecute();

    QProcess *m_ZipProcess;
    QDateTime m_LastExecutionTime;
    QTimer *m_ZipTimer;
signals:
};

#endif // SCHEDULER_H
