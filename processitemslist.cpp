#include "processitemslist.h"

#include <QDebug>
#include <QMessageBox>

// Function to convert WCHAR* to std::string
std::string ProcessItemsList::wideCharToString(const WCHAR* wideString)
{
    if (wideString == nullptr)
    {
        return "";
    }
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, NULL, 0, NULL, NULL);
    if (bufferSize <= 0)
    {
        return "";
    }
    std::string convertedString(bufferSize - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideString, -1, &convertedString[0], bufferSize, NULL, NULL);
    return convertedString;
}

// Funzione per convertire una stringa di caratteri wide (WCHAR) in una stringa standard (char)
std::string ProcessItemsList::WcharToString(const WCHAR* wstr)
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

ProcessItemsList::ProcessItemsList(QObject *parent) : QObject(parent)
{
    m_ProcessList.clear();
}

std::string ProcessItemsList::getProcessNameByPid(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == NULL)
    {
        // La funzione non è riuscita a ottenere un handle del processo
        return "";
    }
    char processName[MAX_PATH];
    if (GetModuleBaseNameA(hProcess, NULL, processName, sizeof(processName)) == 0)
    {
        // La funzione non è riuscita a ottenere il nome del modulo
        CloseHandle(hProcess);
        return "";
    }
    CloseHandle(hProcess);
    return std::string(processName);
}

QString ProcessItemsList::getParentProcessName(DWORD iPID)
{
    QString sParentName;
    sParentName = QString::fromStdString (getProcessNameByPid(iPID));
    if (sParentName == "") sParentName = "N/A (parent process might not be running)";
    return sParentName;
}

//QString ProcessItemsList::getParentProcessName(DWORD iPID)
//{
// QString sParentName;
// int iParentPID;
// int iCount = m_ProcessList.count ();
// for (int i = 0; i < iCount; i++)
// {
// iParentPID = m_ProcessList[i].getProcessID ();
// if (iParentPID == iPID)
// {
// sParentName = m_ProcessList[i].getAppName();
//            //qDebug() << i << " " << m_ProcessList.at(i).getAppName ();
// return sParentName;
// }
// }
// return sParentName;
//}

QString ProcessItemsList::getProcessFullPath(DWORD processId)
{
    // Tentiamo di aprire il processo con i permessi necessari
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess == nullptr)
    {
        // Potrebbe fallire a causa di permessi insufficienti
        return QString("N/A (Could not open process)");
    }
    TCHAR processPath[MAX_PATH];
    DWORD pathSize = sizeof(processPath) / sizeof(TCHAR);
    // Usiamo GetModuleFileNameEx per ottenere il percorso completo dell'eseguibile principale
    if (GetModuleFileNameEx(hProcess, nullptr, processPath, pathSize))
    {
        // Se la funzione ha successo, convertiamo il percorso in una QString
        CloseHandle(hProcess);
        return QString::fromWCharArray(processPath);
    }
    else
    {
        // Se la funzione fallisce, chiudiamo l'handle e restituiamo un errore
        CloseHandle(hProcess);
        return QString("N/A (Could not get process path)");
    }
}

//// Struttura per passare i dati alla callback di EnumWindows
//struct WindowInfo
//{
// DWORD processId;
// QString windowTitle;
//};

bool ProcessItemsList::g_windowIsVisible = false;

// La callback viene eseguita per ogni finestra
BOOL CALLBACK ProcessItemsList::EnumWindowsCallbackTitle(HWND hwnd, LPARAM lParam)
{
    ProcessItemsList::WindowInfoTitle* info = reinterpret_cast<ProcessItemsList::WindowInfoTitle *>(lParam);
    DWORD windowProcessId;
    GetWindowThreadProcessId(hwnd, &windowProcessId);
    // Controlla se i PID corrispondono e se la finestra è visibile.
    if (info->processId == windowProcessId && IsWindowVisible(hwnd))
    {
        // Se i PID corrispondono, otteniamo il titolo.
        TCHAR windowTitle[256];
        GetWindowText(hwnd, windowTitle, sizeof(windowTitle) / sizeof(TCHAR));
        info->windowTitle = QString::fromWCharArray(windowTitle);
        return FALSE; // Fermiamo l'enumerazione.
    }
    return TRUE; // Continua l'enumerazione.
}

// Variabile per memorizzare il risultato della ricerca.
//static bool g_windowIsVisible = false;

