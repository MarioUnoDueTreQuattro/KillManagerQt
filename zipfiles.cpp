#include "zipfiles.h"
#include "SimpleZipper/src/SimpleZipper.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QMessageBox>
#include <QDateTime>

ZipFiles::ZipFiles(QObject *parent)
    : QObject(parent)
{
    connect(&watcher, SIGNAL(finished()), this, SLOT(onFinished()));
    // QString testFile = "KillManagerQttest.txt";
    // QString zipFile = "KillManagerQttest.zip";
    // createTestFile(testFile);
    // zipTestFile(testFile, zipFile);
    // bool bBackuped = zipBackups ();
}

bool ZipFiles::zipLogFiles()
{
    // prepare list of files to copy to "temp_zip_folder"
    QStringList nameFilters;
    nameFilters << "*.log";
    QString sAppPath = QCoreApplication::applicationDirPath();
    sAppPath = QDir::toNativeSeparators (sAppPath);
    QString backupDirName = sAppPath + "\\Log_Backup";
    qDebug() << "backupDirName" << backupDirName;
    QDir backupDir(backupDirName);
    if (!backupDir.exists())
    {
        backupDir.mkpath("."); // Crea la cartella se non esiste
    }
    QString filePath = backupDirName;
    QDir dir(filePath);
    QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int iFilesCount = files.count();
    qDebug() << "iFilesCount= " << QString::number(iFilesCount);
    //iFilesCount = files.size();
    //qDebug() << QString::number(iFilesCount);
    QString logFilePath;
    QStringList filePaths;
    for (const QFileInfo &fileInfo : files)
    {
        filePaths << fileInfo.absoluteFilePath(); // or fileInfo.filePath()
    }
    if (filePaths.count () > 0)
    {
        bool bBackuped = zipMultipleLogFiles (filePaths );
        if (bBackuped)
        {
            bool allDeleted = true;
            for (const QFileInfo& fileInfo : files)
            {
                if (fileInfo.isFile())
                {
                    QFile file(fileInfo.absoluteFilePath());
                    if (!file.remove())
                    {
                        qDebug() << "Errore durante l'eliminazione del file:" << fileInfo.absoluteFilePath();
                        allDeleted = false;
                    }
                    else
                    {
                        //qDebug() << "File eliminato con successo:" << fileInfo.absoluteFilePath();
                    }
                }
            }
            if (allDeleted == false)
                QMessageBox::critical( nullptr, "", "Critical error.\n" "Log files in temporary folder deletion unsuccessfull." );
        }
        else QMessageBox::critical( nullptr, "", "Critical error.\n" "Log files not backupep." );
    }
    else
    {
        //QMessageBox::information (this, "", "There aren't log files to be compressed." );
        qDebug() << "There aren't log files to be compressed.";
    }
    // for (int i = 0; i < iFilesCount; ++i)
    // {
    // logFilePath = files[i].absoluteFilePath();
    //        //qDebug() << "logFilePath" << logFilePath;
    // QFile file(logFilePath);
    // QString fileName = QFileInfo(file).fileName();
    // QString targetPath = backupDir.filePath(fileName);
    // if (!file.rename(targetPath))
    // {
    // qWarning() << "Failed to move file to:" << targetPath;
    // return false;
    // }
    // else qDebug() << fileName << "successfully moved";
    // }
    return true;
}

void ZipFiles::createTestFile(const QString& path)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << "Hello from SimpleZipper!";
        file.close();
    }
    else
    {
        qWarning() << "Failed to create test file.";
    }
}

void ZipFiles::zipTestFile(const QString& inputPath, const QString& outputPath)
{
    if (SimpleZipper::zipFile(inputPath, outputPath))
    {
        qDebug() << "File zipped successfully.";
    }
    else
    {
        qWarning() << "Zipping failed.";
    }
}

QString ZipFiles::prepareTempLogFolder(const QStringList& files)
{
    QString tempFolder = "temp_zip_folder/";
    QDir().mkpath(tempFolder);
    for (const QString& filePath : files)
    {
        QFileInfo fi(filePath);
        QString destPath = tempFolder + fi.fileName();
        QFile::copy(filePath, destPath);
    }
    return tempFolder;
}

bool ZipFiles::zipMultipleLogFiles(const QStringList& files)
{
    QString sAppPath = QCoreApplication::applicationDirPath();
    sAppPath = QDir::toNativeSeparators (sAppPath);
    QString backupDirName = sAppPath + "\\Log_Backup";
    QString zipFileName = backupDirName;
    zipFileName += "\\";
    zipFileName += QDateTime::currentDateTime().toString("'Old_Log_Files_Backup_'yyyy-MM-dd_hh-mm-ss'.zip'");
    qDebug() << "New Log FileName " << zipFileName;
    QString tempFolder = prepareTempLogFolder(files);
    bool success = SimpleZipper::zipFolder(tempFolder, zipFileName);
    QDir(tempFolder).removeRecursively(); // Clean up
    return success;
}

void ZipFiles::startZip(const QStringList &files, const QString &zipFileName)
{
    pendingFiles = files;
    targetZip = zipFileName;
    QFuture<bool> future = QtConcurrent::run(this, &ZipFiles::doZip, files, zipFileName);
    watcher.setFuture(future);
}

bool ZipFiles::doZip(const QStringList &files, const QString &zipFileName)
{
    // QuaZip zip(zipFileName);
    // if (!zip.open(QuaZip::mdCreate)) {
    // qWarning() << "Cannot open zip file for writing:" << zipFileName;
    // return false;
    // }
    // int total = files.size();
    // int current = 0;
    // foreach (QString filePath, files) {
    // QFile file(filePath);
    // if (!file.open(QIODevice::ReadOnly)) {
    // qWarning() << "Cannot read file:" << filePath;
    // continue;
    // }
    // QuaZipFile outFile(&zip);
    // QFileInfo fi(filePath);
    // if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fi.fileName(), filePath))) {
    // qWarning() << "Cannot add file to zip:" << fi.fileName();
    // continue;
    // }
    // outFile.write(file.readAll());
    // outFile.close();
    // file.close();
    // current++;
    // emit zipProgress(current, total); // WARNING: cross-thread emit
    // }
    // zip.close();
    return true;
}

void ZipFiles::onFinished()
{
    bool success = watcher.result();
    emit zipFinished(success, targetZip);
}
