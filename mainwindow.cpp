#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>
#include <cstdlib>  // for system()
//#include "SimpleZipper/src/SimpleZipper.h"

//void createTestFile(const QString& path) {
// QFile file(path);
// if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
// QTextStream out(&file);
// out << "Hello from SimpleZipper!";
// file.close();
// } else {
// qWarning() << "Failed to create test file.";
// }
//}

//void zipTestFile(const QString& inputPath, const QString& outputPath) {
// if (SimpleZipper::zipFile(inputPath, outputPath)) {
// qDebug() << "File zipped successfully.";
// } else {
// qWarning() << "Zipping failed.";
// }
//}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    if (!m_ProcessListEx.enableDebugPrivileges())
    {
        LOG_MSG("Failed to enable debug privileges. Access to some processes might be denied.");
    }
    // QString testFile = "test.txt";
    // QString zipFile = "test.zip";
    // createTestFile(testFile);
    // zipTestFile(testFile, zipFile);
    //unzipTestFile(zipFile, unzipFolder);
    qApp->setStyleSheet( "QStatusBar::item { border: 0px}" ) ;
    this->setWindowIcon(QIcon(":/icons/img/KillManager.png"));       // Use the path defined in .qrc
    m_ProcessListEx.clear();
    ui->setupUi(this);
    m_statusBarMovie = new QMovie(":/icons/img/AGau.gif", QByteArray(), this);
    m_statusBarMovie->setScaledSize (QSize(16, 16));
    m_statusBarMovie->setCacheMode(QMovie::CacheAll);
    m_statusBarMovie->start();
    m_statusBarMovie->setPaused(true); // Fermiamo l'animazione automatica
    //connect(m_statusBarMovie, SIGNAL(frameChanged(int)), this, SLOT(onFrameChanged(int)));
    m_StatusBarLabel = new QLabel("", this); // Il secondo parametro indica il genitore
    m_StatusBarLabel->setMovie (m_statusBarMovie);
    ui->statusBar->addPermanentWidget (m_StatusBarLabel);
    // m_StatusBarLabel->setFrameShape (QFrame::NoFrame);
    // m_StatusBarLabel->setFrameStyle (QFrame::Plain);
    //m_StatusBarLabel->setFixedSize(QSize(32,32));
    //m_statusBarMovie->setParent (m_StatusBarLabel);
    ui->pushButtonReload->setVisible (false);
    ui->pushButtonWrite->setVisible (false);
    m_iTimerUpdatesCount = 0;
    m_bTimerIsConnected = false;
    m_Font = ui->labelEnabled->font ();
    m_Font.setBold(true);
    QColor myColor(0, 0, 128);
    //QColor myColor(128, 0, 0);
    //QColor customColor(100, 200, 50);
    //myColor.setHsl(240,100,25,64);
    m_TextBrush = QBrush (myColor);
    m_TextBrush.setColor (myColor);
    // m_TextBrush = QBrush (myColor,Qt::SolidPattern);
    // m_TextBrush.setColor (myColor);
    QColor myServiceColor(128, 0, 0);
    m_ServiceTextBrush = QBrush (myServiceColor);
    m_ServiceTextBrush.setColor (myServiceColor);
    ui->labelKilled->setText("Killed: 0" );
    readSettings();
    //setWindowTitle("Qt " +qtVersion + " Version"+ APP_VERSION);
    this->setWindowTitle(qApp->applicationName());       // + " Version "+ APP_VERSION);
    // QApplication* currentApp = qApp;
    // m_sAppCompany = currentApp->organizationName();
    // m_sAppName = currentApp->applicationName();
    // m_sKillFile = getKillFilePath();
    // QString universalPath1 = QDir::fromNativeSeparators(m_sKillFile);
    loadListFromFile();
    ui->listWidgetEnabled->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidgetDisabled->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetEnabled, &MyQListWidget::reordering, this, &MainWindow::disconnectTimer);
    connect(ui->listWidgetEnabled, &MyQListWidget::itemsReordered, this, &MainWindow::writeListToFile);
    connect(ui->listWidgetDisabled, &MyQListWidget::reordering, this, &MainWindow::disconnectTimer);
    connect(ui->listWidgetDisabled, &MyQListWidget::itemsReordered, this, &MainWindow::writeListToFile);
    connect(ui->listWidgetEnabled, &QListWidget::customContextMenuRequested,
        this, &MainWindow::showListWidgetEnabledContextMenu);
    connect(ui->listWidgetDisabled, &QListWidget::customContextMenuRequested,
        this, &MainWindow::showListWidgetDisabledContextMenu);
    //connect(ui->menuConfigure, SIGNAL(triggered(QAction*)), this, SLOT(menuConfigure()));
    //connect(ui->actionConfigure_app, SIGNAL(triggered(QAction*)), this, SLOT(menuConfigure()));
    timer = new QTimer(this);
    connectTimer ();
    timer->start(m_iRefreshRate);
    // Connect the signal from the scheduler to our slot in MainApp.
    connect(&m_scheduler, &Scheduler::logCompressionExecuted, this, &MainWindow::onLogCompressionExecuted); //&MainWindow::handleProgramExecuted);
    connect(&m_scheduler, &Scheduler::logCompressionChecked, this, &MainWindow::onLogCompressionChecked); //&MainWindow::handleProgramExecuted);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    QMainWindow::closeEvent(event);       // Call the base class's closeEvent
}

void MainWindow::showEvent(QShowEvent *event)
{
    if (m_sKillFile == "")
    {
        QTimer::singleShot(200, this, SLOT(firstTimeConfiguration()) );
    }
    QMainWindow::showEvent(event);       // Call the base class's showEvent
}

void MainWindow::onLogCompressionExecuted()
{
    //LOG_MSG("");
    ui->statusBar->showMessage ("Old log files compressed in ZIP format", 30000);
}

void MainWindow::onLogCompressionChecked()
{
    //LOG_MSG("");
    ui->statusBar->showMessage ("Checked if it's time to compress old log files.", 10000);
}

void MainWindow::firstTimeConfiguration()
{
    QMessageBox::information(this, "", "The application is not configured.\n\nPlease create or select a batch file in the configuration dialog.");
    menuConfigure ();
}

