#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui
{
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButtonChoose_clicked();
    void on_Dialog_accepted();
    void on_pushButtonChooseExternaEditor_clicked();

    void on_pushButtonChooseBackupPath_clicked();

    void on_pushButtonOpenBackup_clicked();

    void on_pushButtonCreateNew_clicked();

    void on_checkBoxDeleteBackups_toggled(bool checked);

private:
    Ui::Dialog *ui;
    QString m_sKillFile;
    QString m_sInitialPath;
    QString m_sExternalEditorInitialPath;
    QString m_sBackupInitialPath;
    void readSettings();
    void writeSettings();

};

#endif // DIALOG_H
