#ifndef ZIPFILES_H
#define ZIPFILES_H

#include <QObject>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <QStringList>

class ZipFiles : public QObject
{
    Q_OBJECT
public:
    //ZipFiles();
public:
    explicit ZipFiles();
    void createTestFile(const QString&);
    void zipTestFile(const QString& , const QString& );
    QString prepareTempLogFolder(const QStringList& );
    bool zipMultipleLogFiles(const QStringList& );
    bool zipLogFiles();
    QString prepareTempBatchFolder(const QStringList& );
    bool zipMultipleBatchFiles(const QStringList& );
    bool zipBatchFiles();

    // Start zipping files in background
    void startZip(const QStringList &files, const QString &zipFileName);
    void startLogZip();
    void startBatchZip();

signals:
    void zipLogFinished(bool success, const QString &zipFileName);
    void zipLogFinished(bool success);
    void zipBatchFinished(bool success);
    void zipProgress(int current, int total);

private slots:
    void onLogFinished();
    void onBatchFinished();

private:
    // Function executed in background
    bool doZip(const QStringList &files, const QString &zipFileName);

    QFutureWatcher<bool> logWatcher;
    QFutureWatcher<bool> batchWatcher;
//    QStringList pendingFiles;
//    QString targetZip;
};

#endif // ZIPFILES_H