void MainWindow::timerUpdate()
{
    //m_statusBarMovie->start();
    //m_statusBarMovie->setPaused(true); // Fermiamo l'animazione automatica
    int frameCount = m_statusBarMovie->frameCount();
    if (frameCount > 0)
    {
        int frameIndex = (1 * (frameCount - 1)) / 100;
        m_statusBarMovie->jumpToFrame(frameIndex);
    }
    // m_StatusBarLabel->setFixedSize(m_statusBarMovie->currentImage().size());
    m_iTimerUpdatesCount += 1;
    int iModulus = m_iTimerUpdatesCount % 10 ;
    QString repeatedChar;
    repeatedChar = QString(".").repeated(iModulus );
    // repeatedChar.prepend ("<html><head/><body><span style=\" font-weight:600; color:#ff0000;\">");
    // repeatedChar.append ("</span></body></html>");
    //qDebug() << __FUNCTION__ << m_iTimerUpdatesCount;
    loadListFromFile ();
    QString sRefreshed = ui->statusBar->currentMessage ();
    if ((sRefreshed == "") | (sRefreshed.left (9) == "Refreshed"))
    {
        ui->statusBar->showMessage ("Refreshed every " + QString::number (m_iRefreshRate / 1000) + " seconds" + repeatedChar);
    }
    int progress = iModulus * 10;
    //progress=double(double(iModulus)/double(frameCount) *100);
    //LOG_VAR(progress);
    int frameIndex = (progress * m_statusBarMovie->frameCount()) / 100;
    frameIndex = qMin(frameIndex, m_statusBarMovie->frameCount() - 1);
    //LOG_VAR(frameIndex);
    m_statusBarMovie->jumpToFrame(frameIndex);
    m_StatusBarLabel->setToolTip ("Updates count: " + QString::number(m_iTimerUpdatesCount) + "\nProgress: " + QString::number(progress) + "%");
    //m_ProcessList.debugProcessesMemory ();
}

void MainWindow::updateSettings()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    documentsPath = QDir::toNativeSeparators (documentsPath);
    QSettings settings;
    m_sLogFileName = settings.value("LogFileName", "").toString();
    QString sKillFile = settings.value("Path").toString();
    //qDebug() << "Read string from registry:" << sKillFile;
    if (sKillFile == "")
    {
        //sKillFile = "C:\\Users\\Andrea\\Documents\\kill.bat";
        //qDebug() << "Read value is empty.";
    }
    m_sKillFile = sKillFile;
    m_sBackupPath = settings.value("Dialog/Backup path", documentsPath).toString();
    m_bKillInternal = settings.value("Dialog/UseInternalKill", false).toBool ();
    // else {
    //        //m_sKillFile = "C:\\Users\\Andrea\\Documents\\kill.bat";
    // qDebug() << "Read value OK";
    // }
    //return sKillFile;
}

compareByLastModified(const QFileInfo &a, const QFileInfo &b)
{
    return a.lastModified() > b.lastModified(); // Newest first
}

void MainWindow::deleteOldBackups()
{
    QSettings settings;
    bool bDeleteOldBackups = settings.value("Dialog/DeleteOldBackups", true).toBool ();
    int iBackupsCount = settings.value("Dialog/BackupsCount", 100).toInt();
    int iBackupsDays = settings.value("Dialog/BackupsDays", 30).toInt();
    if (bDeleteOldBackups == false) return;
    QStringList nameFilters;
    nameFilters << "*.bat";
    QDir dir(m_sBackupPath);
    QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int iFilesCount = files.count();
    if (iFilesCount < iBackupsCount)
    {
        LOG_MSG("iFilesCount < iBackupsCount = " + QString::number (iFilesCount));
        return;
    }
    files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot); //, QDir::Time);
    std::sort(files.begin(), files.end(), compareByLastModified);
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

void MainWindow::readSettings()
{
    QSettings settings;       // QSettings will use the organization and application names set in main()
    // Restore window geometry (size and position) and state (maximized, fullscreen, etc.)
    // Use QWidget::saveGeometry() and QWidget::restoreGeometry() for this.
    // QMainWindow also has saveState() and restoreState() for dock widgets, toolbars, etc.
    restoreGeometry(settings.value("MainWindow/geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("MainWindow/state", saveState()).toByteArray());
    // You can also save and restore individual pos() and size() if you prefer,
    // but saveGeometry() is generally more robust as it handles window frames
    // and maximized/fullscreen states.
    // Example for individual pos and size:
    // move(settings.value("MainWindow/pos", QPoint(50, 50)).toPoint());
    // resize(settings.value("MainWindow/size", QSize(800, 600)).toSize());
    // Handle maximized state explicitly if using pos/size, otherwise restoreGeometry handles it.
    // if (settings.value("MainWindow/maximized", false).toBool()) {
    // showMaximized();
    // }
    QByteArray savedState = settings.value("MainWindow/splitterState").toByteArray();
    if (!savedState.isEmpty())
    {
        // La funzione restoreState() di QSplitter prende un QByteArray
        // e ripristina la posizione dei divisori.
        ui->splitter->restoreState(savedState);
        //qDebug() << "Splitter state loaded.";
    }
    else
    {
        qDebug() << "No splitter state found, using default.";
        // Se non c'è uno stato salvato, puoi impostare le dimensioni iniziali
        // in modo esplicito se non ti piacciono quelle predefinite di Qt.
        // Ad esempio: splitter->setSizes(QList<int>() << 200 << 400);
    }
    int iRefreshRate = settings.value("Dialog/RefreshRate", 5).toInt(); // default to 5
    m_iRefreshRate = iRefreshRate * 1000;
}

void MainWindow::writeSettings()
{
    QSettings settings;
    // Save window geometry and state
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/state", saveState());
    settings.setValue("MainWindow/splitterState", ui->splitter->saveState());
    // Example for individual pos and size (if not using saveGeometry)
    // if (!isMaximized() && !isFullScreen()) { // Only save if not maximized or fullscreen
    // settings.setValue("MainWindow/pos", pos());
    // settings.setValue("MainWindow/size", size());
    // }
    // settings.setValue("MainWindow/maximized", isMaximized());
}

void MainWindow::showListWidgetEnabledContextMenu(const QPoint& pos)
{
    // Get the item at the clicked position
    QListWidgetItem* clickedItem = ui->listWidgetEnabled->itemAt(pos);
    bool bIsRunning;
    bIsRunning = m_ProcessListEx.isRunning (clickedItem->text ());
    // Create the menu
    QMenu contextMenu(tr("Context Menu"), this);
    QAction* disableAction = contextMenu.addAction(tr("Disable"));
    QAction* deleteAction = contextMenu.addAction(tr("Delete"));
    //QAction* enableAction = contextMenu.addAction(tr("Enable"));
    QAction* copyAction = contextMenu.addAction(tr("Copy Text"));
    QAction* killAction = contextMenu.addAction(tr("Terminate"));
    killAction->setIcon(QIcon(":/icons/img/kill.png"));
    deleteAction->setIcon(QIcon(":/icons/img/icons8-delete-48.png"));
    //enableAction->setIcon(QIcon(":/icons/img/icons8-left-48.png"));
    disableAction->setIcon(QIcon(":/icons/img/icons8-right-48.png"));
    copyAction->setIcon(QIcon(":/icons/img/icons8-copy-to-clipboard-48.png"));
    // contextMenu.addSeparator(); // Add a separator line
    // QAction *globalAction = contextMenu.addAction(tr("Global Action")); // Always enabled
    // Connect actions to their slots
    // connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedItem);
    connect(disableAction, &QAction::triggered, this, &MainWindow::disableSelectedEnabledItem);
    connect(copyAction, &QAction::triggered, this, &MainWindow::copySelectedEnabledItem);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedEnabledItem);
    connect(killAction, &QAction::triggered, this, &MainWindow::killSelectedEnabledItem);
    if (! bIsRunning) killAction->setVisible (false);
    // You could connect globalAction to another slot if needed
    // Enable/disable actions based on whether an item was clicked
    bool itemClicked = (clickedItem != NULL);
    deleteAction->setEnabled(itemClicked);
    //enableAction->setEnabled(false);
    disableAction->setEnabled(itemClicked);
    copyAction->setEnabled(itemClicked);
    // Show the menu at the global position of the mouse click
    // mapToGlobal converts the local widget coordinate (pos) to a global screen coordinate
    disconnectTimer ();
    contextMenu.exec(ui->listWidgetEnabled->mapToGlobal(pos));
    connectTimer ();
}

void MainWindow::showListWidgetDisabledContextMenu(const QPoint& pos)
{
    // Get the item at the clicked position
    QListWidgetItem* clickedItem = ui->listWidgetDisabled->itemAt(pos);
    bool bIsRunning;
    bIsRunning = m_ProcessListEx.isRunning (clickedItem->text ());
    // Create the menu
    QMenu contextMenu(tr("Context Menu"), this);
    QAction* enableAction = contextMenu.addAction(tr("Enable"));
    QAction* deleteAction = contextMenu.addAction(tr("Delete"));
    //QAction* disableAction = contextMenu.addAction(tr("Disable"));
    QAction* copyAction = contextMenu.addAction(tr("Copy Text"));
    QAction* killAction = contextMenu.addAction(tr("Terminate"));
    killAction->setIcon(QIcon(":/icons/img/kill.png"));
    deleteAction->setIcon(QIcon(":/icons/img/icons8-delete-48.png"));
    enableAction->setIcon(QIcon(":/icons/img/icons8-left-48.png"));
    //disableAction->setIcon(QIcon(":/icons/img/icons8-right-48.png"));
    copyAction->setIcon(QIcon(":/icons/img/icons8-copy-to-clipboard-48.png"));
    // contextMenu.addSeparator(); // Add a separator line
    // QAction *globalAction = contextMenu.addAction(tr("Global Action")); // Always enabled
    // Connect actions to their slots
    // connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedItem);
    connect(enableAction, &QAction::triggered, this, &MainWindow::enableSelectedDisabledItem);
    connect(copyAction, &QAction::triggered, this, &MainWindow::copySelectedDisabledItem);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedDisabledItem);
    connect(killAction, &QAction::triggered, this, &MainWindow::killSelectedDisabledItem);
    if (! bIsRunning) killAction->setVisible (false);
    // You could connect globalAction to another slot if needed
    // Enable/disable actions based on whether an item was clicked
    bool itemClicked = (clickedItem != NULL);
    deleteAction->setEnabled(itemClicked);
    enableAction->setEnabled(itemClicked);
    //disableAction->setEnabled(false);
    copyAction->setEnabled(itemClicked);
    // Show the menu at the global position of the mouse click
    // mapToGlobal converts the local widget coordinate (pos) to a global screen coordinate
    disconnectTimer ();
    contextMenu.exec(ui->listWidgetDisabled->mapToGlobal(pos));
    connectTimer ();
}

