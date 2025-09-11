#include "zipfiles.h"
#include "SimpleZipper/src/SimpleZipper.h"
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QDateTime>
#include "utility.h"
#include <QStandardPaths>
#include <QFileInfoList>
#include <cstdlib>

ZipFiles::ZipFiles()
{
    connect(&logWatcher, SIGNAL(finished()), this, SLOT(onLogFinished()));
    connect(&batchWatcher, SIGNAL(finished()), this, SLOT(onBatchFinished()));
    // QString testFile = "KillManagerQttest.txt";
    // QString zipFile = "KillManagerQttest.zip";
    // createTestFile(testFile);
    // zipTestFile(testFile, zipFile);
    // bool bBackuped = zipBackups ();
}

ZipFiles::~ZipFiles()
{
LOG_MSG("Destructor: object correctly destroyed.");
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

void ZipFiles::startLogZip()
{
    QFuture<bool> future = QtConcurrent::run(this, &ZipFiles::zipLogFiles);
    logWatcher.setFuture(future);
}

void ZipFiles::startZip(const QStringList &files, const QString &zipFileName)
{
    // pendingFiles = files;
    // targetZip = zipFileName;
    // QFuture<bool> future = QtConcurrent::run(this, &ZipFiles::doZip, files, zipFileName);
    // watcher.setFuture(future);
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

void ZipFiles::onLogFinished()
{
    qDebug() << __PRETTY_FUNCTION__;
    bool success = logWatcher.result();
    emit zipLogFinished(success);
    // emit zipFinished(success, targetZip);
    //this->deleteLater ();
}

void ZipFiles::onBatchFinished()
{
    qDebug() << __PRETTY_FUNCTION__;
    bool success = batchWatcher.result();
    emit zipBatchFinished(success);
    // emit zipFinished(success, targetZip);
    //this->deleteLater ();
}

bool ZipFiles::zipBatchFiles()
{
    QStringList filePaths;
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QSettings settings;
    QString sBackupPath = settings.value("Dialog/Backup path", documentsPath).toString();
    bool bDeleteOldBackups = settings.value("Dialog/DeleteOldBackups", true).toBool ();
    int iBackupsCount = settings.value("Dialog/BackupsCount", 100).toInt();
    int iBackupsDays = settings.value("Dialog/BackupsDays", 30).toInt();
    if (bDeleteOldBackups == false) return false;
    QStringList nameFilters;
    nameFilters << "*.bat";
    QDir dir(sBackupPath);
    QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int iFilesCount = files.count();
    if (iFilesCount < iBackupsCount)
    {
        LOG_MSG("iFilesCount < iBackupsCount = " + QString::number (iFilesCount));
        return false;
    }
    files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot); //, QDir::Time);
    std::sort(files.begin(), files.end(), ZipFiles::compareByLastModified);
    iFilesCount = files.size();
    int filesToDelete = iFilesCount - iBackupsCount;
    LOG_MSG("filesToDelete (if older than iBackupsDays) = " + QString::number (filesToDelete));
    // Delete the oldest files (those after the first files to retain)
    QDateTime backupDaysAgo = QDateTime::currentDateTime().addDays(-iBackupsDays);
    QDateTime lastModified;
    QString filePath;
    for (int i = iBackupsCount; i < iFilesCount; ++i)
    {
        filePath = files[i].absoluteFilePath();
        lastModified = files[i].lastModified ();
        if (lastModified < backupDaysAgo)
        {
            filePaths.append (filePath);
        }
        else
            LOG_MSG(filePath + " is newer than " + QString::number(iBackupsDays) + " days" );
    }
    if (filePaths.count () > 0)
    {
        bool bBackuped = zipMultipleBatchFiles (filePaths );
        if (bBackuped)
        {
            bool allDeleted = true;
            for (const QFileInfo& fileInfo : filePaths)
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
                QMessageBox::critical( nullptr, "", "Critical error.\n" "Batch files in temporary folder deletion unsuccessfull." );
        }
        else QMessageBox::critical( nullptr, "", "Critical error.\n" "Batch files not backupep." );
    }
    else
    {
        //QMessageBox::information (this, "", "There aren't log files to be compressed." );
        qDebug() << "There aren't Batch files to be compressed.";
    }
    return true;
}

QString ZipFiles::prepareTempBatchFolder(const QStringList& files)
{
    QString tempFolder = "temp_batch_folder/";
    QDir().mkpath(tempFolder);
    for (const QString& filePath : files)
    {
        QFileInfo fi(filePath);
        QString destPath = tempFolder + fi.fileName();
        QFile::copy(filePath, destPath);
    }
    return tempFolder;
}

bool ZipFiles::zipMultipleBatchFiles(const QStringList& files)
{
    QSettings settings;
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString sAppPath = settings.value("Dialog/Backup path", documentsPath).toString();
    sAppPath = QDir::toNativeSeparators (sAppPath);
    QString backupDirName = sAppPath;
    QString zipFileName = backupDirName;
    zipFileName += "\\";
    zipFileName += QDateTime::currentDateTime().toString("'Old_Batch_Files_Backup_'yyyy-MM-dd_hh-mm-ss'.zip'");
    qDebug() << "New Batch FileName " << zipFileName;
    QString tempFolder = prepareTempBatchFolder (files);
    bool success = SimpleZipper::zipFolder(tempFolder, zipFileName);
    QDir(tempFolder).removeRecursively(); // Clean up
    return success;
}

void ZipFiles::startBatchZip()
{
    QFuture<bool> future = QtConcurrent::run(this, &ZipFiles::zipBatchFiles);
    batchWatcher.setFuture(future);
}

void ZipFiles::deleteOldBackups()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QSettings settings;
    QString sBackupPath = settings.value("Dialog/Backup path", documentsPath).toString();
    bool bDeleteOldBackups = settings.value("Dialog/DeleteOldBackups", true).toBool ();
    int iBackupsCount = settings.value("Dialog/BackupsCount", 100).toInt();
    int iBackupsDays = settings.value("Dialog/BackupsDays", 30).toInt();
    if (bDeleteOldBackups == false) return;
    QStringList nameFilters;
    nameFilters << "*.bat";
    QDir dir(sBackupPath);
    QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int iFilesCount = files.count();
    if (iFilesCount < iBackupsCount)
    {
        LOG_MSG("iFilesCount < iBackupsCount = " + QString::number (iFilesCount));
        return;
    }
    files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot); //, QDir::Time);
    std::sort(files.begin(), files.end(), ZipFiles::compareByLastModified);
    iFilesCount = files.size();
    int filesToDelete = iFilesCount - iBackupsCount;
    LOG_MSG("filesToDelete (if older than iBackupsDays) = " + QString::number (filesToDelete));
    // Delete the oldest files (those after the first files to retain)
    QDateTime backupDaysAgo = QDateTime::currentDateTime().addDays(-iBackupsDays);
    QDateTime lastModified;
    QString filePath;
    for (int i = iBackupsCount; i < iFilesCount; ++i)
    {
        filePath = files[i].absoluteFilePath();
        lastModified = files[i].lastModified ();
        if (lastModified < backupDaysAgo)
        {
            if (QFile::remove(filePath))
            {
                qDebug() << "Deleted:" << filePath;
            }
            else
            {
                qDebug() << "Failed to delete:" << filePath;
            }
        }
        else
            LOG_MSG(filePath + " is newer than " + QString::number(iBackupsDays) + " days" );
    }
}

bool ZipFiles::compareByLastModified(const QFileInfo &a, const QFileInfo &b)
{
    return a.lastModified() > b.lastModified(); // Newest first
}
