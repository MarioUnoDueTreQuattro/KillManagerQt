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

public slots:
    void checkForExecution();

private:
    void executeProgram();
    bool shouldExecute();

    QProcess *process;
    QDateTime lastExecutionTime;
    QTimer *timer;
signals:
};

#endif // SCHEDULER_H
