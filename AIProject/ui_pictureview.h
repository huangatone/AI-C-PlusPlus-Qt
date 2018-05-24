/********************************************************************************
** Form generated from reading UI file 'pictureview.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PICTUREVIEW_H
#define UI_PICTUREVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PictureView
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *btnPVZoomIn;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QToolButton *btnPVZoomOut;
    QToolButton *btnPVNO;
    QToolButton *btnPVZoom;
    QLabel *label;
    QSlider *horizontalSlider;

    void setupUi(QWidget *PictureView)
    {
        if (PictureView->objectName().isEmpty())
            PictureView->setObjectName(QStringLiteral("PictureView"));
        PictureView->resize(400, 300);
        gridLayout = new QGridLayout(PictureView);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(4, 4, 4, 4);
        horizontalSpacer = new QSpacerItem(177, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 4, 1, 1);

        btnPVZoomIn = new QToolButton(PictureView);
        btnPVZoomIn->setObjectName(QStringLiteral("btnPVZoomIn"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/zoomin.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPVZoomIn->setIcon(icon);
        btnPVZoomIn->setArrowType(Qt::NoArrow);

        gridLayout->addWidget(btnPVZoomIn, 3, 0, 1, 1);

        scrollArea = new QScrollArea(PictureView);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 390, 220));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 1, 0, 1, 5);

        btnPVZoomOut = new QToolButton(PictureView);
        btnPVZoomOut->setObjectName(QStringLiteral("btnPVZoomOut"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon/zoomout.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPVZoomOut->setIcon(icon1);
        btnPVZoomOut->setArrowType(Qt::NoArrow);

        gridLayout->addWidget(btnPVZoomOut, 3, 1, 1, 1);

        btnPVNO = new QToolButton(PictureView);
        btnPVNO->setObjectName(QStringLiteral("btnPVNO"));
        btnPVNO->setArrowType(Qt::NoArrow);

        gridLayout->addWidget(btnPVNO, 3, 3, 1, 1);

        btnPVZoom = new QToolButton(PictureView);
        btnPVZoom->setObjectName(QStringLiteral("btnPVZoom"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icon/nozoom.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPVZoom->setIcon(icon2);
        btnPVZoom->setArrowType(Qt::NoArrow);

        gridLayout->addWidget(btnPVZoom, 3, 2, 1, 1);

        label = new QLabel(PictureView);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 5);

        horizontalSlider = new QSlider(PictureView);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMinimum(-99);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 2, 0, 1, 5);


        retranslateUi(PictureView);

        QMetaObject::connectSlotsByName(PictureView);
    } // setupUi

    void retranslateUi(QWidget *PictureView)
    {
        PictureView->setWindowTitle(QApplication::translate("PictureView", "Form", Q_NULLPTR));
        btnPVZoomIn->setText(QApplication::translate("PictureView", "...", Q_NULLPTR));
        btnPVZoomOut->setText(QApplication::translate("PictureView", "...", Q_NULLPTR));
        btnPVNO->setText(QApplication::translate("PictureView", "...", Q_NULLPTR));
        btnPVZoom->setText(QApplication::translate("PictureView", "...", Q_NULLPTR));
        label->setText(QApplication::translate("PictureView", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PictureView: public Ui_PictureView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PICTUREVIEW_H