// Callback che viene eseguita per ogni finestra di primo livello.
BOOL CALLBACK ProcessItemsList::EnumWindowsCallbackVisible(HWND hwnd, LPARAM lParam)
{
    DWORD currentProcessId = *reinterpret_cast<DWORD *>(lParam);
    DWORD windowProcessId;
    // Ottiene il PID del processo che possiede la finestra.
    GetWindowThreadProcessId(hwnd, &windowProcessId);
    // Controlla se i PID corrispondono e se la finestra è visibile.
    if (currentProcessId == windowProcessId && IsWindowVisible(hwnd))
    {
        g_windowIsVisible = true;
        return FALSE; // Ferma l'enumerazione non appena la finestra viene trovata.
    }
    return TRUE; // Continua l'enumerazione.
}

// Funzione helper che restituisce il titolo della finestra
QString ProcessItemsList::getWindowTitle(DWORD processId)
{
    ProcessItemsList::WindowInfoTitle info;
    info.processId = processId;
    info.windowTitle = ""; // Inizializziamo il titolo a una stringa vuota.
    // Enumera le finestre e popola la nostra struttura
    EnumWindows(EnumWindowsCallbackTitle, reinterpret_cast<LPARAM>(&info));
    return info.windowTitle;
}

// Funzione helper per verificare se un processo ha una finestra visibile.
bool ProcessItemsList::isProcessWindowVisible(DWORD processId)
{
    g_windowIsVisible = false;
    EnumWindows(EnumWindowsCallbackVisible, reinterpret_cast<LPARAM>(&processId));
    return g_windowIsVisible;
}

//// Struttura per passare i dati. Puoi usarla anche qui, se la dichiari globalmente o in un namespace.
//struct WindowInfo3 {
// DWORD processId;
// QString* windowTitle; // Usiamo un puntatore per il titolo.
// bool foundVisibleWindow = false;
//};

// Callback unificata per trovare la finestra.
BOOL CALLBACK ProcessItemsList::EnumWindowsCallback(HWND hwnd, LPARAM lParam)
{
    WindowInfo* info = reinterpret_cast<WindowInfo *>(lParam);
    DWORD windowProcessId;
    GetWindowThreadProcessId(hwnd, &windowProcessId);
    if (info->processId == windowProcessId && IsWindowVisible(hwnd))
    {
        info->foundVisibleWindow = true;
        // Otteniamo il titolo solo se abbiamo trovato la finestra giusta.
        TCHAR windowTitle[256];
        GetWindowText(hwnd, windowTitle, sizeof(windowTitle) / sizeof(TCHAR));
        *info->windowTitle = QString::fromWCharArray(windowTitle);
        return FALSE; // Fermiamo l'enumerazione.
    }
    return TRUE; // Continuiamo la ricerca.
}

// Funzione unificata per ottenere le informazioni sulla finestra.
bool ProcessItemsList::getWindowInfo(DWORD processId, QString& windowTitle)
{
    WindowInfo info;
    info.processId = processId;
    info.windowTitle = &windowTitle; // Passiamo un puntatore alla stringa di output.
    EnumWindows(ProcessItemsList::EnumWindowsCallback, reinterpret_cast<LPARAM>(&info));
    return info.foundVisibleWindow; // Restituisce true se ha trovato una finestra visibile.
}

