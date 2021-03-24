/********************************************************************************
** Form generated from reading UI file 'MutiCam.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUTICAM_H
#define UI_MUTICAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MutiCamClass
{
public:
    QWidget *centralWidget;
    QListWidget *m_ListBoxCameras;
    QListWidget *m_ListLog;
    QPushButton *m_ButtonStartStop;
    QLabel *m_LabelStream_1;
    QCheckBox *m_ColorProcessingCheckBox;
    QLabel *m_LabelStream_2;

    void setupUi(QMainWindow *MutiCamClass)
    {
        if (MutiCamClass->objectName().isEmpty())
            MutiCamClass->setObjectName(QString::fromUtf8("MutiCamClass"));
        MutiCamClass->resize(1050, 500);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MutiCamClass->sizePolicy().hasHeightForWidth());
        MutiCamClass->setSizePolicy(sizePolicy);
        MutiCamClass->setMinimumSize(QSize(1050, 500));
        MutiCamClass->setMaximumSize(QSize(1050, 500));
        centralWidget = new QWidget(MutiCamClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        m_ListBoxCameras = new QListWidget(centralWidget);
        m_ListBoxCameras->setObjectName(QString::fromUtf8("m_ListBoxCameras"));
        m_ListBoxCameras->setGeometry(QRect(0, 10, 261, 211));
        m_ListBoxCameras->setSelectionMode(QAbstractItemView::MultiSelection);
        m_ListLog = new QListWidget(centralWidget);
        m_ListLog->setObjectName(QString::fromUtf8("m_ListLog"));
        m_ListLog->setGeometry(QRect(0, 300, 1041, 191));
        m_ButtonStartStop = new QPushButton(centralWidget);
        m_ButtonStartStop->setObjectName(QString::fromUtf8("m_ButtonStartStop"));
        m_ButtonStartStop->setGeometry(QRect(0, 260, 261, 31));
        m_LabelStream_1 = new QLabel(centralWidget);
        m_LabelStream_1->setObjectName(QString::fromUtf8("m_LabelStream_1"));
        m_LabelStream_1->setGeometry(QRect(270, 10, 385, 280));
        sizePolicy.setHeightForWidth(m_LabelStream_1->sizePolicy().hasHeightForWidth());
        m_LabelStream_1->setSizePolicy(sizePolicy);
        m_ColorProcessingCheckBox = new QCheckBox(centralWidget);
        m_ColorProcessingCheckBox->setObjectName(QString::fromUtf8("m_ColorProcessingCheckBox"));
        m_ColorProcessingCheckBox->setGeometry(QRect(140, 230, 121, 20));
        m_LabelStream_2 = new QLabel(centralWidget);
        m_LabelStream_2->setObjectName(QString::fromUtf8("m_LabelStream_2"));
        m_LabelStream_2->setGeometry(QRect(660, 10, 385, 280));
        sizePolicy.setHeightForWidth(m_LabelStream_2->sizePolicy().hasHeightForWidth());
        m_LabelStream_2->setSizePolicy(sizePolicy);
        MutiCamClass->setCentralWidget(centralWidget);

        retranslateUi(MutiCamClass);

        QMetaObject::connectSlotsByName(MutiCamClass);
    } // setupUi

    void retranslateUi(QMainWindow *MutiCamClass)
    {
        MutiCamClass->setWindowTitle(QCoreApplication::translate("MutiCamClass", "MutiCam", nullptr));
        m_ButtonStartStop->setText(QCoreApplication::translate("MutiCamClass", "Start Image Recording ...", nullptr));
        m_LabelStream_1->setText(QString());
        m_ColorProcessingCheckBox->setText(QCoreApplication::translate("MutiCamClass", "ColorProcessing", nullptr));
        m_LabelStream_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MutiCamClass: public Ui_MutiCamClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUTICAM_H
