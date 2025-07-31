#ifndef ADDEXEDIALOG_H
#define ADDEXEDIALOG_H

#include <QDialog>

namespace Ui
{
class AddExeDialog;
}

class AddExeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddExeDialog(QWidget* parent = 0);
    ~AddExeDialog();
    QString getText() const; // Public method to retrieve the text
private slots:
    void on_pushButtonChoose_clicked();
    void on_AddExeDialog_accepted();
private:
    Ui::AddExeDialog *ui;
    QString m_sInitialPath;
    void readSettings();
    void writeSettings();
};

#endif // ADDEXEDIALOG_H
