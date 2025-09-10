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
    ZipFiles();
public:
    explicit ZipFiles(QObject *parent = nullptr);
  // old zip functions
    void createTestFile(const QString&);
    void zipTestFile(const QString& , const QString& );
    QString prepareTempFolder(const QStringList& );
    bool zipMultipleFiles(const QStringList& );
    bool zipBackups();

    // Start zipping files in background
    void startZip(const QStringList &files, const QString &zipFileName);

signals:
    void zipFinished(bool success, const QString &zipFileName);
    void zipProgress(int current, int total);

private slots:
    void onFinished();

private:
    // Function executed in background
    bool doZip(const QStringList &files, const QString &zipFileName);

    QFutureWatcher<bool> watcher;
    QStringList pendingFiles;
    QString targetZip;
};

#endif // ZIPFILES_H
