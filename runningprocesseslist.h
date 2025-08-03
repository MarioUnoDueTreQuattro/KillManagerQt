#ifndef RUNNINGPROCESSESLIST_H
#define RUNNINGPROCESSESLIST_H

#include <QStringList>
#include <QString>
#include <windows.h>
#include <tlhelp32.h>

#include <QObject>

class RunningProcessesList : public QObject
{
    Q_OBJECT
public:
    explicit RunningProcessesList(QObject *parent = 0);
    void populateProcessList();
    QStringList getProcessList() const;
    bool isRunning (QString);
private:
    QStringList processList;
signals:

public slots:
};

#endif // RUNNINGPROCESSESLIST_H
