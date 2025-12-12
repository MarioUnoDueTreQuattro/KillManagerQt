#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "clickable_label.h"
#include "myqlistwidget.h"
#include "scheduler.h"
#include "utility.h"
//#include "applicationitemslist.h"
#include "addexedialog.h"
#include "dialog.h"
#include "processitemslist.h"
#include <QFont>
#include <QLabel>
#include <QList>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMovie>
#include <QPalette>
#include <QProcess>
#include <QTimer>
#include <QToolTip>
//#include <3rdparty/qhotkey/QHotkey/QHotkey>

// Include Windows API headers
#ifdef Q_OS_WIN
    #include <psapi.h> // For EnumProcesses, EnumProcessModules, GetModuleBaseName
    #include <windows.h>
#endif
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static bool compareByLastModified(const QFileInfo &a, const QFileInfo &b);

private:
    //QHotkey *m_hotkey;
    bool m_bIsCleaningMemory;
    Scheduler m_scheduler;
    QLabel *m_StatusBarLabel;
    QMovie *m_statusBarMovie;
    ClickableLabel *m_StatusBarRam;
    bool listContainsItemText(MyQListWidget *, const QString &);
    void updateSettings();
    void deleteOldBackups();
    QFont m_Font;
    QBrush m_TextBrush;
    QBrush m_ServiceTextBrush;
    QTimer *timer;
    QTimer *exitTimer;
    QProcess *process;
    Ui::MainWindow *ui;
    QString m_sLogFileName;
    QString m_sKillFile;
    QString m_sInitialPath;
    QString m_sBackupPath;
    QString m_sSelectedEnabledItem;
    QString m_sSelectedDisabledItem;
    // QString m_sAppName;
    // QString m_sAppCompany;
    //QList<ApplicationItem> m_ApplicationItemsList;
    ProcessItemsList m_ProcessListEx;
    //RunningProcessesListEx m_ProcessList;
    //ApplicationItemsList m_ApplicationItemsList;
    bool KillRunningProcesses();
    void readSettings();
    void writeSettings();
    void loadListFromFile(const QString &);
    void showListWidgetEnabledContextMenu(const QPoint &pos);
    void showListWidgetDisabledContextMenu(const QPoint &pos);
    void searchSelectedEnabledItem();
    void searchSelectedDisabledItem();
    void killSelectedDisabledItem();
    void killSelectedEnabledItem();
    void copySelectedEnabledItem();
    void deleteSelectedEnabledItem();
    void deleteSelectedDisabledItem();
    void copySelectedDisabledItem();
    void disableSelectedEnabledItem();
    void enableSelectedDisabledItem();
    bool backupBatchFile();
    void addItemToListwidget(QListWidget *, QString);
    void debugNotFoundWhenKilling();
    void debugFoundWhenKilling();
    void disconnectTimer();
    void connectTimer();
    bool m_bTimerIsConnected;
    bool m_bKillInternal;
    int m_iTimerUpdatesCount;
    int m_iRefreshRate;
    QString getListItemTooltip(QString);
    void setListItemTooltip(QListWidgetItem *);
    bool registerGlobalHotkey();
    void unregisterGlobalHotkey();
    static const int HOTKEY_ID = 1; // Identifier for our hotkey
    void adjustLabelHeightToContent(QLabel *label);

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void changeEvent(QEvent *event) override;
public slots:
    void openGoogleSearch(const QString &text);
    void timerUpdate();
    void menuConfigure();
    void firstTimeConfiguration();
    void showAddExeDialog();
    void loadListFromFile();
    bool writeListToFile();
private slots:
    void updateFreeRAM();
    void onBatchCompressionExecuted();
    void onLogCompressionExecuted();
    void onLogCompressionChecked();
    void readStdOutput();
    void readStdError();
    void onProcessFinished(int, QProcess::ExitStatus);
    void on_listWidgetEnabled_itemClicked(QListWidgetItem *item);
    void on_listWidgetDisabled_itemClicked(QListWidgetItem *item);
    void on_pushButtonWrite_clicked();
    void on_pushButtonReload_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonRun_clicked();
    void on_actionConfigure_app_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionAbout_triggered();
    void on_actionOpen_in_external_editor_triggered();
    void on_actionExecute_in_terminal_window_triggered();
    void on_actionOpen_log_file_in_external_editor_triggered();
    void on_actionReduce_RAM_memory_usage_triggered();
    void on_actionExit_triggered();
    void onReduceMemoryUsageFinished(bool success);
    void exitTimerUpdate();
};

#endif // MAINWINDOW_H