void MainWindow::killSelectedDisabledItem()
{
    bool bKilled = false;
    QListWidgetItem* currentItem = ui->listWidgetDisabled->currentItem();
    if (currentItem)
    {
        bKilled = m_ProcessListEx.killProcessAndChildsByNameEx (currentItem->text());
        ui->statusBar->showMessage("Terminated: " + currentItem->text(), 10000);
    }
    else
    {
        qDebug() << "No item selected";
    }
}

void MainWindow::killSelectedEnabledItem()
{
    bool bKilled = false;
    QListWidgetItem* currentItem = ui->listWidgetEnabled->currentItem();
    if (currentItem)
    {
        bKilled = m_ProcessListEx.killProcessAndChildsByNameEx (currentItem->text());
        ui->statusBar->showMessage("Terminated: " + currentItem->text(), 10000);
    }
    else
    {
        qDebug() << "No item selected";
    }
}

void MainWindow::copySelectedDisabledItem()
{
    QListWidgetItem* currentItem = ui->listWidgetDisabled->currentItem();
    if (currentItem)
    {
        QApplication::clipboard()->setText(currentItem->text());
        ui->statusBar->showMessage("Copied text: " + currentItem->text(), 10000);
    }
    else
    {
        qDebug() << "No item selected for copying.";
    }
}

void MainWindow::copySelectedEnabledItem()
{
    QListWidgetItem* currentItem = ui->listWidgetEnabled->currentItem();
    if (currentItem)
    {
        QApplication::clipboard()->setText(currentItem->text());
        ui->statusBar->showMessage("Copied text: " + currentItem->text(), 10000);
    }
    else
    {
        qDebug() << "No item selected for copying.";
    }
}

void MainWindow::deleteSelectedEnabledItem()
{
    QListWidgetItem* currentItem = ui->listWidgetEnabled->currentItem();
    if (currentItem)
    {
        disconnectTimer ();
        ui->listWidgetEnabled->removeItemWidget(currentItem);
        ui->statusBar->showMessage("Removed: " + currentItem->text(), 10000);
        m_ProcessListEx.deleteApplicationItem(currentItem->text());
        LOG_MSG( "m_ApplicationItemsList.size= " + m_ProcessListEx.size());
        delete currentItem;
        ui->labelEnabled->setText("Enabled: " + QString::number(ui->listWidgetEnabled->count()));
        ui->labelDisabled->setText("Disabled: " + QString::number(ui->listWidgetDisabled->count()));
        LOG_MSG( "deleteSelectedEnabledItem");
        writeListToFile ();
        connectTimer ();
    }
    else
    {
        LOG_MSG( "No item selected for deleting.");
    }
}

void MainWindow::deleteSelectedDisabledItem()
{
    QListWidgetItem* currentItem = ui->listWidgetDisabled->currentItem();
    if (currentItem)
    {
        disconnectTimer ();
        ui->listWidgetDisabled->removeItemWidget(currentItem);
        ui->statusBar->showMessage("Removed: " + currentItem->text(), 10000);
        m_ProcessListEx.deleteApplicationItem(currentItem->text());
        LOG_MSG( "m_ApplicationItemsList.size= " + m_ProcessListEx.size());
        delete currentItem;
        ui->labelEnabled->setText("Enabled: " + QString::number(ui->listWidgetEnabled->count()));
        ui->labelDisabled->setText("Disabled: " + QString::number(ui->listWidgetDisabled->count()));
        LOG_MSG("deleteSelectedDisabledItem");
        writeListToFile ();
        connectTimer ();
    }
    else
    {
        LOG_MSG( "No item selected for deleting.");
    }
}

void MainWindow::enableSelectedDisabledItem()
{
    QListWidgetItem* currentItem = ui->listWidgetDisabled->currentItem();
    if (currentItem)
    {
        disconnectTimer ();
        QListWidgetItem* temp = new QListWidgetItem(*currentItem);
        ui->listWidgetEnabled->addItem(temp);
        ui->listWidgetDisabled->removeItemWidget(currentItem);
        ui->statusBar->showMessage("Enabled: " + currentItem->text(), 10000);
        m_ProcessListEx.moveApplicationItem(currentItem->text(), true);
        delete currentItem;
        ui->labelEnabled->setText("Enabled: " + QString::number(ui->listWidgetEnabled->count()));
        ui->labelDisabled->setText("Disabled: " + QString::number(ui->listWidgetDisabled->count()));
        qDebug() << "disableSelectedEnabledItem";
        writeListToFile ();
        connectTimer ();
    }
    else
    {
        qDebug() << "No item selected for enabling.";
    }
}