void ProcessItemsList::populateProcessList()
{
    m_ProcessList.clear(); // Clear any existing items
    m_ProcessItemsList.clear ();
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        // Handle error, e.g., show a message box
        LOG_MSG( "error: hProcessSnap == INVALID_HANDLE_VALUE");
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
            // qDebug() << __FUNCTION__ << processName;
            ProcessItem newItem(processName, true);
            DWORD processId = pe32.th32ProcessID;
            // EmptyWorkingSet(HANDLE(processId));
            //SetProcessWorkingSetSizeEx(HANDLE(processId), (SIZE_T) -1, (SIZE_T) -1, QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE);
            bool bIsService = processIsService(processId);
            newItem.setIsService (bIsService);
            newItem.setProcessID (processId);
            newItem.setThreadCount (pe32.cntThreads);
            newItem.setParentProcessID (pe32.th32ParentProcessID);
            newItem.setPriority (pe32.pcPriClassBase);
            QString sProcessPath = getProcessFullPath (processId);
            newItem.setProcessPath (sProcessPath);
            QString windowTitleRef = "";
            bool hasVisibleWindow = getWindowInfo(processId, windowTitleRef);
            //if (isProcessWindowVisible(processId))
            if (hasVisibleWindow)
            {
                // Il processo ha una finestra visibile.
                newItem.setIsProcessWindowVisible (true);
                //qDebug() << newItem.getAppName () << " VISIBLE";
                // Otteniamo il titolo della finestra
                //QString windowTitle = getWindowTitle(processId);
                // Verifichiamo se abbiamo trovato un titolo
                //QString windowStatus;
                if (!windowTitleRef.isEmpty())
                {
                    //windowStatus = "Visible Window Title: " + windowTitle;
                    newItem.setWindowTitle (windowTitleRef);
                    //qDebug() << windowTitleRef;
                }
                else
                {
                    //windowStatus = "No visible window found.";
                    newItem.setWindowTitle ("");
                }
            }
            else
            {
                // Il processo non ha una finestra visibile.
                // Potrebbe essere un processo in background o un servizio.
                newItem.setIsProcessWindowVisible (false);
            }
            //newItem.setParentProcessName (getParentProcessName (pe32.th32ParentProcessID));
            m_ProcessList.append (newItem);
        }
        while (Process32Next(hProcessSnap, &pe32));
    }
    CloseHandle(hProcessSnap);
}

void ProcessItemsList::setAllProcessesWorkingSetSize()
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        // Handle error, e.g., show a message box
        LOG_MSG( "error: hProcessSnap == INVALID_HANDLE_VALUE");
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
            // qDebug() << __FUNCTION__ << processName;
            DWORD processId = pe32.th32ProcessID;
            if (processId == 0 );//|| processId == 4) continue; // Skip Idle and System
            HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_QUERY_INFORMATION, FALSE, processId);
            if (hProcess != nullptr)
            {
                // WINBOOL bSuccess = SetProcessWorkingSetSize(hProcess, (SIZE_T) -1, (SIZE_T) -1);
                WINBOOL bSuccess = EmptyWorkingSet(hProcess);
                if (bSuccess)
                {
                    SetProcessWorkingSetSizeEx(hProcess, (SIZE_T) -1, (SIZE_T) -1, QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE);
                    //LOG_MSG(processName + " PID " + QString::number (processId) + " SetProcessWorkingSetSizeEx");
                }
                else
                {
                    std::cout << processName.toStdString () << " " << "FAILED to trim working set. Error: " << GetLastError() << "\n";
                }
                CloseHandle(hProcess);
            }
        }
        while (Process32Next(hProcessSnap, &pe32));
        emptySystemWorkingSets();
    }
    CloseHandle(hProcessSnap);
}

//typedef enum _SYSTEM_MEMORY_LIST_COMMAND
//{
// MemoryFlushModifiedList = 0,
// MemoryPurgeStandbyList = 1,
// MemoryPurgeLowPriorityStandbyList = 2,
// MemoryEmptyWorkingSets = 4,
// MemoryFlushModifiedListByColor = 5
//} SYSTEM_MEMORY_LIST_COMMAND;

typedef enum _SYSTEM_MEMORY_LIST_COMMAND
{
    MemoryCaptureAccessedBits = 0,
    MemoryCaptureAndResetAccessedBits = 1,
    MemoryEmptyWorkingSets = 2,
    MemoryFlushModifiedList = 3,
    MemoryPurgeStandbyList = 4,
    MemoryPurgeLowPriorityStandbyList = 5,
    MemoryCommandMax                      // Not a command, just a count
} SYSTEM_MEMORY_LIST_COMMAND;

typedef NTSTATUS (NTAPI *PFN_NtSetSystemInformation)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength
);

#define SystemMemoryListInformation 0x50  // undocumented

bool ProcessItemsList::enablePrivilege(LPCTSTR privilegeName)
{
    HANDLE token = NULL;
    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
        &token))
    {
        qDebug() << "OpenProcessToken failed:" << GetLastError();
        return false;
    }
    TOKEN_PRIVILEGES tp;
    LUID luid;
    if (!LookupPrivilegeValue(NULL, privilegeName, &luid))
    {
        qDebug() << "LookupPrivilegeValue failed:" << GetLastError();
        CloseHandle(token);
        return false;
    }
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), NULL, NULL))
    {
        qDebug() << "AdjustTokenPrivileges call failed:" << GetLastError();
        CloseHandle(token);
        return false;
    }
    DWORD err = GetLastError();
    CloseHandle(token);
    if (err == ERROR_NOT_ALL_ASSIGNED)
    {
        qDebug() << "Privilege" << privilegeName << "not held by this process token!";
        return false;
    }
    //qDebug() << "Privilege" << privilegeName << "enabled successfully.";
    return true;
}

