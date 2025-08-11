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
#include <QStandardPaths>
#include "utility.h"

Dialog::Dialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    this->setWindowIcon(QIcon(":/icons/img/KillManager.ico")); // Use the path defined in .qrc
    ui->setupUi(this);
    readSettings();
    // QApplication* currentApp = qApp;
    // QString orgName = currentApp->organizationName();
    // QString appName = currentApp->applicationName();
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
    QStringList nameFilters;
    nameFilters << "*.bat";
    QDir dir(m_sBackupInitialPath);
    QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int iFilesCount = files.count();
    files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot); //, QDir::Time);
    //std::sort(files.begin(), files.end(), compareByLastModified);
    iFilesCount = files.size();
    // ui->labelBackupsInfo->setText ("Currently the folder contains&nbsp;<b><span style=\"color:#ff0000;\">" + QString::number (iFilesCount) + "</span></b>" + "&nbsp;backups.");
    ui->labelBackupsInfo->setText ("Currently the folder contains " + QString::number (iFilesCount) + " backups.");
    // RunningProcessesListEx utility;
    // QIcon icon = utility.getProcessIcon (ui->lineEditExternaEditor ->text ().toStdString (), true);
    // ui->labelIcon->setPixmap (icon.pixmap (QSize(32, 32)));
    QString filePath = QCoreApplication::applicationDirPath();
    filePath = QDir::toNativeSeparators (filePath);
    filePath.append ("\\KillManagerQt.log");
    //LOG_MSG(filePath);
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists())
    {
        //file size in bytes
        int iFileSize = fileInfo.size();
        QString sMsg = "Path: ";
         sMsg.append (filePath);
         sMsg.append ("\nSize: ");
        if (iFileSize < 10240)
        {
            sMsg.append (QString::number (iFileSize));
            sMsg.append (" bytes.");
        }
        else
        {
            sMsg.append (QString::number (iFileSize/1024));
            sMsg.append (" KB.");
        }
        ui->labelLog->setText (sMsg);
    }
    else
    {
        // If the file doesn't exist, print a message and return 0
        qDebug() << "Log file " << filePath << " does not exist.";
        ui->labelLog->setText ("Log file doesn't exist.");
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::readSettings()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    documentsPath = QDir::toNativeSeparators (documentsPath);
    QSettings settings; // QSettings will use the organization and application names set in main()
    QString sKillFile = settings.value("Path").toString();
    if (sKillFile == "")
    {
        // sKillFile = "C:\\Users\\Andrea\\Documents\\kill.bat";
        sKillFile = "";
        qDebug() << "Read value is empty.";
    }
    m_sKillFile = sKillFile;
    m_sInitialPath = settings.value("Dialog/InitialPath", documentsPath).toString();
    m_sExternalEditorInitialPath = settings.value("Dialog/External editor", "C:\\Windows\\notepad.exe").toString();
    m_sBackupInitialPath = settings.value("Dialog/Backup path", documentsPath).toString ();
    int iRefreshRate = settings.value("Dialog/RefreshRate", 5).toInt(); // default to 5
    ui->spinBoxUpdateRate->setValue(iRefreshRate);
    bool bDeleteOldBackups = settings.value("Dialog/DeleteOldBackups", true).toBool ();
    int iBackupsCount = settings.value("Dialog/BackupsCount", 100).toInt();
    int iBackupsDays = settings.value("Dialog/BackupsDays", 30).toInt();
    if (bDeleteOldBackups)
    {
        ui->checkBoxDeleteBackups->setChecked (true);
        ui->spinBoxBackupsCount->setEnabled (true);
        ui->spinBoxBackupsDays->setEnabled (true);
    }
    else
    {
        ui->checkBoxDeleteBackups->setChecked (false);
        ui->spinBoxBackupsCount->setEnabled (false);
        ui->spinBoxBackupsDays->setEnabled (false);
    }
    ui->spinBoxBackupsCount->setValue (iBackupsCount);
    ui->spinBoxBackupsDays->setValue (iBackupsDays);
    bool bKillInternal = settings.value("Dialog/UseInternalKill", false).toBool ();
    if (bKillInternal)
    {
        ui->checkBoxKill->setChecked (true);
    }
    else
    {
        ui->checkBoxKill->setChecked (false);
    }
    bool bLogToFile = settings.value("Dialog/UseLogFile", false).toBool ();
    if (bLogToFile)
    {
        ui->checkBoxLog->setChecked (true);
    }
    else
    {
        ui->checkBoxLog->setChecked (false);
    }
}

