#include "utility.h"

QStringList g_debugMessages;
FILE *f;
QString sLogFilePath;
char * cLogFilePath;

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    // if (sLogFilePath==nullptr) buildPath();
    // QString sFile = qApp->applicationName();
    // sFile.append (".log");
    QString message = qFormatLogMessage(type, context, msg);
    f = fopen("KillManagerQt.log", "a");
    LOG_VAR(sLogFilePath);
    //f = fopen(cLogFilePath, "a");
    fprintf(f, "%s\n", qPrintable(message));
    fflush(f);
    Q_UNUSED(type)
    Q_UNUSED(context)
    g_debugMessages.append(msg);
    std::cout << msg.toStdString () << std::endl;
}

void closeLogFile()
{
    if (f != NULL)
    {
        LOG_MSG("Closing LOG file...");
        fclose(f);
        f = NULL;
    }
}

// Funzione per convertire una stringa di caratteri wide (WCHAR) in una stringa standard (char)
std::string WcharToString(const WCHAR* wstr)
{
    std::string str;
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    if (size > 0)
    {
        std::vector<char> buffer(size);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer.data(), size, nullptr, nullptr);
        str.assign(buffer.begin(), buffer.end() - 1);
    }
    return str;
}

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
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned))
    {
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
                        qDebug() << "Terminated process: " << QString::fromWCharArray (processName) << " (PID: " << pid << ")";
                        //std::wcout << "Percorso: " << getProcessPath (hProcess).toStdWString () << std::endl;
                        CloseHandle(hProcess);
                        return true;
                    }
                    else
                    {
                        qDebug() << "Failed to terminate process.";
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

QString RunningProcessesListEx::getProcessPath(HANDLE hProcess)
{
    // Definiamo un buffer per memorizzare il percorso.
    // MAX_PATH è una costante definita da Windows per la lunghezza massima del percorso.
    //char szPath[MAX_PATH];
    wchar_t szPath[MAX_PATH];
    // Chiamiamo la funzione GetModuleFileNameEx.
    // Il primo parametro è l'handle del processo.
    // Il secondo parametro è l'handle del modulo (NULL per il modulo principale).
    // Il terzo parametro è il nostro buffer.
    // Il quarto parametro è la dimensione del buffer.
    DWORD dwResult = GetModuleFileNameEx(hProcess, NULL, szPath, MAX_PATH);
    // Controlliamo se la funzione è riuscita.
    // Se dwResult è 0, c'è stato un errore.
    if (hProcess == INVALID_HANDLE_VALUE | hProcess == 0) return "";
    if (dwResult != 0)
    {
        //std::cout << "Percorso del processo: " << szPath << std::endl;
        return QString::fromStdWString (szPath);
    }
    else
    {
        // Se c'è un errore, stampiamo il codice di errore per il debug.
        qDebug() << "Errore nel recupero del percorso: " << QString::number(GetLastError()) << " HANDLE= " << hProcess ;
    }
    return "";
}

HANDLE RunningProcessesListEx::getProcessHandle(const std::string &executablePath)
{
    // conversione in wstring
    std::wstring wideExecutablePath(executablePath.begin(), executablePath.end());
    // ottieni il puntatore wchar_t*
    const wchar_t *widePath = wideExecutablePath.c_str();
    DWORD processIds[1024], bytesReturned;
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned))
    {
        return 0;
    }
    DWORD processCount = bytesReturned / sizeof(DWORD);
    for (DWORD i = 0; i < processCount; ++i)
    {
        DWORD pid = processIds[i];
        if (pid == 0) continue;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (!hProcess) continue;
        HMODULE hMod;
        DWORD cbNeeded;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
        {
            wchar_t processName[MAX_PATH];
            if (GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(wchar_t)))
            {
                if (_wcsicmp(processName, widePath) == 0)
                {
                    return hProcess;
                }
                else
                {
                    // std::cerr << "Failed to terminate process." << std::endl;
                }
            }
        }
        CloseHandle(hProcess);
    }
    //std::wcerr << L"Process not found: " << targetName << std::endl;
    return 0;
}