void MainWindow::disableSelectedEnabledItem()
{
    QListWidgetItem* currentItem = ui->listWidgetEnabled->currentItem();
    if (currentItem)
    {
        disconnectTimer ();
        QListWidgetItem* temp = new QListWidgetItem(*currentItem);
        ui->listWidgetDisabled->addItem(temp);
        ui->listWidgetEnabled->removeItemWidget(currentItem);
        ui->statusBar->showMessage("Disabled: " + currentItem->text(), 10000);
        m_ProcessListEx.moveApplicationItem(currentItem->text(), false);
        delete currentItem;
        ui->labelEnabled->setText("Enabled: " + QString::number(ui->listWidgetEnabled->count()));
        ui->labelDisabled->setText("Disabled: " + QString::number(ui->listWidgetDisabled->count()));
        qDebug() << "disableSelectedEnabledItem";
        writeListToFile ();
        connectTimer ();
    }
    else
    {
        qDebug() << "No item selected for disabling.";
    }
}

void MainWindow::menuConfigure()
{
    LOG_MSG("menuConfigure");
    Dialog cd;       //=new configureDialog(this);
    connect(&cd, SIGNAL(accepted()), this, SLOT(loadListFromFile()));
    // cd.setParent (this);
    cd.setWindowTitle("Configure");
    cd.exec();
    //qDebug() << __FUNCTION__ << "Reading m_iRefreshRate";
    QSettings settings;
    int iRefreshRate = settings.value("Dialog/RefreshRate", 5).toInt(); // default to 5
    m_iRefreshRate = iRefreshRate * 1000;
    timer->setInterval (m_iRefreshRate);
    deleteOldBackups ();
    // cd.open ();
    // cd.update ();
    //cd.setVisible (true);
    //cd.setFocus ();
    // configureDialog *myDialog = new configureDialog();
    // setCentralWidget(myDialog);
    // myDialog->show ();
}

void MainWindow::showAddExeDialog()
{
    LOG_MSG("showAddExeDialog");
    AddExeDialog addExe;       //=new configureDialog(this);
    //connect(&addExe, SIGNAL(accepted()), this, SLOT(loadListFromFile()));
    addExe.setWindowTitle("Add executable");
    addExe.exec();
    int result = addExe.result();
    if (result == QDialog::Accepted)
    {
        QString receivedText = addExe.getText();           // Retrieve the text
        if (receivedText != "")
        {
            disconnectTimer ();
            addItemToListwidget(ui->listWidgetEnabled, receivedText);
            bool bFound = false;
            int iFoundItem = m_ProcessListEx.findApplicationItemIndex (receivedText);
            if (iFoundItem != -1)
            {
                bFound = true;
                // qDebug() << "m_ApplicationItemsList.size=" << m_ApplicationItemsList.size();
            }
            // QString itemString = "";
            //            // Iterate in reverse to safely remove items while modifying the list
            //            //qDebug()<< "m_ApplicationItemsList.size="<<m_ApplicationItemsList.size();
            // for (int i = m_ApplicationItemsList.size() - 1; i >= 0; --i)
            // {
            // if (m_ApplicationItemsList.at(i).getAppName() == receivedText)
            // {
            // bFound = true;
            // }
            // }
            if (!bFound)
            {
                ProcessItem newAppItem(receivedText, true);
                m_ProcessListEx.append(newAppItem);
                LOG_MSG("m_ApplicationItemsList ADDED " + receivedText);
            }
            LOG_MSG("m_ApplicationItemsList.size= " + m_ProcessListEx.size());
            //QListWidgetItem* newitem = new QListWidgetItem(receivedText, ui->listWidgetEnabled);
            //ui->listWidgetEnabled->addItem(newitem);
            LOG_MSG("Dialog accepted. Received: " + receivedText);
            writeListToFile ();
            connectTimer ();
        }
        else
            LOG_MSG("Dialog accepted but EMPTY");
    }
    else
    {
        LOG_MSG("Dialog rejected.");
    }
    connectTimer ();
}

void MainWindow::loadListFromFile(const QString& fileName)
{
    if (fileName == "")
    {
        qDebug() << __PRETTY_FUNCTION__ << "fileName==\"\"";
        return;
    }
    m_ProcessListEx.clear();
    m_ProcessListEx.populateProcessList ();
    QListWidgetItem* selectedEnabledItem;
    QListWidgetItem* selectedDisabledItem;
    m_sSelectedEnabledItem = "";
    m_sSelectedDisabledItem = "";
    bool bSelectedEnabledIsEmpty = ui->listWidgetEnabled->selectedItems ().isEmpty ();
    bool bSelectedDisabledIsEmpty = ui->listWidgetDisabled->selectedItems ().isEmpty ();
    // if (bSelectedEnabledIsEmpty) qDebug() << "bSelectedEnabledIsEmpty isEmpty";
    // if (bSelectedDisabledIsEmpty) qDebug() << "bSelectedDisabledIsEmpty isEmpty";
    if (bSelectedEnabledIsEmpty == false) selectedEnabledItem = ui->listWidgetEnabled->selectedItems ().first ();
    if (bSelectedDisabledIsEmpty == false) selectedDisabledItem = ui->listWidgetDisabled->selectedItems ().first ();
    if (selectedEnabledItem != NULL)
        if ( ui->listWidgetEnabled->selectedItems ().isEmpty () == false)
        {
            m_sSelectedEnabledItem = selectedEnabledItem->text ();
            //qDebug() << "Selected: " << sSelectedEnabledItem;
        }
    if (selectedDisabledItem != NULL)
        if ( ui->listWidgetDisabled->selectedItems ().isEmpty () == false)
        {
            m_sSelectedDisabledItem = selectedDisabledItem->text ();
            //qDebug() << "Disabled Selected: " << sSelectedDisabledItem;
        }
    //if (selectedDisabledItem!=NULL) selectedDisabledItem->setSelected (true);
    ui->listWidgetEnabled->clear();
    ui->listWidgetDisabled->clear();
    QFile file(fileName);
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    // {
    // qWarning() << "Could not open file:" << fileName;
    //        //return;
    // }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File not found or unreadable. Creating it...";
        // Try opening for writing to create the file
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << "";  // Write empty content or initial data
            file.close();
        }
        else
        {
            qWarning() << "Failed to create file:" << file.errorString();
            return;
        }
        // Reopen for reading
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning() << "Failed to reopen file for reading.";
            return;
        }
    }
    // QTextStream inCount(&file);
    // int lineCount = 0;
    // while (!inCount.atEnd())
    // {
    // inCount.readLine();
    // lineCount++;
    // }
    // file.close();
    // file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString file_line;
    QString firstThreeChars;
    QStringList line_parts;
    QString line_parts_last;
    QTextStream in(&file);
    //int progress=0;
    //int iLine=0;
    while (!in.atEnd())
    {
        // QApplication::processEvents(); // Per mantenere l'UI reattiva
        // iLine++;
        // progress=double(double(iLine)/double(lineCount) *100);
        //        //LOG_VAR(progress);
        // int frameIndex = (progress * m_statusBarMovie->frameCount()) / 100;
        // frameIndex = qMin(frameIndex, m_statusBarMovie->frameCount() - 1);
        //        //LOG_VAR(frameIndex);
        // m_statusBarMovie->jumpToFrame(frameIndex);
        file_line = in.readLine();
        // String with more than 3 characters
        firstThreeChars = file_line.left(3);           // Get the first 3 characters
        if (firstThreeChars.toUpper() == "REM")
        {
            //qDebug() << "REM:" << file_line;
            line_parts = file_line.split(' ');
            line_parts_last = line_parts.last();
            addItemToListwidget(ui->listWidgetDisabled, line_parts_last);
            //ui->listWidgetDisabled->addItem(line_parts_last);
            ProcessItem appItem(line_parts_last, false);
            m_ProcessListEx.append(appItem);
        }
        else
        {
            line_parts = file_line.split(' ');
            line_parts_last = line_parts.last();
            addItemToListwidget(ui->listWidgetEnabled, line_parts_last);
            //ui->listWidgetEnabled->addItem(line_parts_last);
            ProcessItem appItem(line_parts_last, true);
            m_ProcessListEx.append(appItem);
        }
        // You can also create a QListWidgetItem and add it
        // QListWidgetItem *item = new QListWidgetItem(line);
        // ui->listWidget->addItem(item);
    }
    //ui->statusBar->showMessage("File " + m_sKillFile + " read");
    //qDebug() << "ItemCount= " << m_ApplicationItemsList.count();
    ui->labelEnabled->setText("Enabled: " + QString::number(ui->listWidgetEnabled->count()));
    ui->labelDisabled->setText("Disabled: " + QString::number(ui->listWidgetDisabled->count()));
    file.close();       // Not strictly necessary due to RAII, but good practice
    //qDebug() << "m_ApplicationItemsList.size=" << m_ApplicationItemsList.size();
}
void MainWindow::loadListFromFile()
{
    updateSettings();
    // m_sKillFile.replace("\\", "\\\\");
    // qDebug() << m_sKillFile;
    QString universalPath1 = QDir::fromNativeSeparators(m_sKillFile);
    loadListFromFile(universalPath1);
    //qDebug() << "loadListFromFile() finished.";
}

