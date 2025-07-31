#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "addexedialog.h"
#include "applicationitem.h"
#include "dialog.h"
#include <QList>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QProcess>

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
    static QString getKillFilePath();
private:
    QProcess *process;
    Ui::MainWindow *ui;
    QString m_sKillFile;
    QString m_sInitialPath;
    QString m_sAppName;
    QString m_sAppCompany;
    QList<ApplicationItem> m_ApplicationItemsList;
    QStringList getRunningProcesses();
    bool deleteApplicationItem(QString);
    void resetAllApplicationItems();
    bool moveApplicationItem(QString, bool);
    void readSettings();
    void writeSettings();
    void loadListFromFile(const QString &);
    void showListWidgetEnabledContextMenu(const QPoint& pos);
    void showListWidgetDisabledContextMenu(const QPoint& pos);
    void copySelectedEnabledItem();
    void deleteSelectedEnabledItem();
    void deleteSelectedDisabledItem();
    void copySelectedDisabledItem();
    void disableSelectedEnabledItem();
    void enableSelectedDisabledItem();
    bool backupBatchFile();
    void addItemToListwidget(QListWidget*, QString);
    ApplicationItem* findApplicationItem (QString);
    int findApplicationItemIndex (QString);
    void debugNotFoundWhenKilling();
    void debugFoundWhenKilling();
protected:
    void closeEvent(QCloseEvent* event) override;
public slots:
    void menuConfigure();
    void showAddExeDialog();
    void loadListFromFile();
    bool writeListToFile();
private slots:
    void readStdOutput();
    void readStdError();
    void onProcessFinished(int , QProcess::ExitStatus );
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
};

#endif // MAINWINDOW_H