bool ProcessItemsList::emptySystemWorkingSets()
{
    bool result = false;
    // Must enable BOTH privileges
    bool ok1 = enablePrivilege(SE_INCREASE_QUOTA_NAME);
    bool ok2 = enablePrivilege(SE_PROF_SINGLE_PROCESS_NAME);
    if (!ok1 || !ok2)
    {
        qDebug() << "Failed to enable required privileges. Run as Administrator?";
        emit emptySystemWorkingSetsFinished(false);
        return false;
    }
    HMODULE ntdll = GetModuleHandle(TEXT("ntdll.dll"));
    if (!ntdll)
    {
        qDebug() << "Cannot load ntdll.dll";
        emit emptySystemWorkingSetsFinished(false);
        return false;
    }
    PFN_NtSetSystemInformation pNtSetSystemInformation =
        (PFN_NtSetSystemInformation)GetProcAddress(ntdll, "NtSetSystemInformation");
    if (!pNtSetSystemInformation)
    {
        qDebug() << "Cannot resolve NtSetSystemInformation";
        emit emptySystemWorkingSetsFinished(false);
        return false;
    }
    SYSTEM_MEMORY_LIST_COMMAND command;
    NTSTATUS status;
    command = MemoryFlushModifiedList;
    status = pNtSetSystemInformation(
            SystemMemoryListInformation,
            &command,
            sizeof(command)
        );
    if (status != 0) qDebug() << "NtSetSystemInformation (MemoryFlushModifiedList) failed. NTSTATUS:" << hex << status;
    command = MemoryEmptyWorkingSets;
    status = pNtSetSystemInformation(
            SystemMemoryListInformation,
            &command,
            sizeof(command)
        );
    if (status != 0) qDebug() << "NtSetSystemInformation (MemoryEmptyWorkingSets) failed. NTSTATUS:" << hex << status;
    command = MemoryPurgeStandbyList;
    status = pNtSetSystemInformation(
            SystemMemoryListInformation,
            &command,
            sizeof(command)
        );
    if (status != 0) qDebug() << "NtSetSystemInformation (MemoryPurgeStandbyList) failed. NTSTATUS:" << hex << status;
    command = MemoryPurgeLowPriorityStandbyList;
    status = pNtSetSystemInformation(
            SystemMemoryListInformation,
            &command,
            sizeof(command)
        );
    if (status != 0) qDebug() << "NtSetSystemInformation (MemoryPurgeLowPriorityStandbyList) failed. NTSTATUS:" << hex << status;
    command = MemoryCaptureAndResetAccessedBits;
    status = pNtSetSystemInformation(
            SystemMemoryListInformation,
            &command,
            sizeof(command)
        );
    if (status != 0) qDebug() << "NtSetSystemInformation (MemoryCaptureAndResetAccessedBits) failed. NTSTATUS:" << hex << status;
    // Open the "System" process (PID 4 on most systems)
    HANDLE hSystem = OpenProcess(PROCESS_SET_QUOTA | PROCESS_QUERY_INFORMATION,
            FALSE, 4);
    if (hSystem)
    {
        // Trim PID 4 working set (system WS)
        SetProcessWorkingSetSize(hSystem, (SIZE_T) -1, (SIZE_T) -1);
        CloseHandle(hSystem);
    }
    if (status != 0)
    {
        qDebug() << "NtSetSystemInformation failed. NTSTATUS:" << QString("0x%1").arg(status, 8, 16, QLatin1Char('0')).toUpper();
        return false;
    }
    //LOG_MSG("Memory usage reduced successfully.");
    result = (status == 0); // NTSTATUS 0 = STATUS_SUCCESS
    emit emptySystemWorkingSetsFinished(result);
    return result;
}

void ProcessItemsList::runEmptySystemWorkingSets(ProcessItemsList *self)
{
    if (self)
        self->emptySystemWorkingSets();
}