bool MainWindow::listContainsItemText(MyQListWidget* listWidget, const QString& text)
{
    for (int i = 0; i < listWidget->count(); ++i)
    {
        QListWidgetItem* item = listWidget->item(i);
        if (item && item->text() == text)
        {
            return true;
        }
    }
    return false;
}

bool MainWindow::writeListToFile()
{
    disconnectTimer ();
    bool bBackuped = backupBatchFile();
    if (bBackuped == false)
    {
        LOG_MSG("NOT BACKUPED");
        return false;
    }
    else
        LOG_MSG( "BACKUP OK");
    //updatePaths();
    // m_sKillFile.replace("\\", "\\\\");
    // qDebug() << m_sKillFile;
    QString universalPath1 = QDir::fromNativeSeparators(m_sKillFile);
    // 1. Define the file path
    // It's good practice to use QDir for platform-independent paths.
    // Here, we'll save the file in the user's desktop directory.
    // QString filePath = QDir::homePath() + QDir::separator() + "MyTestFile.bat";
    //QString filePath = universalPath1;
    // Or a fixed path for testing (adjust for your OS):
    // QString filePath = "C:/Temp/MyTestFile.txt"; // Windows
    // QString filePath = "/tmp/MyTestFile.txt";   // Linux/macOS
    // 2. Create a QFile object
    QFile file(universalPath1);
    // 3. Open the file in WriteOnly mode (and optionally Text mode for QTextStream)
    // QFile::WriteOnly: The file is opened for writing. If it exists, its content is truncated.
    // QFile::Append: If you want to add to an existing file instead of overwriting it.
    // QFile::Text: (Optional but recommended for text files) Automatically handles end-of-line conversions (\n to \r\n on Windows).
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical() << "Could not open file for writing:" << file.errorString();
        return false;           // Indicate error
    }
    // 4. Create a QTextStream object for writing text
    QTextStream out(&file);
    // Optional: Set encoding for QTextStream (defaults to system locale encoding)
    // out.setCodec("UTF-8"); // Explicitly set UTF-8 encoding
    // 5. Write content to the file
    QString s_TaskKill = "taskkill /F /T /IM ";
    QString s_NoTaskKill = "REM taskkill /F /T /IM ";
    int iCount;
    iCount = ui->listWidgetEnabled->count();
    QString sItemText;
    for (int i = 0; i < iCount; i++)
    {
        sItemText = ui->listWidgetEnabled->item(i)->text();
        out << s_TaskKill << sItemText << "\n";
    }
    iCount = ui->listWidgetDisabled->count();
    for (int i = 0; i < iCount; i++)
    {
        sItemText = ui->listWidgetDisabled->item(i)->text();
        out << s_NoTaskKill << sItemText << "\n";
    }
    // out << QString("The current application path is: %1\n").arg(QCoreApplication::applicationDirPath());
    // 6. Close the file
    // The QTemporaryFile will be automatically removed when it goes out of scope or QApplication exits.
    // For regular QFile, it's good practice to close it explicitly, although it will be
    // automatically closed when the QFile object is destroyed.
    file.close();
    universalPath1 = QDir::toNativeSeparators(universalPath1);
    LOG_MSG("Batch file successfully written to: " + universalPath1);
    ui->statusBar->showMessage("Batch file successfully written to: " + universalPath1, 10000);
    connectTimer ();
    return true;
}