void Dialog::writeSettings()
{
    QSettings settings; // QSettings will use the organization and application names set in main()
    settings.setValue("Dialog/InitialPath", m_sInitialPath);
    settings.setValue("Dialog/External editor", m_sExternalEditorInitialPath);
    settings.setValue("Dialog/Backup path", m_sBackupInitialPath);
    int iRefreshRate = ui->spinBoxUpdateRate->value ();
    settings.setValue("Dialog/RefreshRate", iRefreshRate);
    settings.sync();
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
            "Choose batch file",
            initialPath, // Start in user's home directory
            "Batch Files (*.bat)", 0, 0);
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
    m_sInitialPath = QDir::toNativeSeparators(myFile.absolutePath());
    settings.setValue("Dialog/InitialPath", m_sInitialPath);
    settings.setValue("Dialog/External editor", m_sExternalEditorInitialPath);
    settings.setValue("Dialog/Backup path", ui->lineEditBackupPath->text ());
    settings.setValue("Dialog/RefreshRate", ui->spinBoxUpdateRate->value ());
    settings.setValue("Dialog/DeleteOldBackups", ui->checkBoxDeleteBackups->isChecked ());
    settings.setValue("Dialog/BackupsCount", ui->spinBoxBackupsCount->value ());
    settings.setValue("Dialog/BackupsDays", ui->spinBoxBackupsDays->value ());
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
    settings.setValue("Dialog/UseInternalKill", ui->checkBoxKill->isChecked ());
    settings.setValue("Dialog/UseLogFile", ui->checkBoxLog->isChecked ());
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
            "Choose external editor",
            initialPath, // Start in user's home directory
            "*.exe ;; *.exe", 0, 0);
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
    RunningProcessesListEx utility;
    QIcon icon = utility.getProcessIcon (ui->lineEditExternaEditor ->text ().toStdString (), true);
    ui->labelIcon->setPixmap (icon.pixmap (QSize(32, 32)));
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

void Dialog::on_pushButtonCreateNew_clicked()
{
    QString defaultName = m_sInitialPath + "\\" + "kill.bat" ;
    QString fileName = QFileDialog::getSaveFileName(this,
            "Create new batch file",
            defaultName,
            "Batch Files (*.bat)");
    if (!fileName.isEmpty())
    {
        fileName = QDir::toNativeSeparators(fileName);
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            out << "";  // Write nothing to create an empty file
            file.close();
            QMessageBox::information(this, "New batch file created", "The New batch file was successfully created:\n" + fileName);
        }
        else
        {
            QMessageBox::critical(this, "New batch file error", "Failed to create the new batch file:\n" + fileName);
        }
        ui->lineEditPath->setText (fileName);
        QFileInfo myFile(fileName);
        m_sInitialPath = QDir::toNativeSeparators(myFile.absolutePath());
        QSettings settings;
        settings.setValue("Dialog/InitialPath", m_sInitialPath);
        settings.setValue("Path", fileName);
        settings.sync();
    }
    // QFile file(m_sKillFile);
    //    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    //    // {
    //    // qWarning() << "Could not open file:" << fileName;
    //    //        //return;
    //    // }
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    // {
    // qDebug() << "File not found or unreadable. Creating it...";
    //        // Try opening for writing to create the file
    // if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    // {
    // QTextStream out(&file);
    // out << "";  // Write empty content or initial data
    // file.close();
    // }
    // else
    // {
    // qWarning() << "Failed to create file:" << file.errorString();
    // return;
    // }
    //        // Reopen for reading
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    // {
    // qWarning() << "Failed to reopen file for reading.";
    // return;
    // }
    // }
}

void Dialog::on_checkBoxDeleteBackups_toggled(bool checked)
{
    if (checked)
    {
        ui->spinBoxBackupsCount->setEnabled (true);
        ui->spinBoxBackupsDays->setEnabled (true);
    }
    else
    {
        ui->spinBoxBackupsCount->setEnabled (false);
        ui->spinBoxBackupsDays->setEnabled (false);
    }
}

void Dialog::on_lineEditExternaEditor_textChanged(const QString &arg1)
{
    bool bIsFullPath = false;
    if (arg1.contains ("\\")) bIsFullPath = true;
    m_sExternalEditorInitialPath = arg1;
    RunningProcessesListEx utility;
    QIcon icon;
    if (bIsFullPath)
        icon = utility.getProcessIcon (arg1.toStdString (), true);
    else
        icon = utility.getProcessIcon (arg1.toStdString (), false);
    ui->labelIcon->setPixmap (icon.pixmap (QSize(32, 32)));
    LOG_MSG(arg1);
}
