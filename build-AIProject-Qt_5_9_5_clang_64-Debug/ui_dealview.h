/********************************************************************************
** Form generated from reading UI file 'dealview.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEALVIEW_H
#define UI_DEALVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "cvpictureview.h"
#include "rgbeditor.h"

QT_BEGIN_NAMESPACE

class Ui_DealView
{
public:
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    CVPictureView *cvpictureWidget;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QToolButton *btnReset;
    QSlider *sliderB;
    QToolButton *btnAdd;
    QLabel *label;
    QSlider *sliderC;
    QLabel *label_3;
    QSlider *sliderD;
    QToolButton *btnRun;
    QToolButton *btnAddFromTree;
    QLabel *label_2;
    RGBEditor *widget;
    QToolButton *btnRefresh;
    QSlider *sliderA;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QToolButton *btnDel;
    QListWidget *listWidget;
    QTextEdit *textEdit;

    void setupUi(QWidget *DealView)
    {
        if (DealView->objectName().isEmpty())
            DealView->setObjectName(QStringLiteral("DealView"));
        DealView->resize(785, 438);
        gridLayout_2 = new QGridLayout(DealView);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        splitter = new QSplitter(DealView);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        cvpictureWidget = new CVPictureView(splitter);
        cvpictureWidget->setObjectName(QStringLiteral("cvpictureWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cvpictureWidget->sizePolicy().hasHeightForWidth());
        cvpictureWidget->setSizePolicy(sizePolicy);
        cvpictureWidget->setMinimumSize(QSize(200, 0));
        splitter->addWidget(cvpictureWidget);
        groupBox_2 = new QGroupBox(splitter);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 6, 1, 1);

        btnReset = new QToolButton(groupBox_2);
        btnReset->setObjectName(QStringLiteral("btnReset"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/update.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnReset->setIcon(icon);

        gridLayout->addWidget(btnReset, 8, 4, 1, 1);

        sliderB = new QSlider(groupBox_2);
        sliderB->setObjectName(QStringLiteral("sliderB"));
        sliderB->setMinimum(1);
        sliderB->setMaximum(100);
        sliderB->setValue(50);
        sliderB->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderB, 1, 7, 1, 1);

        btnAdd = new QToolButton(groupBox_2);
        btnAdd->setObjectName(QStringLiteral("btnAdd"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon/file_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAdd->setIcon(icon1);

        gridLayout->addWidget(btnAdd, 8, 1, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label, 0, 6, 1, 1);

        sliderC = new QSlider(groupBox_2);
        sliderC->setObjectName(QStringLiteral("sliderC"));
        sliderC->setMaximum(10);
        sliderC->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderC, 2, 7, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 6, 1, 1);

        sliderD = new QSlider(groupBox_2);
        sliderD->setObjectName(QStringLiteral("sliderD"));
        sliderD->setMaximum(10);
        sliderD->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderD, 4, 7, 1, 1);

        btnRun = new QToolButton(groupBox_2);
        btnRun->setObjectName(QStringLiteral("btnRun"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icon/run_opencv.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRun->setIcon(icon2);

        gridLayout->addWidget(btnRun, 8, 9, 1, 1);

        btnAddFromTree = new QToolButton(groupBox_2);
        btnAddFromTree->setObjectName(QStringLiteral("btnAddFromTree"));
        btnAddFromTree->setArrowType(Qt::NoArrow);

        gridLayout->addWidget(btnAddFromTree, 8, 0, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 6, 1, 1);

        widget = new RGBEditor(groupBox_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(widget, 5, 7, 1, 1);

        btnRefresh = new QToolButton(groupBox_2);
        btnRefresh->setObjectName(QStringLiteral("btnRefresh"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icon/trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRefresh->setIcon(icon3);

        gridLayout->addWidget(btnRefresh, 8, 3, 1, 1);

        sliderA = new QSlider(groupBox_2);
        sliderA->setObjectName(QStringLiteral("sliderA"));
        sliderA->setMinimumSize(QSize(150, 0));
        sliderA->setMinimum(1);
        sliderA->setMaximum(100);
        sliderA->setValue(50);
        sliderA->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderA, 0, 7, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 7, 7, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 8, 5, 1, 4);

        btnDel = new QToolButton(groupBox_2);
        btnDel->setObjectName(QStringLiteral("btnDel"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icon/file_del.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDel->setIcon(icon4);

        gridLayout->addWidget(btnDel, 8, 2, 1, 1);

        listWidget = new QListWidget(groupBox_2);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy3);
        listWidget->setMinimumSize(QSize(240, 0));

        gridLayout->addWidget(listWidget, 0, 0, 8, 6);

        textEdit = new QTextEdit(groupBox_2);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        gridLayout->addWidget(textEdit, 6, 7, 1, 1);

        splitter->addWidget(groupBox_2);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(DealView);

        QMetaObject::connectSlotsByName(DealView);
    } // setupUi

    void retranslateUi(QWidget *DealView)
    {
        DealView->setWindowTitle(QApplication::translate("DealView", "Form", Q_NULLPTR));
        groupBox_2->setTitle(QString());
        label_4->setText(QApplication::translate("DealView", "D", Q_NULLPTR));
        btnReset->setText(QApplication::translate("DealView", "...", Q_NULLPTR));
        btnAdd->setText(QApplication::translate("DealView", "...", Q_NULLPTR));
        label->setText(QApplication::translate("DealView", "A", Q_NULLPTR));
        label_3->setText(QApplication::translate("DealView", "C", Q_NULLPTR));
        btnRun->setText(QApplication::translate("DealView", "...", Q_NULLPTR));
        btnAddFromTree->setText(QApplication::translate("DealView", "...", Q_NULLPTR));
        label_2->setText(QApplication::translate("DealView", "B", Q_NULLPTR));
        btnRefresh->setText(QApplication::translate("DealView", "...", Q_NULLPTR));
        btnDel->setText(QApplication::translate("DealView", "...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DealView: public Ui_DealView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEALVIEW_H