void MainWindow::on_listWidgetEnabled_itemClicked(QListWidgetItem * item)
{
    if (item)         // Always check if the item pointer is valid
    {
        ui->listWidgetEnabled->setCurrentItem(item);
        // QString clickedText = item->text();
        // ui->statusBar->showMessage(clickedText);
    }
}
void MainWindow::on_listWidgetDisabled_itemClicked(QListWidgetItem * item)
{
    if (item)         // Always check if the item pointer is valid
    {
        ui->listWidgetDisabled->setCurrentItem(item);
        // QString clickedText = item->text();
        // ui->statusBar->showMessage(clickedText);
    }
}
void MainWindow::on_pushButtonWrite_clicked()
{
    writeListToFile ();
    return;
    QMessageBox::StandardButton reply;
    // Use the static question() method for a confirmation dialog
    reply = QMessageBox::question(this,
            "Confirm",       // Dialog title
            "Are you sure you want to proceed writing the file to disk?",       // Main text
            QMessageBox::Yes | QMessageBox::Cancel);       // Buttons
    if (reply == QMessageBox::Yes)
    {
        qDebug() << "User clicked Yes";
        writeListToFile();
    }
    else         // QMessageBox::Cancel or closing the dialog
    {
        qDebug() << "User clicked Cancel or closed the dialog";
        // Do nothing or abort
    }
}
void MainWindow::on_pushButtonReload_clicked()
{
    disconnectTimer ();
    loadListFromFile();
    connectTimer ();
}
void MainWindow::on_pushButtonAdd_clicked()
{
    showAddExeDialog();
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
    // QString initialPath = ui->lineEditPath->text();
    // if (initialPath.isEmpty())
    /*
       QString initialPath = QDir::homePath();
       QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Choose executable application"),
        initialPath, // Start in user's home directory
        tr("*.exe"), 0, 0);
       if (!filePath.isEmpty()) {
        filePath = QDir::toNativeSeparators(filePath);
        QStringList line_parts = filePath.split('\\');
        QString line_parts_last = line_parts.last();
        QListWidgetItem* newitem = new QListWidgetItem(line_parts_last, ui->listWidgetEnabled);
        ui->listWidgetEnabled->addItem(newitem);
        qDebug() << "User selected file:" << filePath;
        // You can now use filePath to open and read the file
       } else {
        //                fileNameLabel->setText("No file selected.");
        qDebug() << "File dialog cancelled or no file selected.";
       }*/
}
bool MainWindow::backupBatchFile()
{
    updateSettings();
    deleteOldBackups ();
    // m_sKillFile.replace("\\", "\\\\");
    // qDebug() << m_sKillFile;
    QString universalPath1 = QDir::fromNativeSeparators(m_sKillFile);
    QString sourcePath = universalPath1;
    QFileInfo fileInfo(sourcePath);
    universalPath1 = fileInfo.absolutePath();
    QString destinationPath = fileInfo.baseName();
    QDateTime current = QDateTime::currentDateTime();
    LOG_MSG("Current Date and Time: " + current.toString());
    // format it
    QString formatted = current.toString("yyyyMMdd_hhmmss");
    destinationPath = destinationPath + "_" + formatted;
    LOG_MSG("Formatted: " + destinationPath);
    destinationPath = m_sBackupPath + "\\" + destinationPath + ".bat";
    LOG_MSG("destinationPath: " + destinationPath);
    // 2. Check if the destination file exists and remove it if it does (to allow overwriting)
    if (QFile::exists(destinationPath))
    {
        LOG_MSG( "Destination file already exists. Removing...");
        if (!QFile::remove(destinationPath))
        {
            LOG_MSG("Failed to remove existing destination file: " + destinationPath);
            return false;
        }
    }
    // 3. Perform the copy
    if (QFile::copy(sourcePath, destinationPath))
    {
        LOG_MSG("File copied successfully from " + sourcePath + " to " + destinationPath);
        return true;
    }
    else
    {
        LOG_MSG( "Failed to copy file from " + sourcePath + " to " + destinationPath);
        // You can get more specific error information if needed
        // For example:
        // QFile destFile(destinationPath);
        // qDebug() << "Error:" << destFile.errorString();
        return false;
    }
}

QString MainWindow::getListItemTooltip(QString newItemText)
{
    QString sItemTooltip = "";
    ProcessItem *appItem = m_ProcessListEx.findProcessItem (newItemText);
    if (appItem != nullptr)
    {
        sItemTooltip = "<b>Process:</b> " + newItemText + "<br>";
        sItemTooltip.append("<b>Path:</b> " + appItem->getProcessPath () + "<br>");
        sItemTooltip.append("<b>PID:</b> " + QString::number(appItem->getProcessID ()) + "<br>");
        sItemTooltip.append("<b>Parent PID:</b> " + QString::number(appItem->getParentProcessID ()) + "<br>");
        if (appItem->getIsProcessWindowVisible ())
        {
            sItemTooltip.append("<b>Visible:</b> Yes<br>");
            sItemTooltip.append("<b>Title:</b> " + appItem->getWindowTitle () + "<br>");
        }
        else
            sItemTooltip.append("<b>Visible:</b> No<br>");
        if (appItem->getIsService ())sItemTooltip.append("<b>Service:</b> Yes<br>");
        else sItemTooltip.append("<b>Service:</b> No<br>");
        sItemTooltip.append("<b>Thread count:</b> " + QString::number(appItem->getThreadCount ()) + "<br>");
        sItemTooltip.append("<b>Priority:</b> " + appItem->getPriorityClassName () );
        return sItemTooltip;
    }
    LOG_MSG(newItemText << " appItem == nullptr");
    return sItemTooltip;
}

void MainWindow::addItemToListwidget(QListWidget * listWidget, QString newItemText)
{
    bool bFound = false;
    QString itemString = "";
    // Iterate in reverse to safely remove items while modifying the list
    //qDebug()<< "m_ApplicationItemsList.size="<<m_ApplicationItemsList.size();
    int iFound = m_ProcessListEx.findApplicationItemIndex (newItemText);
    if (iFound != -1) bFound = true;
    // for (int i = m_ApplicationItemsList.size() - 1; i >= 0; --i)
    // {
    // if (m_ApplicationItemsList.at(i).getAppName() == newItemText)
    // {
    // bFound = true;
    // }
    // }
    // bool found = false;
    // for (int i = 0; i < listWidget->count(); ++i) {
    // if (listWidget->item(i)->text() == newItemText) {
    // found = true;
    // break;
    // }
    // }
    if (bFound)
    {
        QMessageBox::information(this, "Duplicate", QString("'%1' already exists in the list.").arg(newItemText));
    }
    else
    {
        bFound = m_ProcessListEx.isRunning (newItemText);
        //qDebug() << __FUNCTION__ << newItemText;
        if (bFound)
        {
            QListWidgetItem* item = new QListWidgetItem(newItemText);
            bool bIsService = m_ProcessListEx.processIsService (newItemText);
            if (bIsService)
            {
                item->setFont(m_Font);
                item->setForeground (m_ServiceTextBrush);
                item->setBackground (QColor(239, 191, 191));
            }
            else
            {
                item->setFont(m_Font);
                item->setForeground (m_TextBrush);
                //item->setBackground (QColor(247,209,209));
                item->setBackground (QColor(191, 191, 239));
            }
            item->setIcon (m_ProcessListEx.getProcessIcon (newItemText.toStdString (), false));
            listWidget->addItem(item);
            // m_ProcessListEx.debugProcessItemsList();
            item->setToolTip (getListItemTooltip(newItemText));
            if (newItemText == m_sSelectedEnabledItem)
            {
                item->setSelected (true);
                //qDebug() << "selected" << newItemText;
                listWidget->setCurrentItem(item);
            }
            if (newItemText == m_sSelectedDisabledItem)
            {
                item->setSelected (true);
                //qDebug() << "selectedDis" << newItemText;
                listWidget->setCurrentItem(item);
            }
        }
        else
        {
            QListWidgetItem* item = new QListWidgetItem(newItemText);
            listWidget->addItem(item);
            if (newItemText == m_sSelectedEnabledItem)
            {
                item->setSelected (true);
                //qDebug() << "selected" << newItemText;
                listWidget->setCurrentItem(item);
            }
            if (newItemText == m_sSelectedDisabledItem)
            {
                item->setSelected (true);
                //qDebug() << "selectedDis" << newItemText;
                listWidget->setCurrentItem(item);
            }
        }
        // ApplicationItem newAppItem(newItemText,true);
        // m_ApplicationItemsList.append(newAppItem);
        //QListWidgetItem* newitem = new QListWidgetItem(receivedText, ui->listWidgetEnabled);
        //ui->listWidgetEnabled->addItem(newitem);
    }
    //qDebug() << __FUNCTION__ << "found= " << bFound;
}

