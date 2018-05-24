/********************************************************************************
** Form generated from reading UI file 'stylewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STYLEWINDOW_H
#define UI_STYLEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "cvpanel.h"
#include "iconbutton.h"

QT_BEGIN_NAMESPACE

class Ui_StyleWindow
{
public:
    QAction *actionOpen;
    QAction *actionClose;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    IconButton *iconbtn1;
    IconButton *iconbtn2;
    IconButton *iconbtn5;
    IconButton *iconbtn3;
    IconButton *iconbtn4;
    QFrame *line;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QToolButton *toolButton_5;
    QLabel *label_5;
    QToolButton *toolButton_8;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_5;
    CVPanel *widget_2;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuAction;
    QMenu *menuWindows;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *StyleWindow)
    {
        if (StyleWindow->objectName().isEmpty())
            StyleWindow->setObjectName(QStringLiteral("StyleWindow"));
        StyleWindow->resize(1015, 618);
        actionOpen = new QAction(StyleWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionClose = new QAction(StyleWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        centralwidget = new QWidget(StyleWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(80, 0));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(0, 250));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(2);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(2, 2, 2, 2);
        iconbtn1 = new IconButton(groupBox);
        iconbtn1->setObjectName(QStringLiteral("iconbtn1"));

        gridLayout_4->addWidget(iconbtn1, 0, 0, 1, 1);

        iconbtn2 = new IconButton(groupBox);
        iconbtn2->setObjectName(QStringLiteral("iconbtn2"));

        gridLayout_4->addWidget(iconbtn2, 1, 0, 1, 1);

        iconbtn5 = new IconButton(groupBox);
        iconbtn5->setObjectName(QStringLiteral("iconbtn5"));

        gridLayout_4->addWidget(iconbtn5, 4, 0, 1, 1);

        iconbtn3 = new IconButton(groupBox);
        iconbtn3->setObjectName(QStringLiteral("iconbtn3"));

        gridLayout_4->addWidget(iconbtn3, 2, 0, 1, 1);

        iconbtn4 = new IconButton(groupBox);
        iconbtn4->setObjectName(QStringLiteral("iconbtn4"));

        gridLayout_4->addWidget(iconbtn4, 3, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        line = new QFrame(widget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_6, 4, 0, 1, 1);

        toolButton_5 = new QToolButton(groupBox_2);
        toolButton_5->setObjectName(QStringLiteral("toolButton_5"));

        gridLayout_3->addWidget(toolButton_5, 1, 0, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_5, 2, 0, 1, 1);

        toolButton_8 = new QToolButton(groupBox_2);
        toolButton_8->setObjectName(QStringLiteral("toolButton_8"));

        gridLayout_3->addWidget(toolButton_8, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 2, 0, 1, 1);


        gridLayout->addWidget(widget, 0, 0, 1, 1);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        gridLayout_5 = new QGridLayout(page);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        widget_2 = new CVPanel(page);
        widget_2->setObjectName(QStringLiteral("widget_2"));

        gridLayout_5->addWidget(widget_2, 0, 0, 1, 1);

        stackedWidget->addWidget(page);

        gridLayout->addWidget(stackedWidget, 0, 1, 1, 1);

        StyleWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(StyleWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1015, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuAction = new QMenu(menubar);
        menuAction->setObjectName(QStringLiteral("menuAction"));
        menuWindows = new QMenu(menubar);
        menuWindows->setObjectName(QStringLiteral("menuWindows"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        StyleWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(StyleWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        StyleWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(StyleWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMovable(false);
        StyleWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuAction->menuAction());
        menubar->addAction(menuWindows->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionClose);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionClose);

        retranslateUi(StyleWindow);

        QMetaObject::connectSlotsByName(StyleWindow);
    } // setupUi

    void retranslateUi(QMainWindow *StyleWindow)
    {
        StyleWindow->setWindowTitle(QApplication::translate("StyleWindow", "MainWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("StyleWindow", "Open", Q_NULLPTR));
        actionClose->setText(QApplication::translate("StyleWindow", "Close", Q_NULLPTR));
        groupBox->setTitle(QString());
        groupBox_2->setTitle(QString());
        label_6->setText(QApplication::translate("StyleWindow", "back", Q_NULLPTR));
        toolButton_5->setText(QApplication::translate("StyleWindow", "...", Q_NULLPTR));
        label_5->setText(QApplication::translate("StyleWindow", "Go", Q_NULLPTR));
        toolButton_8->setText(QApplication::translate("StyleWindow", "...", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("StyleWindow", "File", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("StyleWindow", "Edit", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("StyleWindow", "View", Q_NULLPTR));
        menuAction->setTitle(QApplication::translate("StyleWindow", "Action", Q_NULLPTR));
        menuWindows->setTitle(QApplication::translate("StyleWindow", "Windows", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("StyleWindow", "Help", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("StyleWindow", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class StyleWindow: public Ui_StyleWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STYLEWINDOW_H
