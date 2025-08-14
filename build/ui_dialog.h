/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelPath_2;
    QPushButton *pushButtonCreateNew;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout;
    QLabel *labelPath;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEditPath;
    QPushButton *pushButtonChoose;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelIcon;
    QLineEdit *lineEditExternaEditor;
    QPushButton *pushButtonChooseExternaEditor;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelBackupPath;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEditBackupPath;
    QPushButton *pushButtonChooseBackupPath;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QPushButton *pushButtonOpenBackup;
    QCheckBox *checkBoxDeleteBackups;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QSpinBox *spinBoxBackupsCount;
    QLabel *label_4;
    QSpinBox *spinBoxBackupsDays;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelBackupsInfo;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QSpinBox *spinBoxUpdateRate;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *checkBoxKill;
    QCheckBox *checkBoxLog;
    QLabel *labelLog;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(443, 709);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
        Dialog->setSizePolicy(sizePolicy);
        Dialog->setMinimumSize(QSize(0, 0));
        verticalLayout_9 = new QVBoxLayout(Dialog);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        groupBox_5 = new QGroupBox(Dialog);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy1);
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        font.setKerning(true);
        groupBox_5->setFont(font);
        verticalLayout_8 = new QVBoxLayout(groupBox_5);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        labelPath_2 = new QLabel(groupBox_5);
        labelPath_2->setObjectName(QString::fromUtf8("labelPath_2"));
        sizePolicy1.setHeightForWidth(labelPath_2->sizePolicy().hasHeightForWidth());
        labelPath_2->setSizePolicy(sizePolicy1);
        labelPath_2->setMinimumSize(QSize(0, 19));

        horizontalLayout_6->addWidget(labelPath_2);

        pushButtonCreateNew = new QPushButton(groupBox_5);
        pushButtonCreateNew->setObjectName(QString::fromUtf8("pushButtonCreateNew"));

        horizontalLayout_6->addWidget(pushButtonCreateNew);


        verticalLayout_8->addLayout(horizontalLayout_6);


        verticalLayout_9->addWidget(groupBox_5);

        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPath = new QLabel(groupBox);
        labelPath->setObjectName(QString::fromUtf8("labelPath"));
        labelPath->setMinimumSize(QSize(0, 19));
        labelPath->setTextFormat(Qt::PlainText);

        verticalLayout->addWidget(labelPath);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEditPath = new QLineEdit(groupBox);
        lineEditPath->setObjectName(QString::fromUtf8("lineEditPath"));

        horizontalLayout->addWidget(lineEditPath);

        pushButtonChoose = new QPushButton(groupBox);
        pushButtonChoose->setObjectName(QString::fromUtf8("pushButtonChoose"));

        horizontalLayout->addWidget(pushButtonChoose);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(verticalLayout);


        verticalLayout_9->addWidget(groupBox);

        groupBox_2 = new QGroupBox(Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelIcon = new QLabel(groupBox_2);
        labelIcon->setObjectName(QString::fromUtf8("labelIcon"));

        horizontalLayout_2->addWidget(labelIcon);

        lineEditExternaEditor = new QLineEdit(groupBox_2);
        lineEditExternaEditor->setObjectName(QString::fromUtf8("lineEditExternaEditor"));

        horizontalLayout_2->addWidget(lineEditExternaEditor);

        pushButtonChooseExternaEditor = new QPushButton(groupBox_2);
        pushButtonChooseExternaEditor->setObjectName(QString::fromUtf8("pushButtonChooseExternaEditor"));

        horizontalLayout_2->addWidget(pushButtonChooseExternaEditor);


        verticalLayout_4->addLayout(horizontalLayout_2);


        verticalLayout_9->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy2);
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelBackupPath = new QLabel(groupBox_3);
        labelBackupPath->setObjectName(QString::fromUtf8("labelBackupPath"));
        sizePolicy1.setHeightForWidth(labelBackupPath->sizePolicy().hasHeightForWidth());
        labelBackupPath->setSizePolicy(sizePolicy1);
        labelBackupPath->setMinimumSize(QSize(0, 38));

        verticalLayout_2->addWidget(labelBackupPath);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lineEditBackupPath = new QLineEdit(groupBox_3);
        lineEditBackupPath->setObjectName(QString::fromUtf8("lineEditBackupPath"));

        horizontalLayout_3->addWidget(lineEditBackupPath);

        pushButtonChooseBackupPath = new QPushButton(groupBox_3);
        pushButtonChooseBackupPath->setObjectName(QString::fromUtf8("pushButtonChooseBackupPath"));

        horizontalLayout_3->addWidget(pushButtonChooseBackupPath);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        pushButtonOpenBackup = new QPushButton(groupBox_3);
        pushButtonOpenBackup->setObjectName(QString::fromUtf8("pushButtonOpenBackup"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/img/icons8-history-folder-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonOpenBackup->setIcon(icon);

        horizontalLayout_4->addWidget(pushButtonOpenBackup);


        verticalLayout_2->addLayout(horizontalLayout_4);

        checkBoxDeleteBackups = new QCheckBox(groupBox_3);
        checkBoxDeleteBackups->setObjectName(QString::fromUtf8("checkBoxDeleteBackups"));

        verticalLayout_2->addWidget(checkBoxDeleteBackups);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy3);

        horizontalLayout_7->addWidget(label_5);

        spinBoxBackupsCount = new QSpinBox(groupBox_3);
        spinBoxBackupsCount->setObjectName(QString::fromUtf8("spinBoxBackupsCount"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(spinBoxBackupsCount->sizePolicy().hasHeightForWidth());
        spinBoxBackupsCount->setSizePolicy(sizePolicy4);
        spinBoxBackupsCount->setMinimum(1);
        spinBoxBackupsCount->setMaximum(1000);

        horizontalLayout_7->addWidget(spinBoxBackupsCount);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy3.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy3);

        horizontalLayout_7->addWidget(label_4);

        spinBoxBackupsDays = new QSpinBox(groupBox_3);
        spinBoxBackupsDays->setObjectName(QString::fromUtf8("spinBoxBackupsDays"));
        sizePolicy4.setHeightForWidth(spinBoxBackupsDays->sizePolicy().hasHeightForWidth());
        spinBoxBackupsDays->setSizePolicy(sizePolicy4);
        spinBoxBackupsDays->setMinimum(1);
        spinBoxBackupsDays->setMaximum(1000);

        horizontalLayout_7->addWidget(spinBoxBackupsDays);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy3.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy3);

        horizontalLayout_7->addWidget(label_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_7);

        labelBackupsInfo = new QLabel(groupBox_3);
        labelBackupsInfo->setObjectName(QString::fromUtf8("labelBackupsInfo"));

        verticalLayout_2->addWidget(labelBackupsInfo);


        verticalLayout_9->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_7 = new QVBoxLayout(groupBox_4);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_6->addWidget(label_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_5->addWidget(label_3);

        spinBoxUpdateRate = new QSpinBox(groupBox_4);
        spinBoxUpdateRate->setObjectName(QString::fromUtf8("spinBoxUpdateRate"));
        spinBoxUpdateRate->setMinimum(1);
        spinBoxUpdateRate->setMaximum(10000);
        spinBoxUpdateRate->setValue(600);

        horizontalLayout_5->addWidget(spinBoxUpdateRate);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_6->addLayout(horizontalLayout_5);


        verticalLayout_7->addLayout(verticalLayout_6);


        verticalLayout_9->addWidget(groupBox_4);

        groupBox_6 = new QGroupBox(Dialog);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        verticalLayout_3 = new QVBoxLayout(groupBox_6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        checkBoxKill = new QCheckBox(groupBox_6);
        checkBoxKill->setObjectName(QString::fromUtf8("checkBoxKill"));

        verticalLayout_3->addWidget(checkBoxKill);

        checkBoxLog = new QCheckBox(groupBox_6);
        checkBoxLog->setObjectName(QString::fromUtf8("checkBoxLog"));

        verticalLayout_3->addWidget(checkBoxLog);

        labelLog = new QLabel(groupBox_6);
        labelLog->setObjectName(QString::fromUtf8("labelLog"));

        verticalLayout_3->addWidget(labelLog);


        verticalLayout_9->addWidget(groupBox_6);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_9->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_9->addWidget(buttonBox);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("Dialog", "New batch file", nullptr));
        labelPath_2->setText(QCoreApplication::translate("Dialog", "Create a new empty batch file.", nullptr));
        pushButtonCreateNew->setText(QCoreApplication::translate("Dialog", "Create new...", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Dialog", "Existing batch file", nullptr));
        labelPath->setText(QCoreApplication::translate("Dialog", "Select an existing batch file or type (or paste) the long path to the file.", nullptr));
        pushButtonChoose->setText(QCoreApplication::translate("Dialog", "Choose...", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Dialog", "External text editor", nullptr));
        labelIcon->setText(QString());
        lineEditExternaEditor->setText(QCoreApplication::translate("Dialog", "external editor", nullptr));
        pushButtonChooseExternaEditor->setText(QCoreApplication::translate("Dialog", "Choose...", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Dialog", "Backups", nullptr));
        labelBackupPath->setText(QCoreApplication::translate("Dialog", "A new backup file is created whenever changes occur.\n"
"Select the backup folder or type (or paste) the long path to the folder.", nullptr));
        lineEditBackupPath->setText(QString());
        lineEditBackupPath->setPlaceholderText(QCoreApplication::translate("Dialog", "Backups folder", nullptr));
        pushButtonChooseBackupPath->setText(QCoreApplication::translate("Dialog", "Choose...", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Open backup folder to delete old backup files.", nullptr));
        pushButtonOpenBackup->setText(QCoreApplication::translate("Dialog", "Open backup folder", nullptr));
        checkBoxDeleteBackups->setText(QCoreApplication::translate("Dialog", "Delete old backups", nullptr));
        label_5->setText(QCoreApplication::translate("Dialog", "If there are more than", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "and older than", nullptr));
        label_6->setText(QCoreApplication::translate("Dialog", "days", nullptr));
        labelBackupsInfo->setText(QCoreApplication::translate("Dialog", "TextLabel", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Dialog", "Update interval", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "The refresh interval determines how often the page reloads data.", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "Update rate in seconds:", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("Dialog", "Other settings", nullptr));
        checkBoxKill->setText(QCoreApplication::translate("Dialog", "Use internal kill (TerminateProcess function) instead of executing the batch file", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxLog->setToolTip(QCoreApplication::translate("Dialog", "After changing this option, you must restart the program for it to take effect.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxLog->setText(QCoreApplication::translate("Dialog", "Log to file", nullptr));
        labelLog->setText(QCoreApplication::translate("Dialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
