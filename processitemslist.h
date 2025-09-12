#ifndef PROCESSITEMSLIST_H
#define PROCESSITEMSLIST_H

#include <QObject>
#include <QList>
#include <QString>
#include "utility.h"

class ProcessItem
{
private:
    static const std::map<DWORD, QString> s_priorityMap;
    QString appName;
    bool m_bAppKillEnabled;
    bool m_bFoundWhenKilling;
    bool m_bIsService;
    DWORD m_ProcessID;
    DWORD m_ThreadCount;
    DWORD m_ParentProcessID;
    DWORD m_Priority;
    bool m_IsProcessWindowVisible;
    QString m_WindowTitle;
    QString m_ProcessPath;
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
    DWORD getProcessID() const;
    void setProcessID(const DWORD &value);
    DWORD getThreadCount() const;
    void setThreadCount(const DWORD &value);
    DWORD getParentProcessID() const;
    void setParentProcessID(const DWORD &value);
    DWORD getPriority() const;
    void setPriority(const DWORD &value);
    QString getPriorityClassName();
    bool getIsProcessWindowVisible() const;
    void setIsProcessWindowVisible(bool value);
    QString getWindowTitle() const;
    void setWindowTitle(const QString &value);
    QString getProcessPath() const;
    void setProcessPath(const QString &value);
};

class ProcessItemsList : public QObject
{
    Q_OBJECT
public:
    explicit ProcessItemsList(QObject *parent = 0);
    double getFreeRAM() ;
   void setAllProcessesWorkingSetSize();
   bool emptySystemWorkingSets();
   static bool enablePrivilege(LPCTSTR privilegeName);
   QStringList getRunningProcesses();
    std::string getProcessNameByPid(DWORD);
    QString getParentProcessName(DWORD) ;
    void populateProcessList();
    bool killProcessAndChildsByNameEx(const std::string& processName);
    bool killProcessAndChildsByNameEx(QString);
    bool killProcessTree(DWORD parentPid);
    QIcon getProcessIcon(std::string, bool);
    HICON getIconFromExecutable(const std::string& executablePath);
    QString getProcessPath(HANDLE hProcess);
    HANDLE getProcessHandle(const std::string& executablePath);
    bool processIsService(int iPos);
    bool processIsService(QString);
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
    ProcessItem *findProcessItem(QString );
    void resetAllApplicationItems();
    bool deleteApplicationItem(QString);
    bool moveApplicationItem(QString, bool );
    void debugProcessList();
    void debugProcessItemsList();

    // Struttura per passare i dati. Puoi usarla anche qui, se la dichiari globalmente o in un namespace.
    struct WindowInfo
    {
        DWORD processId;
        QString *windowTitle; // Usiamo un puntatore per il titolo.
        bool foundVisibleWindow = false;
    };

    // Struttura per passare i dati alla callback di EnumWindows
    struct WindowInfoTitle
    {
        DWORD processId;
        QString windowTitle;
    };
private:
    std::string WcharToString(const WCHAR* wstr);
    QList<ProcessItem> m_ProcessItemsList;
    //QStringList m_ProcessList;
    QList<ProcessItem> m_ProcessList;
    bool processIsService(DWORD processId);
    bool isProcessWindowVisible(DWORD processId);
    QString getWindowTitle(DWORD processId);
    static BOOL CALLBACK EnumWindowsCallbackVisible(HWND hwnd, LPARAM lParam);
    static BOOL CALLBACK EnumWindowsCallbackTitle(HWND hwnd, LPARAM lParam);
    static bool g_windowIsVisible;
    // Un'unica callback per ottenere sia lo stato di visibilità che il titolo.
    static BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);

    // Funzione helper unificata per EnumWindowsCallback
    bool getWindowInfo(DWORD processId, QString& windowTitle);
    QString getProcessFullPath(DWORD processId);
};

#endif // PROCESSITEMSLIST_H