int RunningProcessesListEx::debugProcessesMemory()
{
    DWORD processIds[1024];
    DWORD bytesReturned;
    // Ottiene i PID di tutti i processi attivi
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned))
    {
        qDebug() << "Errore in EnumProcesses.";
        return 1;
    }
    int numProcesses = bytesReturned / sizeof(DWORD);
    qDebug() << "Elenco dei processi attivi:" ;
    qDebug() << "--------------------------------------------------------" ;
    qDebug() << "PID\t\tNome\t\tMemoria (MB)" ;
    qDebug() << "--------------------------------------------------------" ;
    for (int i = 0; i < numProcesses; i++)
    {
        DWORD pid = processIds[i];
        // Apri un handle per il processo con i diritti necessari
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess != NULL)
        {
            // Ottieni il nome del processo
            HMODULE hMod;
            DWORD cbNeeded;
            WCHAR processName[MAX_PATH] = L"Sconosciuto"; // Valore predefinito
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            {
                GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(WCHAR));
            }
            // Ottieni l'utilizzo della memoria
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
            {
                // Calcola la memoria di lavoro in megabyte
                double memoryUsageMB = pmc.WorkingSetSize / (1024.0 * 1024.0);
                // Stampa il PID, il nome e l'utilizzo della memoria
                qDebug() << pid << "\t\t" << QString::fromWCharArray (processName) << "\t\t" << memoryUsageMB ;
            }
            // Chiudi l'handle del processo
            CloseHandle(hProcess);
        }
    }
    return 0;
}

QIcon RunningProcessesListEx::getProcessIcon( std::string sProcessPath, bool bIsFullPath)
{
    QIcon icon(":/icons/img/imageres_15.ico");
    // const wchar_t *exePath = L"C:\\Program Files\\App\\app.exe";
    // SHFILEINFO shinfo = {};
    // QIcon icon = QtWin::fromHICON(hIcon); if (SHGetFileInfo(exePath, 0, &shinfo, sizeof(shinfo),
    // SHGFI_ICON | SHGFI_SMALLICON))
    // {
    // QPixmap pixmap = HIconToQPixmap(shinfo.hIcon);
    // DestroyIcon(shinfo.hIcon); // libera risorsa
    // return pixmap;
    // }
    //std::string exePath = "C:\\Percorso\\del\\tuo\\file.exe";
    HANDLE hProc = getProcessHandle (sProcessPath);
    if (hProc == INVALID_HANDLE_VALUE | hProc == 0 && bIsFullPath == false)
    {
        LOG_VAR(QString::fromStdString (sProcessPath));
        LOG_VAR(bIsFullPath);
        return icon;
    }
    QString path;
    if (bIsFullPath == false)
        path = getProcessPath (hProc);
    else
        path = QString::fromStdString(sProcessPath);
    HICON hIcon = getIconFromExecutable(path.toStdString ());
    if (hIcon != NULL)
    {
        // Converte l'handle HICON di Windows in un QIcon di Qt
        // Passaggio 1: Converti l'HICON in un QPixmap
        // QPixmap pixmap = QtWin::fromHICON(hIcon);
        // Passaggio 2: Converti il QPixmap in un QImage
        // QImage image = pixmap.toImage();
        icon = QIcon (QtWin::fromHICON (hIcon));
        // Importante: distruggi l'handle dell'icona quando non è più necessario
        DestroyIcon(hIcon);
        return icon;
    }
    QString sLogMessage = QString::fromStdString (sProcessPath);
    sLogMessage.append (" does not have an icon, using default icon.");
    LOG_MSG(sLogMessage);
    return icon;
}

HICON RunningProcessesListEx::getIconFromExecutable(const std::string& executablePath)
{
    if (executablePath.empty())
    {
        return NULL;
    }
    // Converti il percorso in una stringa di caratteri wide (LPCSTR)
    const char *path = executablePath.c_str();
    // ExtractIcon restituisce un handle a una singola icona.
    // Il parametro '0' indica che vogliamo la prima icona disponibile nel file.
    HICON hIcon = ExtractIconA(NULL, path, 0);
    return hIcon;
}
