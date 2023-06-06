/********************************************************************************
** Form generated from reading UI file 'CreateVSProject.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEVSPROJECT_H
#define UI_CREATEVSPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateVSProjectClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_ProjectName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboBox_Base;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *lineEdit_Path;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_Create;

    void setupUi(QWidget *CreateVSProjectClass)
    {
        if (CreateVSProjectClass->objectName().isEmpty())
            CreateVSProjectClass->setObjectName(QString::fromUtf8("CreateVSProjectClass"));
        CreateVSProjectClass->resize(353, 148);
        verticalLayout = new QVBoxLayout(CreateVSProjectClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CreateVSProjectClass);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit_ProjectName = new QLineEdit(CreateVSProjectClass);
        lineEdit_ProjectName->setObjectName(QString::fromUtf8("lineEdit_ProjectName"));

        horizontalLayout->addWidget(lineEdit_ProjectName);

        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CreateVSProjectClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboBox_Base = new QComboBox(CreateVSProjectClass);
        comboBox_Base->setObjectName(QString::fromUtf8("comboBox_Base"));

        horizontalLayout_2->addWidget(comboBox_Base);

        horizontalLayout_2->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CreateVSProjectClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        lineEdit_Path = new QLineEdit(CreateVSProjectClass);
        lineEdit_Path->setObjectName(QString::fromUtf8("lineEdit_Path"));

        horizontalLayout_3->addWidget(lineEdit_Path);

        horizontalLayout_3->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton_Create = new QPushButton(CreateVSProjectClass);
        pushButton_Create->setObjectName(QString::fromUtf8("pushButton_Create"));

        horizontalLayout_4->addWidget(pushButton_Create);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(CreateVSProjectClass);

        QMetaObject::connectSlotsByName(CreateVSProjectClass);
    } // setupUi

    void retranslateUi(QWidget *CreateVSProjectClass)
    {
        CreateVSProjectClass->setWindowTitle(QCoreApplication::translate("CreateVSProjectClass", "CreateVSProject", nullptr));
        label->setText(QCoreApplication::translate("CreateVSProjectClass", "\345\267\245\347\250\213\345\220\215\347\247\260\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("CreateVSProjectClass", "\345\267\245\347\250\213\345\237\272\347\261\273\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("CreateVSProjectClass", "\347\224\237\346\210\220\350\267\257\345\276\204\357\274\232", nullptr));
        pushButton_Create->setText(QCoreApplication::translate("CreateVSProjectClass", "\347\224\237\346\210\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateVSProjectClass: public Ui_CreateVSProjectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEVSPROJECT_H