bool ProcessItemsList::killProcessAndChildsByNameEx(const std::string &processName)
{
    QString qsProcessName, qsCurrentProcessName;
    qsProcessName = QString::fromStdString (processName);
    qsProcessName = qsProcessName.toUpper ();
    bool overallSuccess = false;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to get process snapshot. Error code: " << GetLastError() << std::endl;
        return false;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe32))
    {
        CloseHandle(hSnapshot);
        return false;
    }
    do
    {
        std::string currentProcessName = WcharToString(pe32.szExeFile);
        qsCurrentProcessName = QString::fromStdString (currentProcessName);
        qsCurrentProcessName = qsCurrentProcessName.toUpper ();
        // if (currentProcessName == processName) {
        if (qsCurrentProcessName == qsProcessName)
        {
            LOG_MSG( "Found process to kill: " << QString::fromStdString (processName) << " with PID: " << pe32.th32ProcessID);
            if (killProcessTree(pe32.th32ProcessID))
            {
                overallSuccess = true;
            }
        }
    }
    while (Process32Next(hSnapshot, &pe32));
    CloseHandle(hSnapshot);
    return overallSuccess;
}

bool ProcessItemsList::killProcessAndChildsByNameEx(QString sTargetName)
{
    std::string wideName = sTargetName.toStdString();
    return killProcessAndChildsByNameEx (wideName );
}

bool ProcessItemsList::killProcessTree(DWORD parentPid)
{
    bool terminationSuccessful = true;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to get process snapshot. Error code: " << GetLastError() << std::endl;
        return false;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe32))
    {
        CloseHandle(hSnapshot);
        return false;
    }
    do
    {
        if (pe32.th32ParentProcessID == parentPid)
        {
            // Found a child process, so kill its entire tree first.
            if (!killProcessTree(pe32.th32ProcessID))
            {
                terminationSuccessful = false;
            }
        }
    }
    while (Process32Next(hSnapshot, &pe32));
    CloseHandle(hSnapshot);
    // Now terminate the parent process itself.
    if (parentPid != 0)
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, parentPid);
        if (hProcess != NULL)
        {
            if (!TerminateProcess(hProcess, 1))
            {
                std::cerr << "Failed to terminate process with PID " << parentPid
                    << ". Error code: " << GetLastError() << std::endl;
                terminationSuccessful = false;
            }
            else
            {
                std::cout << "Process with PID " << parentPid << " terminated successfully." << std::endl;
            }
            CloseHandle(hProcess);
        }
        else
        {
            std::cerr << "Failed to open process with PID " << parentPid
                << ". Error code: " << GetLastError() << std::endl;
            terminationSuccessful = false;
        }
    }
    return terminationSuccessful;
}

QIcon ProcessItemsList::getProcessIcon(std::string sProcessPath, bool bIsFullPath)
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
    if ((hProc == INVALID_HANDLE_VALUE) | (hProc == 0 && bIsFullPath == false))
    {
        //LOG_VAR(QString::fromStdString (sProcessPath));
        //LOG_VAR(bIsFullPath);
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
    // QString sLogMessage = QString::fromStdString (sProcessPath);
    // sLogMessage.append (" does not have an icon, using default icon.");
    // LOG_MSG(sLogMessage);
    return icon;
}

HICON ProcessItemsList::getIconFromExecutable(const std::string &executablePath)
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

QString ProcessItemsList::getProcessPath(HANDLE hProcess)
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
    if ((hProcess == INVALID_HANDLE_VALUE) | (hProcess == 0)) return "";
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

HANDLE ProcessItemsList::getProcessHandle(const std::string &executablePath)
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

bool ProcessItemsList::processIsService(DWORD processId)
{
    // FIXME works?
    DWORD sessionId;
    // Questa funzione ottiene l'ID della sessione per un dato PID.
    // I servizi di Windows, per la maggior parte, girano nella sessione 0.
    // Le applicazioni utente girano in sessioni con ID maggiore di 0.
    if (ProcessIdToSessionId(processId, &sessionId))
    {
        if (sessionId == 0)
        {
            return true; // Molto probabile che sia un servizio
        }
    }
    return false; // Applicazione utente o errore
}