void MainWindow::debugNotFoundWhenKilling()
{
    int i_AppItemCount = m_ProcessListEx.count ();
    ProcessItem *foundItem;
    //for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = m_ProcessListEx.at(iCount);
        if (foundItem->getFoundWhenKilling () == false) qDebug() << foundItem->getAppName ();
    }
}

void MainWindow::debugFoundWhenKilling()
{
    ui->listWidgetKilled->clear ();
    int i_AppItemCount = m_ProcessListEx.count ();
    ProcessItem *foundItem;
    //for (int iCount = i_AppItemCount - 1; iCount >= 0; --iCount)
    for (int iCount = 0; iCount < i_AppItemCount; iCount++)
    {
        foundItem = m_ProcessListEx.at(iCount);
        if (foundItem->getFoundWhenKilling () && foundItem->getAppKillEnabled ())
        {
            ui->listWidgetKilled->addItem (foundItem->getAppName ());
            //qDebug() << foundItem->getAppName ();
        }
    }
    ui->labelKilled->setText("Killed: " + QString::number(ui->listWidgetKilled->count()));
}
void MainWindow::disconnectTimer()
{
    if (m_bTimerIsConnected == true)
    {
        LOG_MSG("Disconnecting timer");
        disconnect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
        m_bTimerIsConnected = false;
    }
}
void MainWindow::connectTimer()
{
    if (m_bTimerIsConnected == false)
    {
        LOG_MSG("Reconnecting timer");
        //m_iTimerUpdatesCount = 0;
        //timer->setInterval (m_iRefreshRate);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
        m_bTimerIsConnected = true;
    }
}
void MainWindow::readStdOutput()
{
    qApp->processEvents ();
    // QString standardOut = process->readAllStandardOutput ();
    // QString output = standardOut;
    //    //output.replace ("\r\n", "");
    // output.replace ("\n", "");
    // output.replace ("\r", "");
    // QStringList outList = output.split(' ');
    // if (outList.isEmpty ()) qDebug() << "EMPTY";
    //    // qDebug() << "QStringList: " << outList;
    // qDebug() << "size: " << outList.size();
    // for (int iCount = 0; iCount < outList.size (); iCount++)
    // {
    // QString sLine = outList.at (iCount);
    //        //qDebug() << sLine;         //qDebug() << sLine;
    // if (sLine.contains (".exe") )
    // {
    // output = sLine;
    // qDebug() << sLine;
    // }
    // }
    // if (output != "")
    // {
    //        //qDebug() << "std: " << standardOut;
    //        //ui->listWidgetEnabled->addItem (output);
    // }
}
void MainWindow::readStdError()
{
    qApp->processEvents ();
    QString output = QString(process->readAllStandardError ());
    //output.replace ("\r\n", "");
    QStringList lines = output.split ("\n");
    int iLines = lines.count ();
    if (iLines > 2)
    {
        qDebug() << "MORE THAN ONE ROW!!!!!!";
        //QMessageBox::information (this, "", "PIU' DI UNA RIGA");
    }
    QStringList outList;
    QString outputProg;
    ProcessItem *foundItem ;
    for (int iRepeat = 0; iRepeat < iLines - 1; iRepeat++)
    {
        // QString outputProg = output;
        outputProg = lines[iRepeat];
        outputProg.replace ("\n", "");
        outputProg.replace ("\r", "");
        outList = outputProg.split(' ');
        //qDebug() << outList.size();
        outputProg = outList.at(2);
        outputProg.replace ("\"", "");
        //qDebug() << __FUNCTION__ << "err: " << outputProg;
        foundItem = m_ProcessListEx.findApplicationItem (outputProg);
        if (foundItem)
        {
            //qDebug() << "foundItem->getAppName () " << foundItem->getAppName ();
            foundItem->setFoundWhenKilling (false);
        }
        else qDebug() << "foundItem->getAppName () NOT FOUND";
        //ui->listWidgetDisabled->addItem (output);
    }
}
void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Batch CMD Process finished with code:" << exitCode
        << "and status:" << exitStatus;
    ui->statusBar->showMessage(m_sKillFile + " executed.", 10000);
    //debugNotFoundWhenKilling ();
    debugFoundWhenKilling();
    m_statusBarMovie->setPaused (true);
    connectTimer ();
}
void MainWindow::on_pushButtonRun_clicked()
{
    m_iTimerUpdatesCount = 0;
    QString sStatusMessage;
    //m_bKillInternal = true;
    disconnectTimer ();
    m_ProcessListEx.resetAllApplicationItems();
    sStatusMessage = "TerminateProcess";
    sStatusMessage.append (" started...");
    ui->statusBar->showMessage(sStatusMessage, 10000);
    ui->statusBar->repaint ();
    if (m_bKillInternal)
    {
        //m_ProcessList.debugProcessesMemory ();
        bool bKillOk = KillRunningProcesses();
        // if (bKillOk) LOG_MSG("Internal TerminateProcess result= OK");
        // else LOG_MSG("Internal TerminateProcess result= NOT all processes killed");
        sStatusMessage = "TerminateProcess";
        sStatusMessage.append (" executed.");
        ui->statusBar->showMessage(sStatusMessage, 10000);
        //debugNotFoundWhenKilling ();
        debugFoundWhenKilling();
        connectTimer ();
        return;
    }
    else
    {
        LOG_MSG("animation");
        //animation
        int frameCount = m_statusBarMovie->frameCount();
        if (frameCount > 0)
        {
            int frameIndex = (1 * (frameCount - 1)) / 100;
            m_statusBarMovie->jumpToFrame(frameIndex);
        }
        m_statusBarMovie->start ();
        //jumpToFrame(frameIndex);
        //qApp->processEvents ();
        //disconnectTimer ();
        //QProcess *process = new QProcess(this); // 'this' sets the parent, good for memory management
        //m_ApplicationItemsList.resetAllApplicationItems();
        process = new QProcess(this);
        connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOutput()));
        connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readStdError()));
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
        QString program = "cmd.exe";
        QStringList arguments;
        arguments << "/c" << m_sKillFile;       // Use forward slashes for paths in Qt
        // process.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments * args)
        // {
        //        // Questa flag dice a Windows di creare una NUOVA finestra console.
        // args->flags |= CREATE_NEW_CONSOLE;
        // });
        // ui->statusBar->showMessage(m_sKillFile + " started...", 10000);
        // ui->statusBar->repaint ();
        qDebug() << "Process started " << arguments;
        process->start (program, arguments);
        //QProcess *process = new QProcess();
        // Connect signals to slots to capture output and handle process finish
        // QObject::connect(process, &QProcess::readyReadStandardOutput, [process]() {
        // qDebug() << "Standard Output:" << process->readAllStandardOutput();
        // });
        // QObject::connect(process, &QProcess::readyReadStandardError, [process]() {
        // qWarning() << "Standard Error:" << process->readAllStandardError();
        // });
        // QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        // [process](int exitCode, QProcess::ExitStatus exitStatus) {
        // qDebug() << "Batch file finished with exit code:" << exitCode
        // << "and exit status:" << exitStatus;
        // process->deleteLater(); // Clean up the process object
        //          //QCoreApplication::quit(); // Quit the application after the batch file finishes
        // });
        // process->start("cmd.exe", QStringList() << "/c" << m_sKillFile);   //process.startDetached (program, QStringList() << "/C" << m_sKillFile);
        // Optional: Wait for the process to finish
        process->waitForStarted(-1);
        process->waitForFinished(-1);       // -1 means wait indefinitely
        process->deleteLater ();
        delete process;
        //ui->statusBar->showMessage(m_sKillFile + " executed.");
        // Optional: Read standard output/error
        // qDebug() << "Standard Output:" << process->readAllStandardOutput();
        // qDebug() << "Standard Error:" << process->readAllStandardError();
        // Connect to signals for asynchronous handling
        // connect(process, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        // qDebug() << "Batch file finished with exit code:" << exitCode << "and status:" << exitStatus;
        // qDebug() << "Standard Output:\n" << process->readAllStandardOutput();
        // qDebug() << "Standard Error:\n" << process->readAllStandardError();
        // process->deleteLater(); // Clean up the QProcess object
        // });
        // connect(process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        // qDebug() << "Error starting batch file:" << error;
        // qDebug() << "Error string:" << process->errorString();
        // process->deleteLater();
        // });
    }
}
void MainWindow::on_actionConfigure_app_triggered()
{
    menuConfigure();
}
void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
    // QStringList processes = getRunningProcesses();
    //   // ui->listWidgetDisabled->clear(); // Assuming you have a QListWidget named 'listWidget'
    // ui->listWidgetDisabled->addItems(processes);
    // qDebug() << "Refreshed process list. Total processes:" << processes.count();
}
void MainWindow::on_actionAbout_triggered()
{
    QString sIsDebug = "";
#ifdef QT_DEBUG
    sIsDebug = " Debug ";
#else
    sIsDebug = " Release ";
#endif
    QString sVersion = "<h3><p style=\"font-weight: bold; color: rgb(0, 0, 128);\">Version ";
    sVersion = sVersion + APP_VERSION;
    sVersion = sVersion + sIsDebug + "</p></h3>";
    QString sApp = qApp->applicationName();
    QMessageBox::about(this, "About " + qApp->applicationName(),
        "<h1><p style=\"font-weight: bold; color: rgb(0, 0, 128);\">"
        + sApp + "</p></h1>"
        + sVersion +
    "<p>Build date and time: " + __DATE__ + " at " + __TIME__ + "</p>"
    "<br>"
    "<p>This application manages a 'KILL' batch file.</p>"
    "<br>"
    "<p>Copyright &copy; 2025 Andrea G.</p>"
    "<p>All rights reserved.</p>"
        /* "<p>Visit our website: <a href='https://www.example.com'>www.example.com</a></p>"*/);
}
void MainWindow::on_actionOpen_in_external_editor_triggered()
{
    QSettings settings;
    QProcess process;
    QString program = "notepad.exe";
    program = settings.value("Dialog/External editor", "notepad.exe").toString();
    QStringList arguments;
    arguments << m_sKillFile;       // Use forward slashes for paths in Qt
    process.startDetached(program, arguments);
    ui->statusBar->showMessage(m_sKillFile + " opened in external editor.", 10000);
    LOG_MSG( "External editor started " + arguments.join (' '));
    // Optional: Wait for the process to finish
    // process.waitForStarted(-1);
    // process.waitForFinished(-1); // -1 means wait indefinitely
    // ui->statusBar->showMessage(m_sKillFile + " executed.");
}
void MainWindow::on_actionExecute_in_terminal_window_triggered()
{
    ui->statusBar->showMessage(m_sKillFile + " started in terminal window...", 10000);
    ui->statusBar->repaint ();
    const char *cstrKillFile = m_sKillFile.toUtf8().data();
    std::string command = std::string("cmd /C ") + cstrKillFile;
    system(command.c_str());
    LOG_MSG("Execute_in_terminal_window started: " + QString::fromStdString (command.c_str()) );
    ui->statusBar->showMessage(m_sKillFile + " executed in terminal window.", 10000);
}

