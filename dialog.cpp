#include "dialog.h"
#include "mainwindow.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

Dialog::Dialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    this->setWindowIcon(QIcon(":/icons/img/KillManager.ico")); // Use the path defined in .qrc
    ui->setupUi(this);
    readSettings();
    QApplication* currentApp = qApp;
    QString orgName = currentApp->organizationName();
    QString appName = currentApp->applicationName();
    // QSettings settings;
    // QString readValue = settings.value("Path").toString();
    // qDebug() << "Read string from registry:" << readValue;
    // if (readValue == "") {
    // qDebug() << "Read value is empty.";
    // } else {
    // ui->lineEditPath->setText(readValue);
    // qDebug() << "Read value OK";
    // }
    // MainWindow::updatePaths();
    // QString readValue = MainWindow::getKillFile();
    //   //QString readValue = MainWindow::updatePaths();
    ui->lineEditPath->setText(m_sKillFile);
    ui->lineEditExternaEditor->setText(m_sExternalEditorInitialPath);
    ui->lineEditBackupPath->setText(m_sBackupInitialPath);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::readSettings()
{
    QSettings settings; // QSettings will use the organization and application names set in main()
    QString sKillFile = settings.value("Path").toString();
    if (sKillFile == "")
    {
        sKillFile = "C:\\Users\\Andrea\\Documents\\kill.bat";
        qDebug() << "Read value is empty.";
    }
    m_sKillFile = sKillFile;
    m_sInitialPath = settings.value("Dialog/InitialPath", QDir::homePath()).toString();
    m_sExternalEditorInitialPath = settings.value("Dialog/External editor", "notepad.exe").toString();
    m_sBackupInitialPath = settings.value("Dialog/Backup path", QDir::homePath()).toString();
}

void Dialog::writeSettings()
{
    QSettings settings; // QSettings will use the organization and application names set in main()
    settings.setValue("Dialog/InitialPath", m_sInitialPath);
    settings.setValue("Dialog/External editor", m_sExternalEditorInitialPath);
    settings.setValue("Dialog/Backup path", m_sBackupInitialPath);
}

void Dialog::on_pushButtonChoose_clicked()
{
    // QApplication* currentApp = qApp;
    // QString orgName = currentApp->organizationName();
    // QString appName = currentApp->applicationName();
    // qDebug() << orgName;
    // qDebug() << appName;
    // QSettings settings;
    // QString readValue = settings.value("Path").toString();
    // qDebug() << "Read string from registry:" << readValue;
    // if (readValue == "") {
    // qDebug() << "Read value is empty.";
    // } else {
    //        // ui->lineEditPath->setText (readValue);
    // qDebug() << "Read value OK";
    // }
    QString initialPath = ui->lineEditPath->text();
    if (initialPath.isEmpty())
        //initialPath = QDir::homePath();
        initialPath = m_sInitialPath;
    QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("Choose kill.bat file"),
            initialPath, // Start in user's home directory
            tr("kill.bat ;; *.bat"), 0, 0);
    if (!filePath.isEmpty())
    {
        filePath = QDir::toNativeSeparators(filePath);
        ui->lineEditPath->setText(filePath);
        qDebug() << "User selected file:" << filePath;
        // You can now use filePath to open and read the file
        // QFileInfo myFile(filePath);
        // m_sInitialPath = myFile.absolutePath();
        // writeSettings();
    }
    else
    {
        // fileNameLabel->setText("No file selected.");
        qDebug() << "File dialog cancelled or no file selected.";
    }
}