bool ProcessItemsList::processIsService(int iPos)
{
    // FIXME works?
    QString sName = m_ProcessItemsList.at (iPos).getAppName ();
    HANDLE hProcess = getProcessHandle(sName.toStdString ());
    bool bIsService = processIsService (DWORD(hProcess));
    return bIsService;
}

bool ProcessItemsList::processIsService(QString sName)
{
    ProcessItem *foundItem;
    int i_AppItemCount = m_ProcessList.count ();
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = &m_ProcessList[iCount];
        if (foundItem->getAppName () == sName)
        {
            return foundItem->getIsService ();
        }
    }
    return false;
}

QStringList ProcessItemsList::getRunningProcesses()
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

bool ProcessItemsList::enableDebugPrivileges()
{
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tp;
    // Open the current process token.
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        return false;
    }
    // Get the LUID for the SeDebugPrivilege.
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
    {
        CloseHandle(hToken);
        return false;
    }
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    // Enable the privilege.
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
    {
        CloseHandle(hToken);
        return false;
    }
    CloseHandle(hToken);
    return true;
}

bool ProcessItemsList::isRunning(QString sProcessName)
{
    sProcessName = sProcessName.toUpper ();
    if (m_ProcessList.isEmpty ())
        populateProcessList ();
    QString sCurProcess = "";
    bool bFound = false;
    int i_AppCount = m_ProcessList.count ();
    for (int iCount = 0; iCount < i_AppCount; iCount++)
    {
        sCurProcess = m_ProcessList[iCount].getAppName ();
        sCurProcess = sCurProcess.toUpper ();
        if (sCurProcess == sProcessName)
        {
            bFound = true;
            return true;
        }
    }
    return bFound;
}

void ProcessItemsList::clear()
{
    m_ProcessItemsList.clear ();
}

void ProcessItemsList::removeAt(int iIndex)
{
    m_ProcessItemsList.removeAt (iIndex);
}

int ProcessItemsList::size() const
{
    return m_ProcessItemsList.size ();
}

int ProcessItemsList::count() const
{
    return m_ProcessItemsList.count ();
}

ProcessItem *ProcessItemsList::at(int i)
{
    return &m_ProcessItemsList[i];
}

void ProcessItemsList::append(ProcessItem item)
{
    m_ProcessItemsList.append (item);
}

ProcessItem *ProcessItemsList::findApplicationItem(QString sFound)
{
    int i_AppItemCount = this->count ();
    ProcessItem *foundItem;
    // for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = this->at(iCount);
        if (foundItem->getAppName () == sFound) return foundItem;
    }
    return NULL;
}

ProcessItem *ProcessItemsList::findProcessItem(QString sFound)
{
    int i_AppItemCount = m_ProcessList.count ();
    ProcessItem *foundItem;
    // for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = &m_ProcessList[iCount];
        if (foundItem->getAppName ().toUpper () == sFound.toUpper ()) return foundItem;
    }
    return NULL;
}

int ProcessItemsList::findApplicationItemIndex(QString sFound)
{
    int i_AppItemCount = this->count ();
    ProcessItem *foundItem;
    // for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = this->at(iCount);
        if (foundItem->getAppName () == sFound) return iCount;
    }
    return -1;
}

void ProcessItemsList::resetAllApplicationItems()
{
    int i_AppItemCount = this->count ();
    ProcessItem *foundItem;
    // for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = this->at(iCount);
        foundItem->setFoundWhenKilling (true);
    }
}

bool ProcessItemsList::deleteApplicationItem(QString deleteString)
{
    bool bFound = false;
    QString itemString = "";
    // Iterate in reverse to safely remove items while modifying the list
    //qDebug()<< "m_ProcessItemsList.size="<<m_ProcessItemsList.size();
    int iFoundItem = this->findApplicationItemIndex (deleteString);
    if (iFoundItem != -1)
    {
        bFound = true;
        this->removeAt (iFoundItem);
        LOG_MSG( "m_ProcessItemsList.size= " << this->size());
    }
    // for (int i = m_ProcessItemsList.size() - 1; i >= 0; --i)
    // {
    //        //qDebug()<< "m_ProcessItemsList.at="<<i;
    //        //qDebug()<< m_ProcessItemsList.at(i).getAppName();
    // if (m_ProcessItemsList.at(i).getAppName() == deleteString)
    // {
    // m_ProcessItemsList.removeAt(i);
    // qDebug() << "m_ProcessItemsList.size=" << m_ProcessItemsList.size();
    // bFound = true;
    // }
    // for (int i = m_ProcessItemsList.count() - 1; i >= 0; --i) {
    // itemString = m_ProcessItemsList.at(i).getAppName();
    // if (itemString == deleteString) {
    // bFound = true;
    // m_ProcessItemsList.takeAt(i);
    // }
    // }
    if (!bFound)
        QMessageBox::information(nullptr, "NOT Removed", QString("NOT Removed all items with text '%1'.").arg(deleteString));
    return bFound;
}

