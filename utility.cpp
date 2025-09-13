#include "utility.h"
#include <QMessageLogContext>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

//QStringList g_debugMessages;
//FILE *f;
//QString sLogFilePath;
//char *cLogFilePath;
// Dichiariamo una QMutex per rendere il nostro gestore di messaggi thread-safe

//void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
//{
//    // if (sLogFilePath==nullptr) buildPath();
//    // QString sFile = qApp->applicationName();
//    // sFile.append (".log");
// QString message = qFormatLogMessage(type, context, msg);
// f = fopen("KillManagerQt.log", "a");
//    //LOG_VAR(sLogFilePath);
//    //f = fopen(cLogFilePath, "a");
// fprintf(f, "%s\n", qPrintable(message));
// fflush(f);
// Q_UNUSED(type)
// Q_UNUSED(context)
// g_debugMessages.append(msg);
// std::cout << msg.toStdString () << std::endl;
//}

//void closeLogFile()
//{
// if (f != NULL)
// {
// LOG_MSG("Closing LOG file...");
// fclose(f);
// f = NULL;
// }
//}

//QStringList MyUtility::getRunningProcesses()
//{
// QStringList processList;
//#ifdef Q_OS_WIN
// DWORD aProcesses[1024], cbNeeded, cProcesses;
// unsigned int i;
//    // Get the list of process identifiers.
// if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
// {
// qWarning() << "EnumProcesses failed!";
// return processList;
// }
//    // Calculate how many process identifiers were returned.
// cProcesses = cbNeeded / sizeof(DWORD);
//    // Iterate through each process to get its name.
// for (i = 0; i < cProcesses; i++)
// {
// if (aProcesses[i] == 0)
// continue;
//        // Open process with necessary access rights
// HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
// if (NULL != hProcess)
// {
// HMODULE hMod;
// DWORD cbNeededModule;
// char szProcessName[MAX_PATH] = { 0 };
//            // Get a list of all modules in the process.
// if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule))
// {
//                // Get the base name of the first module (usually the executable).
// GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
// }
//            // If we got a name, add it to our list
// if (szProcessName[0] != '\0')
// {
// processList << QString::fromLocal8Bit(szProcessName);
// }
//            // Release the handle to the process.
// CloseHandle(hProcess);
// }
// }
//#else
//    // For non-Windows platforms, you would use different methods.
//    // For example, on Linux, you might read from /proc/<pid>/comm or use 'ps' command.
// qDebug() << "This function is designed for Windows. Other OS not implemented.";
// processList << "Not available on this OS.";
//#endif
//    // Sort the list for better readability
// processList.sort();
// return processList;
//}

//bool MyUtility::KillRunningProcesses()
//{
// QStringList processList;
//#ifdef Q_OS_WIN
// DWORD aProcesses[1024], cbNeeded, cProcesses;
// unsigned int i;
//    // Get the list of process identifiers.
// if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
// {
// LOG_MSG("EnumProcesses failed!");
// return false;
// }
//    // Calculate how many process identifiers were returned.
// cProcesses = cbNeeded / sizeof(DWORD);
//    // Iterate through each process to get its name.
// for (i = 0; i < cProcesses; i++)
// {
// if (aProcesses[i] == 0)
// continue;
//        // Open process with necessary access rights
// HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
// if (NULL != hProcess)
// {
// HMODULE hMod;
// DWORD cbNeededModule;
// char szProcessName[MAX_PATH] = { 0 };
//            // Get a list of all modules in the process.
// if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule))
// {
//                // Get the base name of the first module (usually the executable).
// GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
// }
//            // If we got a name, add it to our list
// if (szProcessName[0] != '\0')
// {
// processList << QString::fromLocal8Bit(szProcessName);
// }
//            // Release the handle to the process.
// CloseHandle(hProcess);
// }
// }
//#else
//    // For non-Windows platforms, you would use different methods.
//    // For example, on Linux, you might read from /proc/<pid>/comm or use 'ps' command.
// qDebug() << "This function is designed for Windows. Other OS not implemented.";
// processList << "Not available on this OS.";
//#endif
//    // Sort the list for better readability
// processList.sort();
// return true;
//}