void Dialog::on_Dialog_accepted()
{
    QSettings settings; // Uses QCoreApplication::organizationName() and applicationName()
    // --- Writing a string to the registry ---
    QString mySettingValue = ui->lineEditPath->text();
    settings.setValue("Path", mySettingValue); // Key path: "Group/KeyName"
    QFileInfo myFile(mySettingValue);
    m_sInitialPath = myFile.absolutePath();
    settings.setValue("Dialog/InitialPath", m_sInitialPath);
    settings.setValue("Dialog/External editor", m_sExternalEditorInitialPath);
    settings.setValue("Dialog/Backup path", ui->lineEditBackupPath->text ());
    qDebug() << "Wrote string to registry (or equivalent):" << mySettingValue;
    // create folder if does not exists
    QString folderPath = ui->lineEditBackupPath->text ();
    QDir dirBackup;
    if (!dirBackup.exists(folderPath))
    {
        if (dirBackup.mkpath(folderPath))
        {
            QMessageBox::information(this, "Backup folder created", "The backup folder was successfully created:\n" + folderPath);
        }
        else
        {
            QMessageBox::critical(this, "Backup folder error", "Failed to create the backup folder:\n" + folderPath);
        }
    }
    // else
    // {
    // QMessageBox::information(parent, "Folder Exists",
    // "The folder already exists:\n" + folderPath);
    // }
    // --- You can also write other data types ---
    //settings.setValue("User/Age", 30);
    //settings.setValue("Graphics/FullScreen", true);
    // QSettings changes are not always written to persistent storage immediately
    // Forcing a write can be done with sync()
    settings.sync();
    qDebug() << "Settings synchronized to persistent storage.";
    // QFileInfo myFile(mySettingValue);
    // m_sInitialPath = myFile.absolutePath();
    //writeSettings();
    //return 0; // No event loop needed for this simple example
}

void Dialog::on_pushButtonChooseExternaEditor_clicked()
{
    QString initialPath = ui->lineEditExternaEditor->text();
    if (initialPath.isEmpty())
        //initialPath = QDir::homePath();
        initialPath = m_sExternalEditorInitialPath;
    QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("Choose external editor"),
            initialPath, // Start in user's home directory
            tr("*.exe ;; *.exe"), 0, 0);
    if (!filePath.isEmpty())
    {
        filePath = QDir::toNativeSeparators(filePath);
        ui->lineEditExternaEditor->setText(filePath);
        qDebug() << "User selected file:" << filePath;
        // You can now use filePath to open and read the file
        // QFileInfo myFile(filePath);
        // m_sInitialPath = myFile.absolutePath();
        // writeSettings();
        m_sExternalEditorInitialPath = filePath;
    }
    else
    {
        // fileNameLabel->setText("No file selected.");
        qDebug() << "File dialog cancelled or no file selected.";
    }
}

void Dialog::on_pushButtonChooseBackupPath_clicked()
{
    QString initialPath = ui->lineEditBackupPath->text();
    if (initialPath.isEmpty())
        //initialPath = QDir::homePath();
        initialPath = m_sBackupInitialPath;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setWindowTitle("Choose the backup folder");
    dialog.setDirectory (initialPath);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString folderPath = dialog.selectedFiles().first();
        folderPath = QDir::toNativeSeparators(folderPath);
        qDebug() << "Selected folder:" << folderPath;
        m_sBackupInitialPath = folderPath;
        ui->lineEditBackupPath->setText (m_sBackupInitialPath);
    }
    else
    {
        qDebug() << "No folder selected.";
        //return QString();
    }
    // QString initialPath = ui->lineEditExternaEditor->text();
    // if (initialPath.isEmpty())
    //        //initialPath = QDir::homePath();
    // initialPath = m_sExternalEditorInitialPath;
    // QString filePath = QFileDialog::getOpenFileName(
    // this,
    // tr("Choose external editor"),
    // initialPath, // Start in user's home directory
    // tr("*.exe ;; *.exe"), 0, 0);
    // if (!filePath.isEmpty())
    // {
    // filePath = QDir::toNativeSeparators(filePath);
    // ui->lineEditExternaEditor->setText(filePath);
    // qDebug() << "User selected file:" << filePath;
    //        // You can now use filePath to open and read the file
    //        // QFileInfo myFile(filePath);
    //        // m_sInitialPath = myFile.absolutePath();
    //        // writeSettings();
    // m_sExternalEditorInitialPath = filePath;
    // }
    // else
    // {
    //        // fileNameLabel->setText("No file selected.");
    // qDebug() << "File dialog cancelled or no file selected.";
    // }
}

void Dialog::on_pushButtonOpenBackup_clicked()
{
   QDesktopServices::openUrl(QUrl::fromLocalFile(ui->lineEditBackupPath->text()));

}
