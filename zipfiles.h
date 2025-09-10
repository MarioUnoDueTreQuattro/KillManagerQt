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
  // old zip functions
    void createTestFile(const QString&);
    void zipTestFile(const QString& , const QString& );
    QString prepareTempLogFolder(const QStringList& );
    bool zipMultipleLogFiles(const QStringList& );
    bool zipLogFiles();

    // Start zipping files in background
    void startZip(const QStringList &files, const QString &zipFileName);
    void startLogZip();

signals:
    void zipFinished(bool success, const QString &zipFileName);
    void zipFinished(bool success);
    void zipProgress(int current, int total);

private slots:
    void onFinished();

private:
    // Function executed in background
    bool doZip(const QStringList &files, const QString &zipFileName);

    QFutureWatcher<bool> watcher;
//    QStringList pendingFiles;
//    QString targetZip;
};

#endif // ZIPFILES_H
