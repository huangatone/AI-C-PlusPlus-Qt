/********************************************************************************
** Form generated from reading UI file 'cvpictureview.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVPICTUREVIEW_H
#define UI_CVPICTUREVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>
#include "pictureview.h"

QT_BEGIN_NAMESPACE

class Ui_CVPictureView
{
public:
    QGridLayout *gridLayout_3;
    QSplitter *splitter;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QToolButton *btnPath;
    QTreeView *treeView;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    PictureView *widget;
    QToolButton *btnNext;
    QToolButton *btnPre;
    QLabel *label;

    void setupUi(QWidget *CVPictureView)
    {
        if (CVPictureView->objectName().isEmpty())
            CVPictureView->setObjectName(QStringLiteral("CVPictureView"));
        CVPictureView->resize(608, 462);
        gridLayout_3 = new QGridLayout(CVPictureView);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        splitter = new QSplitter(CVPictureView);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        frame = new QFrame(splitter);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        btnPath = new QToolButton(frame);
        btnPath->setObjectName(QStringLiteral("btnPath"));

        gridLayout_2->addWidget(btnPath, 0, 1, 1, 1);

        treeView = new QTreeView(frame);
        treeView->setObjectName(QStringLiteral("treeView"));

        gridLayout_2->addWidget(treeView, 1, 0, 1, 2);

        splitter->addWidget(frame);
        groupBox_2 = new QGroupBox(splitter);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        widget = new PictureView(groupBox_2);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);

        gridLayout->addWidget(widget, 0, 0, 1, 5);

        btnNext = new QToolButton(groupBox_2);
        btnNext->setObjectName(QStringLiteral("btnNext"));
        btnNext->setArrowType(Qt::RightArrow);

        gridLayout->addWidget(btnNext, 2, 4, 1, 1);

        btnPre = new QToolButton(groupBox_2);
        btnPre->setObjectName(QStringLiteral("btnPre"));
        btnPre->setArrowType(Qt::LeftArrow);

        gridLayout->addWidget(btnPre, 2, 0, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 2, 1, 1, 3);

        splitter->addWidget(groupBox_2);

        gridLayout_3->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(CVPictureView);

        QMetaObject::connectSlotsByName(CVPictureView);
    } // setupUi

    void retranslateUi(QWidget *CVPictureView)
    {
        CVPictureView->setWindowTitle(QApplication::translate("CVPictureView", "Form", Q_NULLPTR));
        label_2->setText(QApplication::translate("CVPictureView", "TextLabel", Q_NULLPTR));
        btnPath->setText(QApplication::translate("CVPictureView", "...", Q_NULLPTR));
        groupBox_2->setTitle(QString());
        btnNext->setText(QApplication::translate("CVPictureView", "...", Q_NULLPTR));
        btnPre->setText(QApplication::translate("CVPictureView", "...", Q_NULLPTR));
        label->setText(QApplication::translate("CVPictureView", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CVPictureView: public Ui_CVPictureView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVPICTUREVIEW_H