// windowvisibilitychecker.cpp

//WindowVisibilityChecker::WindowVisibilityChecker(QObject *parent)
// : QObject(parent)
//{
//}

// Public method for QWidget
bool WindowVisibilityChecker::isWidgetFullyVisible(QWidget *widget)
{
    if (!widget)
        return false;
    // QWidget -> HWND
    HWND hWnd = reinterpret_cast<HWND>(widget->winId());
    return checkFullWindowVisibility(hWnd);
}

bool WindowVisibilityChecker::isWidgetVisible(QWidget *widget)
{
    if (!widget)
        return false;
    // QWidget -> HWND
    HWND hWnd = reinterpret_cast<HWND>(widget->winId());
    return checkWindowVisibility(hWnd);
}

// Public method for QWindow
bool WindowVisibilityChecker::isQWindowFullyVisible(QWindow *window)
{
    if (!window)
        return false;
    HWND hWnd = reinterpret_cast<HWND>(window->winId());
    return checkFullWindowVisibility(hWnd);
}

bool WindowVisibilityChecker::isQWindowVisible(QWindow *window)
{
    if (!window)
        return false;
    HWND hWnd = reinterpret_cast<HWND>(window->winId());
    return checkWindowVisibility(hWnd);
}

// Core logic (Win32 API)
bool WindowVisibilityChecker::checkWindowVisibility(HWND hWnd)
{
    if (!IsWindow(hWnd) || !IsWindowVisible(hWnd))
        return false;
    if (IsIconic(hWnd))  // minimized
        return false;
    RECT rect;
    if (!GetWindowRect(hWnd, &rect))
        return false;
    DWORD mainProcessId = 0;
    GetWindowThreadProcessId(hWnd, &mainProcessId); // PID of the main window
    // Points to check (4 corners + center)
    POINT points[5] =
    {
        {rect.left + 5, rect.top + 5},
        {rect.right - 5, rect.top + 5},
        {rect.left + 5, rect.bottom - 5},
        {rect.right - 5, rect.bottom - 5},
        {(rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2}
    };
    for (int i = 0; i < 5; ++i)
    {
        HWND hAtPoint = WindowFromPoint(points[i]);
        DWORD pointProcessId = 0;
        GetWindowThreadProcessId(hAtPoint, &pointProcessId);
        if (pointProcessId != mainProcessId)
        {
            // Another window is on top at this point
            return false;
        }
    }
    return true; // Window seems fully visible
}

bool WindowVisibilityChecker::checkFullWindowVisibility(HWND hWnd)
{
    if (!IsWindow(hWnd) || !IsWindowVisible(hWnd))
        return false;
    if (IsIconic(hWnd))  // minimized
        return false;
    RECT rect;
    if (!GetWindowRect(hWnd, &rect))
        return false;
    DWORD mainProcessId = 0;
    GetWindowThreadProcessId(hWnd, &mainProcessId); // PID of the main window
    // Sample points: 4 corners + center
    POINT points[5] =
    {
        {rect.left + 5, rect.top + 5},                     // top-left
        {rect.right - 5, rect.top + 5},                    // top-right
        {rect.left + 5, rect.bottom - 5},                  // bottom-left
        {rect.right - 5, rect.bottom - 5},                 // bottom-right
        {(rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2} // center
    };
    int coveredPoints = 0;
    for (int i = 0; i < 5; ++i)
    {
        HWND hAtPoint = WindowFromPoint(points[i]);
        DWORD pointProcessId = 0;
        GetWindowThreadProcessId(hAtPoint, &pointProcessId);
        // Count points that are covered by another window
        if (pointProcessId != mainProcessId)
        {
            ++coveredPoints;
        }
    }
    // Only consider window "not visible" if all points are covered
    return (coveredPoints < 5);
}
