#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scheduler.h"
#include "utility.h"
#include "myqlistwidget.h"
#include "applicationitemslist.h"
#include "processitemslist.h"
#include "addexedialog.h"
#include "dialog.h"
#include <QList>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QProcess>
#include <QFont>
#include <QPalette>
#include <QTimer>
#include <QLabel>
#include <QMovie>
#include <QToolTip>

// Include Windows API headers
#ifdef Q_OS_WIN
    #include <windows.h>
    #include <psapi.h> // For EnumProcesses, EnumProcessModules, GetModuleBaseName
#endif
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    Scheduler m_scheduler;
    QLabel *m_StatusBarLabel;
    QMovie *m_statusBarMovie;
    bool listContainsItemText(MyQListWidget*, const QString & );
    void updateSettings();
    void deleteOldBackups();
    QFont m_Font;
    QBrush m_TextBrush;
    QBrush m_ServiceTextBrush;
    QTimer *timer;
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
    void showListWidgetEnabledContextMenu(const QPoint& pos);
    void showListWidgetDisabledContextMenu(const QPoint& pos);
    void killSelectedDisabledItem();
    void killSelectedEnabledItem();
    void copySelectedEnabledItem();
    void deleteSelectedEnabledItem();
    void deleteSelectedDisabledItem();
    void copySelectedDisabledItem();
    void disableSelectedEnabledItem();
    void enableSelectedDisabledItem();
    bool backupBatchFile();
    void addItemToListwidget(QListWidget*, QString);
    void debugNotFoundWhenKilling();
    void debugFoundWhenKilling();
    void disconnectTimer();
    void connectTimer();
    bool m_bTimerIsConnected;
    bool m_bKillInternal;
    int m_iTimerUpdatesCount;
    int m_iRefreshRate;
protected:
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent *event) override;
public slots:
    void timerUpdate();
    void menuConfigure();
    void firstTimeConfiguration();
    void showAddExeDialog();
    void loadListFromFile();
    bool writeListToFile();
private slots:
    void onLogCompressionExecuted();
    void onLogCompressionChecked();
    void readStdOutput();
    void readStdError();
    void onProcessFinished(int, QProcess::ExitStatus );
    void on_listWidgetEnabled_itemClicked(QListWidgetItem* item);
    void on_listWidgetDisabled_itemClicked(QListWidgetItem* item);
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

    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
