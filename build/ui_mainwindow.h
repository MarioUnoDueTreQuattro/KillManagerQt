/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "myqlistwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConfigure_app;
    QAction *actionOpen_in_external_editor;
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QAction *actionExecute_in_terminal_window;
    QAction *actionOpen_log_file_in_external_editor;
    QAction *actionExit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelEnabled;
    MyQListWidget *listWidgetEnabled;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelDisabled;
    MyQListWidget *listWidgetDisabled;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelKilled;
    QListWidget *listWidgetKilled;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonReload;
    QPushButton *pushButtonWrite;
    QPushButton *pushButtonRun;
    QMenuBar *menuBar;
    QMenu *menu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(595, 522);
        actionConfigure_app = new QAction(MainWindow);
        actionConfigure_app->setObjectName(QString::fromUtf8("actionConfigure_app"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/img/icons8-wrench-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfigure_app->setIcon(icon);
        actionOpen_in_external_editor = new QAction(MainWindow);
        actionOpen_in_external_editor->setObjectName(QString::fromUtf8("actionOpen_in_external_editor"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/img/icons8-pencil-drawing-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_in_external_editor->setIcon(icon1);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/img/KillManager.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon2);
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/img/qt-logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout_Qt->setIcon(icon3);
        actionExecute_in_terminal_window = new QAction(MainWindow);
        actionExecute_in_terminal_window->setObjectName(QString::fromUtf8("actionExecute_in_terminal_window"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/img/icons8-terminal-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExecute_in_terminal_window->setIcon(icon4);
        actionOpen_log_file_in_external_editor = new QAction(MainWindow);
        actionOpen_log_file_in_external_editor->setObjectName(QString::fromUtf8("actionOpen_log_file_in_external_editor"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/img/icons8-log-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_log_file_in_external_editor->setIcon(icon5);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/img/icons8-quit-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon6);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelEnabled = new QLabel(centralWidget);
        labelEnabled->setObjectName(QString::fromUtf8("labelEnabled"));

        verticalLayout->addWidget(labelEnabled);

        listWidgetEnabled = new MyQListWidget(centralWidget);
        listWidgetEnabled->setObjectName(QString::fromUtf8("listWidgetEnabled"));

        verticalLayout->addWidget(listWidgetEnabled);


        horizontalLayout->addLayout(verticalLayout);

        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        labelDisabled = new QLabel(layoutWidget);
        labelDisabled->setObjectName(QString::fromUtf8("labelDisabled"));

        verticalLayout_2->addWidget(labelDisabled);

        listWidgetDisabled = new MyQListWidget(layoutWidget);
        listWidgetDisabled->setObjectName(QString::fromUtf8("listWidgetDisabled"));

        verticalLayout_2->addWidget(listWidgetDisabled);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout_3 = new QVBoxLayout(layoutWidget1);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        labelKilled = new QLabel(layoutWidget1);
        labelKilled->setObjectName(QString::fromUtf8("labelKilled"));

        verticalLayout_3->addWidget(labelKilled);

        listWidgetKilled = new QListWidget(layoutWidget1);
        listWidgetKilled->setObjectName(QString::fromUtf8("listWidgetKilled"));

        verticalLayout_3->addWidget(listWidgetKilled);

        splitter->addWidget(layoutWidget1);

        horizontalLayout->addWidget(splitter);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButtonAdd = new QPushButton(centralWidget);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/img/icons8-add-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonAdd->setIcon(icon7);

        horizontalLayout_2->addWidget(pushButtonAdd);

        pushButtonReload = new QPushButton(centralWidget);
        pushButtonReload->setObjectName(QString::fromUtf8("pushButtonReload"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/img/icons8-rotate-right-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonReload->setIcon(icon8);

        horizontalLayout_2->addWidget(pushButtonReload);

        pushButtonWrite = new QPushButton(centralWidget);
        pushButtonWrite->setObjectName(QString::fromUtf8("pushButtonWrite"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/img/icons8-save-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonWrite->setIcon(icon9);

        horizontalLayout_2->addWidget(pushButtonWrite);

        pushButtonRun = new QPushButton(centralWidget);
        pushButtonRun->setObjectName(QString::fromUtf8("pushButtonRun"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/img/icons8-run-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonRun->setIcon(icon10);

        horizontalLayout_2->addWidget(pushButtonRun);


        verticalLayout_4->addLayout(horizontalLayout_2);


        verticalLayout_5->addLayout(verticalLayout_4);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 595, 18));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setFocusPolicy(Qt::NoFocus);
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(actionConfigure_app);
        menu->addSeparator();
        menu->addAction(actionOpen_in_external_editor);
        menu->addAction(actionExecute_in_terminal_window);
        menu->addAction(actionOpen_log_file_in_external_editor);
        menu->addSeparator();
        menu->addAction(actionAbout);
        menu->addAction(actionAbout_Qt);
        menu->addSeparator();
        menu->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionConfigure_app->setText(QCoreApplication::translate("MainWindow", "Configure", nullptr));
#if QT_CONFIG(statustip)
        actionConfigure_app->setStatusTip(QCoreApplication::translate("MainWindow", "Configure application", nullptr));
#endif // QT_CONFIG(statustip)
        actionOpen_in_external_editor->setText(QCoreApplication::translate("MainWindow", "Open in external editor", nullptr));
#if QT_CONFIG(statustip)
        actionOpen_in_external_editor->setStatusTip(QCoreApplication::translate("MainWindow", "Open batch file in external editor", nullptr));
#endif // QT_CONFIG(statustip)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
#if QT_CONFIG(statustip)
        actionAbout->setStatusTip(QCoreApplication::translate("MainWindow", "About application", nullptr));
#endif // QT_CONFIG(statustip)
        actionAbout_Qt->setText(QCoreApplication::translate("MainWindow", "About Qt", nullptr));
#if QT_CONFIG(statustip)
        actionAbout_Qt->setStatusTip(QCoreApplication::translate("MainWindow", "About Qt C++ toolkit", nullptr));
#endif // QT_CONFIG(statustip)
        actionExecute_in_terminal_window->setText(QCoreApplication::translate("MainWindow", "Execute in terminal window", nullptr));
#if QT_CONFIG(statustip)
        actionExecute_in_terminal_window->setStatusTip(QCoreApplication::translate("MainWindow", "Execute batch file in terminal window", nullptr));
#endif // QT_CONFIG(statustip)
        actionOpen_log_file_in_external_editor->setText(QCoreApplication::translate("MainWindow", "Open log file in external editor", nullptr));
#if QT_CONFIG(statustip)
        actionOpen_log_file_in_external_editor->setStatusTip(QCoreApplication::translate("MainWindow", "Open log file in external editor", nullptr));
#endif // QT_CONFIG(statustip)
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(statustip)
        actionExit->setStatusTip(QCoreApplication::translate("MainWindow", "Exit application", nullptr));
#endif // QT_CONFIG(statustip)
        labelEnabled->setText(QCoreApplication::translate("MainWindow", "Enabled", nullptr));
        labelDisabled->setText(QCoreApplication::translate("MainWindow", "Disabled", nullptr));
        labelKilled->setText(QCoreApplication::translate("MainWindow", "Killed", nullptr));
#if QT_CONFIG(tooltip)
        pushButtonAdd->setToolTip(QCoreApplication::translate("MainWindow", "Add executable to list", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButtonAdd->setText(QCoreApplication::translate("MainWindow", "Add...", nullptr));
#if QT_CONFIG(tooltip)
        pushButtonReload->setToolTip(QCoreApplication::translate("MainWindow", "Reload batch file from disk", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButtonReload->setText(QCoreApplication::translate("MainWindow", "Reload", nullptr));
#if QT_CONFIG(tooltip)
        pushButtonWrite->setToolTip(QCoreApplication::translate("MainWindow", "Save batch file to disk", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButtonWrite->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(tooltip)
        pushButtonRun->setToolTip(QCoreApplication::translate("MainWindow", "Kill running executables", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButtonRun->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
#if QT_CONFIG(statustip)
        menuBar->setStatusTip(QCoreApplication::translate("MainWindow", "Menu", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(statustip)
        menu->setStatusTip(QCoreApplication::translate("MainWindow", "Menu", nullptr));
#endif // QT_CONFIG(statustip)
        menu->setTitle(QCoreApplication::translate("MainWindow", "Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