bool ProcessItemsList::moveApplicationItem(QString deleteString, bool bState)
{
    bool bFound = false;
    QString itemString = "";
    // Iterate in reverse to safely remove items while modifying the list
    LOG_MSG( "m_ProcessItemsList.size= " << this->size());
    int iFoundItem = this->findApplicationItemIndex (deleteString);
    if (iFoundItem != -1)
    {
        bFound = true;
        this->at(iFoundItem)->setAppKillEnabled(bState);
        //LOG_MSG( "m_ProcessItemsList.size= " << this->size());
    }
    // for (int i = m_ProcessItemsList.size() - 1; i >= 0; --i)
    // {
    //        //qDebug()<< "m_ProcessItemsList.at="<<i;
    //        //qDebug()<< m_ProcessItemsList.at(i).getAppName();
    // if (m_ProcessItemsList.at(i).getAppName() == deleteString)
    // {
    //            //qDebug()<< "m_ProcessItemsList.at(i)="<<m_ProcessItemsList.at(i).getAppKillEnabled ();
    //            // ApplicationItem foundItem=m_ProcessItemsList.at(i);
    // m_ProcessItemsList[i].setAppKillEnabled(bState);
    // bFound = true;
    //            //qDebug()<< "m_ProcessItemsList.at(i)="<<m_ProcessItemsList.at(i).getAppKillEnabled ();
    //            // qDebug()<< "foundItem="<<foundItem.getAppKillEnabled ();
    // }
    //        // for (int i = m_ProcessItemsList.count() - 1; i >= 0; --i) {
    //        // itemString = m_ProcessItemsList.at(i).getAppName();
    //        // if (itemString == deleteString) {
    //        // bFound = true;
    //        // m_ProcessItemsList.takeAt(i);
    //        // }
    // }
    if (!bFound)
        QMessageBox::information(nullptr, "NOT moved", QString("NOT moved all items with text '%1'.").arg(deleteString));
    return bFound;
}

void ProcessItemsList::debugProcessList()
{
    int iCount = m_ProcessList.count ();
    for (int i = 0; i < iCount; i++)
    {
        qDebug() << i << " " << m_ProcessList.at(i).getAppName ();
    }
}

void ProcessItemsList::debugProcessItemsList ()
{
    int iCount = m_ProcessItemsList.count ();
    for (int i = 0; i < iCount; i++)
    {
        qDebug() << i << " " << m_ProcessItemsList.at(i).getAppName ();
    }
}