void MainWindow::on_actionOpen_log_file_in_external_editor_triggered()
{
    // QString filePath = QCoreApplication::applicationDirPath();
    // filePath = QDir::toNativeSeparators (filePath);
    // filePath.append ("\\KillManagerQt.log");
    QSettings settings;
    QProcess process;
    QString program = "notepad.exe";
    program = settings.value("Dialog/External editor", "notepad.exe").toString();
    QStringList arguments;
    arguments << m_sLogFileName;
    process.startDetached(program, arguments);
    ui->statusBar->showMessage("Log file opened in external editor.", 10000);
    qDebug() << "Opening log file " << arguments;
}

bool MainWindow::KillRunningProcesses()
{
    bool bKilled = false;
    int iCount;
    iCount = ui->listWidgetEnabled->count();
    //animation
    int frameCount = m_statusBarMovie->frameCount();
    if (frameCount > 0)
    {
        int frameIndex = (1 * (frameCount - 1)) / 100;
        m_statusBarMovie->jumpToFrame(frameIndex);
    }
    //m_iTimerUpdatesCount += 1;
    //int iModulus = m_iTimerUpdatesCount % 10 ;
    QString sItemText;
    ProcessItem *foundItem ;
    for (int i = 0; i < iCount; i++)
    {
        sItemText = ui->listWidgetEnabled->item(i)->text();
        bKilled = m_ProcessListEx.killProcessAndChildsByNameEx (sItemText);
        foundItem = m_ProcessListEx.findApplicationItem (sItemText);
        if (foundItem)
        {
            //LOG_MSG(sItemText);
            //LOG_MSG("KILLED " + sItemText);
            //qDebug() << "foundItem->getAppName () " << foundItem->getAppName ();
            foundItem->setFoundWhenKilling (bKilled);
        }
        else
        {
            foundItem->setFoundWhenKilling (bKilled);
            qDebug() << "foundItem->getAppName () NOT FOUND";
        }
        int progress = double(double(i + 1) / double(iCount)) * 100;
        //progress=double(double(iModulus)/double(frameCount) *100);
        //LOG_VAR(progress);
        int frameIndex = (progress * m_statusBarMovie->frameCount()) / 100;
        frameIndex = qMin(frameIndex, m_statusBarMovie->frameCount() - 1);
        //LOG_VAR(frameIndex);
        m_statusBarMovie->jumpToFrame(frameIndex);
        qApp->processEvents ();
    }
    return bKilled;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}
