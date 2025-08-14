#ifndef PROCESSITEMSLIST_H
#define PROCESSITEMSLIST_H

#include <QObject>
#include <QList>
#include <QString>
#include "utility.h"

class ProcessItem
{
private:
    QString appName;
    bool m_bAppKillEnabled;
    bool m_bFoundWhenKilling;
    bool m_bIsService;
public:
    //ProcessItem();
    ProcessItem(QString, bool);
    QString getAppName() const;
    void setAppName(const QString &value);
    bool getAppKillEnabled() const;
    void setAppKillEnabled(bool value);
    bool getFoundWhenKilling() const;
    void setFoundWhenKilling(bool value);
    bool getIsService() const;
    void setIsService(bool bIsService);
};

class ProcessItemsList : public QObject
{
    Q_OBJECT
public:
    explicit ProcessItemsList(QObject *parent = 0);
    QStringList getRunningProcesses(); // TODO substitute getRunningProcesses in MainWindow
    void populateProcessList();
    bool killProcessAndChildsByNameEx(const std::string& processName);
    bool killProcessAndChildsByNameEx(QString);
    bool killProcessTree(DWORD parentPid);
    QIcon getProcessIcon(std::string, bool);
    HICON getIconFromExecutable(const std::string& executablePath);
    QString getProcessPath(HANDLE hProcess);
    HANDLE getProcessHandle(const std::string& executablePath);
    bool processIsService(DWORD processId);
    bool enableDebugPrivileges();
    bool isRunning (QString);
    void clear();
    void removeAt (int);
    int size() const;
    int count() const;
    ProcessItem *at(int i);
    void append( ProcessItem);
    int findApplicationItemIndex(QString );
    ProcessItem *findApplicationItem(QString );
    void resetAllApplicationItems();
    bool deleteApplicationItem(QString);
    bool moveApplicationItem(QString, bool );
private:
    QList<ProcessItem> m_ProcessItemsList;
    //QStringList m_ProcessList;
    QList<ProcessItem>  m_ProcessList;
};

#endif // PROCESSITEMSLIST_H
