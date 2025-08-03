#include "runningprocesseslist.h"
#include <QDebug>

RunningProcessesList::RunningProcessesList(QObject *parent) : QObject(parent)
{
    processList.clear();
}

void RunningProcessesList::populateProcessList()
{
    processList.clear(); // Clear any existing items
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        // Handle error, e.g., show a message box
        qDebug() << __FUNCTION__ << " error: hProcessSnap == INVALID_HANDLE_VALUE";
        return;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hProcessSnap, &pe32))
    {
        do
        {
            // Convert the wide-character string (wchar_t*) to QString
            QString processName = QString::fromWCharArray(pe32.szExeFile);
            // Add the process name to the QListWidget
            qDebug()<< processName;
            processList.append (processName);
        }
        while (Process32Next(hProcessSnap, &pe32));
    }
    CloseHandle(hProcessSnap);
}

QStringList RunningProcessesList::getProcessList() const
{
    return processList;
}

bool RunningProcessesList::isRunning(QString sProcessName)
{
   sProcessName=sProcessName.toUpper ();
    if (processList.isEmpty ())
        populateProcessList ();
    QString sCurProcess = "";
    bool bFound = false;
    int i_AppCount = processList.count ();
    for (int iCount = 0; iCount < i_AppCount; iCount++)
    {
        sCurProcess = processList[iCount];
        sCurProcess=sCurProcess.toUpper ();
        if (sCurProcess == sProcessName)
        {
            bFound = true;
            return true;
        }
    }
    return bFound;
}
