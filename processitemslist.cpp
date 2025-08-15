#include "processitemslist.h"

#include <QDebug>
#include <QMessageBox>

// TODO Rename WcharToString2 to WcharToString
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

//// Struttura per passare i dati alla callback di EnumWindows
//struct WindowInfo
//{
//    DWORD processId;
//    QString windowTitle;
//};

bool ProcessItemsList::g_windowIsVisible = false;

// La callback viene eseguita per ogni finestra
BOOL CALLBACK ProcessItemsList::EnumWindowsCallback2(HWND hwnd, LPARAM lParam)
{
    ProcessItemsList::WindowInfo* info = reinterpret_cast<ProcessItemsList::WindowInfo *>(lParam);
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
BOOL CALLBACK ProcessItemsList::EnumWindowsCallback(HWND hwnd, LPARAM lParam)
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
    ProcessItemsList::WindowInfo info;
    info.processId = processId;
    info.windowTitle = ""; // Inizializziamo il titolo a una stringa vuota.
    // Enumera le finestre e popola la nostra struttura
    EnumWindows(EnumWindowsCallback2, reinterpret_cast<LPARAM>(&info));
    return info.windowTitle;
}

// Funzione helper per verificare se un processo ha una finestra visibile.
bool ProcessItemsList::isProcessWindowVisible(DWORD processId)
{
    g_windowIsVisible = false;
    EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&processId));
    return g_windowIsVisible;
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
            //qDebug() << __FUNCTION__ << processName;
            ProcessItem newItem(processName, true);
            bool bIsService = processIsService(pe32.th32ProcessID);
            newItem.setIsService (bIsService);
            newItem.setProcessID (pe32.th32ProcessID);
            newItem.setThreadCount (pe32.cntThreads);
            newItem.setParentProcessID (pe32.th32ParentProcessID);
            newItem.setPriority (pe32.pcPriClassBase);
            DWORD processId = pe32.th32ProcessID;
            if (isProcessWindowVisible(processId))
            {
                // Il processo ha una finestra visibile.
                newItem.setIsProcessWindowVisible (true);
                //qDebug() << newItem.getAppName () << " VISIBLE";
                // Otteniamo il titolo della finestra
                QString windowTitle = getWindowTitle(processId);
                // Verifichiamo se abbiamo trovato un titolo
                //QString windowStatus;
                if (!windowTitle.isEmpty())
                {
                    //windowStatus = "Visible Window Title: " + windowTitle;
                    newItem.setWindowTitle (windowTitle);
                    //qDebug() << windowTitle;
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
            m_ProcessList.append (newItem);
        }
        while (Process32Next(hProcessSnap, &pe32));
    }
    CloseHandle(hProcessSnap);
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

QString ProcessItem::getPriorityClassName()
{
    DWORD priorityClass = getPriority ();
    if (priorityClass == REALTIME_PRIORITY_CLASS)
    {
        return "Realtime";
    }
    else if (priorityClass == HIGH_PRIORITY_CLASS)
    {
        return "High";
    }
    else if (priorityClass == ABOVE_NORMAL_PRIORITY_CLASS)
    {
        return "Above normal";
    }
    else if (priorityClass == NORMAL_PRIORITY_CLASS)
    {
        return "Normal";
    }
    else if (priorityClass == BELOW_NORMAL_PRIORITY_CLASS)
    {
        return "Below normal";
    }
    else if (priorityClass == IDLE_PRIORITY_CLASS)
    {
        return "Idle (Low)";
    }
    return "Unknown";
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