int ProcessItemsList::debugProcessesMemory()
{
    DWORD processIds[1024];
    DWORD bytesReturned;
    // Ottiene i PID di tutti i processi attivi
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned))
    {
        LOG_MSG("Errore in EnumProcesses.");
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

double ProcessItemsList::getFreeRAM()
{
    const double dBytesToMB = 1024.0 * 1024.0;
    // Declare a MEMORYSTATUSEX structure
    MEMORYSTATUSEX status;
    // Set the dwLength member to the size of the structure
    status.dwLength = sizeof(status);
    // Call the function to populate the structure
    if (GlobalMemoryStatusEx(&status))
    {
        // The function succeeded, now you can access the data
        // ullAvailPhys is the free physical RAM in bytes
        long long free_ram_bytes = status.ullAvailPhys;
        //std::cout << "Available Physical RAM: " << free_ram_bytes / dBytesToMB << " MB" << std::endl;
        double dFreeRam = free_ram_bytes / dBytesToMB;
        return dFreeRam;
    }
    else
    {
        // The function failed, you can get the error code
        std::cerr << "Failed to retrieve memory status. Error code: " << GetLastError() << std::endl;
        return 0.0;
    }
}

bool ProcessItem::getFoundWhenKilling() const
{
    return m_bFoundWhenKilling;
}

void ProcessItem::setFoundWhenKilling(bool value)
{
    m_bFoundWhenKilling = value;
}

bool ProcessItem::getIsService() const
{
    return m_bIsService;
}

void ProcessItem::setIsService(bool bIsService)
{
    m_bIsService = bIsService;
}

DWORD ProcessItem::getProcessID() const
{
    return m_ProcessID;
}

void ProcessItem::setProcessID(const DWORD &value)
{
    m_ProcessID = value;
}

DWORD ProcessItem::getThreadCount() const
{
    return m_ThreadCount;
}

void ProcessItem::setThreadCount(const DWORD &value)
{
    m_ThreadCount = value;
}

DWORD ProcessItem::getParentProcessID() const
{
    return m_ParentProcessID;
}

void ProcessItem::setParentProcessID(const DWORD &value)
{
    m_ParentProcessID = value;
}

DWORD ProcessItem::getPriority() const
{
    return m_Priority;
}

void ProcessItem::setPriority(const DWORD &value)
{
    m_Priority = value;
}

const std::map<DWORD, QString> ProcessItem::s_priorityMap =
{
    {4, "Idle (Low)"},
    {6, "Below normal"},
    {8, "Normal"},
    {10, "Above normal"},
    {13, "High"},
    {15, "Realtime"}
};

QString ProcessItem::getPriorityClassName()
{
    DWORD priorityClass = getPriority();
    //    // Mappa i valori di pcPriClassBase alle descrizioni testuali
    // std::map<DWORD, QString> priorityMap;
    // priorityMap[4] = "Idle (Low)";       // IDLE_PRIORITY_CLASS
    // priorityMap[6] = "Below normal"; // BELOW_NORMAL_PRIORITY_CLASS
    // priorityMap[8] = "Normal";    // NORMAL_PRIORITY_CLASS
    // priorityMap[10] = "Above normal"; // ABOVE_NORMAL_PRIORITY_CLASS
    // priorityMap[13] = "High";      // HIGH_PRIORITY_CLASS
    // priorityMap[15] = "Realtime"; // REALTIME_PRIORITY_CLASS
    // auto it = priorityMap.find(priorityClass);
    //std::map<DWORD, QString>::iterator it = priorityMap.find(priorityClass);
    std::map<DWORD, QString>::const_iterator it = s_priorityMap.find(priorityClass);
    if (it != s_priorityMap.end())
    {
        return it->second;
    }
    // DWORD priorityClass = getPriority ();
    // if (priorityClass == REALTIME_PRIORITY_CLASS)
    // {
    // return "Realtime";
    // }
    // else if (priorityClass == HIGH_PRIORITY_CLASS)
    // {
    // return "High";
    // }
    // else if (priorityClass == ABOVE_NORMAL_PRIORITY_CLASS)
    // {
    // return "Above normal";
    // }
    // else if (priorityClass == NORMAL_PRIORITY_CLASS)
    // {
    // return "Normal";
    // }
    // else if (priorityClass == BELOW_NORMAL_PRIORITY_CLASS)
    // {
    // return "Below normal";
    // }
    // else if (priorityClass == IDLE_PRIORITY_CLASS)
    // {
    // return "Idle (Low)";
    // }
    // return "Unknown";
}

bool ProcessItem::getIsProcessWindowVisible() const
{
    return m_IsProcessWindowVisible;
}

void ProcessItem::setIsProcessWindowVisible(bool value)
{
    m_IsProcessWindowVisible = value;
}

QString ProcessItem::getWindowTitle() const
{
    return m_WindowTitle;
}

void ProcessItem::setWindowTitle(const QString &value)
{
    m_WindowTitle = value;
}

QString ProcessItem::getProcessPath() const
{
    return m_ProcessPath;
}

void ProcessItem::setProcessPath(const QString &value)
{
    m_ProcessPath = value;
}

ProcessItem::ProcessItem(QString sAppName, bool bAppKillEnabled)
{
    appName = sAppName;
    m_bAppKillEnabled = bAppKillEnabled;
    m_bFoundWhenKilling = true;
}

bool ProcessItem::getAppKillEnabled() const
{
    return m_bAppKillEnabled;
}

void ProcessItem::setAppKillEnabled(bool value)
{
    m_bAppKillEnabled = value;
}

QString ProcessItem::getAppName() const
{
    return appName;
}

void ProcessItem::setAppName(const QString& value)
{
    appName = value;
}
