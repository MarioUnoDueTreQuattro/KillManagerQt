#include "utility.h"

QStringList MyUtility::getRunningProcesses()
{
    QStringList processList;
#ifdef Q_OS_WIN
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    // Get the list of process identifiers.
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        qWarning() << "EnumProcesses failed!";
        return processList;
    }
    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    // Iterate through each process to get its name.
    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] == 0)
            continue;
        // Open process with necessary access rights
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
        if (NULL != hProcess)
        {
            HMODULE hMod;
            DWORD cbNeededModule;
            char szProcessName[MAX_PATH] = { 0 };
            // Get a list of all modules in the process.
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule))
            {
                // Get the base name of the first module (usually the executable).
                GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
            }
            // If we got a name, add it to our list
            if (szProcessName[0] != '\0')
            {
                processList << QString::fromLocal8Bit(szProcessName);
            }
            // Release the handle to the process.
            CloseHandle(hProcess);
        }
    }
#else
    // For non-Windows platforms, you would use different methods.
    // For example, on Linux, you might read from /proc/<pid>/comm or use 'ps' command.
    qDebug() << "This function is designed for Windows. Other OS not implemented.";
    processList << "Not available on this OS.";
#endif
    // Sort the list for better readability
    processList.sort();
    return processList;
}

bool MyUtility::KillRunningProcesses()
{
    QStringList processList;
#ifdef Q_OS_WIN
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    // Get the list of process identifiers.
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        qWarning() << "EnumProcesses failed!";
        return false;
    }
    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    // Iterate through each process to get its name.
    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] == 0)
            continue;
        // Open process with necessary access rights
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
        if (NULL != hProcess)
        {
            HMODULE hMod;
            DWORD cbNeededModule;
            char szProcessName[MAX_PATH] = { 0 };
            // Get a list of all modules in the process.
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule))
            {
                // Get the base name of the first module (usually the executable).
                GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
            }
            // If we got a name, add it to our list
            if (szProcessName[0] != '\0')
            {
                processList << QString::fromLocal8Bit(szProcessName);
            }
            // Release the handle to the process.
            CloseHandle(hProcess);
        }
    }
#else
    // For non-Windows platforms, you would use different methods.
    // For example, on Linux, you might read from /proc/<pid>/comm or use 'ps' command.
    qDebug() << "This function is designed for Windows. Other OS not implemented.";
    processList << "Not available on this OS.";
#endif
    // Sort the list for better readability
    processList.sort();
    return true;
}

RunningProcessesListEx::RunningProcessesListEx(QObject *parent) : QObject(parent)
{
    processList.clear();
}

void RunningProcessesListEx::populateProcessList()
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
            //qDebug() << __FUNCTION__ << processName;
            processList.append (processName);
        }
        while (Process32Next(hProcessSnap, &pe32));
    }
    CloseHandle(hProcessSnap);
}

QStringList RunningProcessesListEx::getProcessList() const
{
    return processList;
}

bool RunningProcessesListEx::isRunning(QString sProcessName)
{
    sProcessName = sProcessName.toUpper ();
    if (processList.isEmpty ())
        populateProcessList ();
    QString sCurProcess = "";
    bool bFound = false;
    int i_AppCount = processList.count ();
    for (int iCount = 0; iCount < i_AppCount; iCount++)
    {
        sCurProcess = processList[iCount];
        sCurProcess = sCurProcess.toUpper ();
        if (sCurProcess == sProcessName)
        {
            bFound = true;
            return true;
        }
    }
    return bFound;
}

bool RunningProcessesListEx::killProcessByName(const std::wstring& targetName)
{
    DWORD processIds[1024], bytesReturned;

      if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) {
          return false;
      }

      DWORD processCount = bytesReturned / sizeof(DWORD);
      for (DWORD i = 0; i < processCount; ++i)
    {
        DWORD pid = processIds[i];
        if (pid == 0) continue;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, pid);
        if (!hProcess) continue;
        HMODULE hMod;
        DWORD cbNeeded;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
        {
            wchar_t processName[MAX_PATH];
            if (GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(wchar_t)))
            {
                if (_wcsicmp(processName, targetName.c_str()) == 0)
                {
                    // Found the target process
                    if (TerminateProcess(hProcess, 1))
                    {
                        std::wcout << L"Terminated process: " << processName << L" (PID: " << pid << L")" << std::endl;
                        CloseHandle(hProcess);
                        return true;
                    }
                    else
                    {
                        std::cerr << "Failed to terminate process." << std::endl;
                    }
                }
            }
        }
        CloseHandle(hProcess);
    }
    //std::wcerr << L"Process not found: " << targetName << std::endl;
    return false;
}

bool RunningProcessesListEx::killProcessByName(QString sTargetName)
{
    std::wstring wideName = sTargetName.toStdWString();
    return killProcessByName (wideName );

}
