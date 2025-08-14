/********************************************************************************
** Form generated from reading UI file 'addexedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDEXEDIALOG_H
#define UI_ADDEXEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddExeDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelChooseExe;
    QHBoxLayout *horizontalLayout;
    QLabel *labelIcon;
    QLineEdit *lineEditPath;
    QPushButton *pushButtonChoose;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AddExeDialog)
    {
        if (AddExeDialog->objectName().isEmpty())
            AddExeDialog->setObjectName(QString::fromUtf8("AddExeDialog"));
        AddExeDialog->resize(445, 133);
        verticalLayout_3 = new QVBoxLayout(AddExeDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelChooseExe = new QLabel(AddExeDialog);
        labelChooseExe->setObjectName(QString::fromUtf8("labelChooseExe"));
        labelChooseExe->setMinimumSize(QSize(0, 38));

        verticalLayout->addWidget(labelChooseExe);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelIcon = new QLabel(AddExeDialog);
        labelIcon->setObjectName(QString::fromUtf8("labelIcon"));
        labelIcon->setPixmap(QPixmap(QString::fromUtf8(":/icons/img/imageres_15.ico")));

        horizontalLayout->addWidget(labelIcon);

        lineEditPath = new QLineEdit(AddExeDialog);
        lineEditPath->setObjectName(QString::fromUtf8("lineEditPath"));

        horizontalLayout->addWidget(lineEditPath);

        pushButtonChoose = new QPushButton(AddExeDialog);
        pushButtonChoose->setObjectName(QString::fromUtf8("pushButtonChoose"));

        horizontalLayout->addWidget(pushButtonChoose);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(AddExeDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(AddExeDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AddExeDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AddExeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AddExeDialog);
    } // setupUi

    void retranslateUi(QDialog *AddExeDialog)
    {
        AddExeDialog->setWindowTitle(QCoreApplication::translate("AddExeDialog", "Dialog", nullptr));
        labelChooseExe->setText(QCoreApplication::translate("AddExeDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Select executable file or type (or paste) executable name (example: myexecutable.exe)</p></body></html>", nullptr));
        labelIcon->setText(QString());
        pushButtonChoose->setText(QCoreApplication::translate("AddExeDialog", "Choose...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddExeDialog: public Ui_AddExeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDEXEDIALOG_H
