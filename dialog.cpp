#include "dialog.h"
#include "mainwindow.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>

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
    QString readValue = MainWindow::getKillFilePath();
    ui->lineEditPath->setText(readValue);
    ui->lineEditExternaEditor->setText(m_sExternalEditorInitialPath);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::readSettings()
{
    QSettings settings; // QSettings will use the organization and application names set in main()
    m_sInitialPath = settings.value("Dialog/InitialPath", QDir::homePath()).toString();
    m_sExternalEditorInitialPath = settings.value("Dialog/External editor", "notepad.exe").toString();
}

void Dialog::writeSettings()
{
    QSettings settings; // QSettings will use the organization and application names set in main()
    settings.setValue("Dialog/InitialPath", m_sInitialPath);
    settings.setValue("Dialog/External editor", m_sExternalEditorInitialPath);
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
    qDebug() << "Wrote string to registry (or equivalent):" << mySettingValue;
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
