#include "addexedialog.h"
#include "mainwindow.h"
#include "ui_addexedialog.h"
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>

AddExeDialog::AddExeDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AddExeDialog)
{
    ui->setupUi(this);
    readSettings();
}

AddExeDialog::~AddExeDialog() { delete ui; }

QString AddExeDialog::getText() const { return ui->lineEditPath->text(); }

void AddExeDialog::on_pushButtonChoose_clicked()
{

    QString initialPath = m_sInitialPath;
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("Choose executable application"),
        initialPath, // Start in user's home directory
        tr("Executable (*.exe)"), 0, 0);
    if (!filePath.isEmpty()) {
        filePath = QDir::toNativeSeparators(filePath);
        QStringList line_parts = filePath.split('\\');
        QString line_parts_last = line_parts.last();
        ui->lineEditPath->setText(line_parts_last);
        // QListWidgetItem* newitem = new QListWidgetItem(line_parts_last,
        // ui->listWidgetEnabled); ui->listWidgetEnabled->addItem(newitem);
        qDebug() << "User selected file:" << line_parts_last;
        // You can now use filePath to open and read the file
        //        QDir myDir;
        //        m_sInitialPath = myDir.absoluteFilePath (filePath);
        QFileInfo myFile(filePath);
        m_sInitialPath = myFile.absolutePath();
        writeSettings();
    } else {
        //                fileNameLabel->setText("No file selected.");
        qDebug() << "File dialog cancelled or no file selected.";
    }
}

void AddExeDialog::on_AddExeDialog_accepted()
{
}

void AddExeDialog::readSettings()
{
    QSettings settings; // QSettings will use the organization and application names set in main()
    m_sInitialPath = settings.value("AddExeDialog/InitialPath", QDir::homePath()).toString();
}

void AddExeDialog::writeSettings()
{
    QSettings settings; // QSettings will use the organization and application names set in main()
    settings.setValue("AddExeDialog/InitialPath", m_sInitialPath);
}
