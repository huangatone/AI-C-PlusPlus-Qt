/********************************************************************************
** Form generated from reading UI file 'cvpanel.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVPANEL_H
#define UI_CVPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "fileselectwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CVPanel
{
public:
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    QWidget *widget_3;
    QGridLayout *gridLayout_5;
    QWidget *widget_2;
    QGridLayout *gridLayout_7;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout;
    FileSelectWidget *fileWidget2;
    QPushButton *btnDo;
    FileSelectWidget *fileWidget1;
    FileSelectWidget *fileWidget3;
    FileSelectWidget *fileWidget4;
    QSlider *sliderB;
    QSlider *sliderC;
    QSlider *sliderD;
    QSpacerItem *verticalSpacer_3;
    QListWidget *functionList;
    QSlider *sliderA;
    QPushButton *pushButton;
    QWidget *page_2;
    QMdiArea *mdiArea;
    QTreeWidget *treeWidget;

    void setupUi(QWidget *CVPanel)
    {
        if (CVPanel->objectName().isEmpty())
            CVPanel->setObjectName(QStringLiteral("CVPanel"));
        CVPanel->resize(971, 520);
        gridLayout_2 = new QGridLayout(CVPanel);
        gridLayout_2->setSpacing(1);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(1, 1, 1, 1);
        splitter = new QSplitter(CVPanel);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        widget_3 = new QWidget(splitter);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(80, 0));
        gridLayout_5 = new QGridLayout(widget_3);
        gridLayout_5->setSpacing(1);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(1, 1, 1, 1);
        widget_2 = new QWidget(widget_3);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(180, 0));
        gridLayout_7 = new QGridLayout(widget_2);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(2, 2, 2, 2);
        stackedWidget = new QStackedWidget(widget_2);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setMinimumSize(QSize(0, 150));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        gridLayout = new QGridLayout(page);
        gridLayout->setSpacing(1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        fileWidget2 = new FileSelectWidget(page);
        fileWidget2->setObjectName(QStringLiteral("fileWidget2"));
        fileWidget2->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(fileWidget2, 3, 0, 1, 1);

        btnDo = new QPushButton(page);
        btnDo->setObjectName(QStringLiteral("btnDo"));

        gridLayout->addWidget(btnDo, 10, 0, 1, 1);

        fileWidget1 = new FileSelectWidget(page);
        fileWidget1->setObjectName(QStringLiteral("fileWidget1"));
        fileWidget1->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(fileWidget1, 2, 0, 1, 1);

        fileWidget3 = new FileSelectWidget(page);
        fileWidget3->setObjectName(QStringLiteral("fileWidget3"));
        fileWidget3->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(fileWidget3, 4, 0, 1, 1);

        fileWidget4 = new FileSelectWidget(page);
        fileWidget4->setObjectName(QStringLiteral("fileWidget4"));
        fileWidget4->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(fileWidget4, 5, 0, 1, 1);

        sliderB = new QSlider(page);
        sliderB->setObjectName(QStringLiteral("sliderB"));
        sliderB->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderB, 7, 0, 1, 1);

        sliderC = new QSlider(page);
        sliderC->setObjectName(QStringLiteral("sliderC"));
        sliderC->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderC, 8, 0, 1, 1);

        sliderD = new QSlider(page);
        sliderD->setObjectName(QStringLiteral("sliderD"));
        sliderD->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderD, 9, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 12, 0, 1, 1);

        functionList = new QListWidget(page);
        functionList->setObjectName(QStringLiteral("functionList"));
        functionList->setMouseTracking(true);
        functionList->setFrameShape(QFrame::Box);
        functionList->setFrameShadow(QFrame::Raised);
        functionList->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        functionList->setProperty("showDropIndicator", QVariant(false));
        functionList->setDragDropMode(QAbstractItemView::DragDrop);
        functionList->setIconSize(QSize(48, 48));
        functionList->setResizeMode(QListView::Adjust);
        functionList->setSpacing(1);
        functionList->setViewMode(QListView::IconMode);

        gridLayout->addWidget(functionList, 1, 0, 1, 1);

        sliderA = new QSlider(page);
        sliderA->setObjectName(QStringLiteral("sliderA"));
        sliderA->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderA, 6, 0, 1, 1);

        pushButton = new QPushButton(page);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 11, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        stackedWidget->addWidget(page_2);

        gridLayout_7->addWidget(stackedWidget, 1, 0, 1, 2);


        gridLayout_5->addWidget(widget_2, 0, 0, 1, 1);

        splitter->addWidget(widget_3);
        mdiArea = new QMdiArea(splitter);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));
        splitter->addWidget(mdiArea);
        treeWidget = new QTreeWidget(splitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setDragEnabled(true);
        splitter->addWidget(treeWidget);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(CVPanel);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CVPanel);
    } // setupUi

    void retranslateUi(QWidget *CVPanel)
    {
        CVPanel->setWindowTitle(QApplication::translate("CVPanel", "Form", Q_NULLPTR));
        btnDo->setText(QApplication::translate("CVPanel", "Process", Q_NULLPTR));
        pushButton->setText(QApplication::translate("CVPanel", "Open Browser", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CVPanel: public Ui_CVPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVPANEL_H
