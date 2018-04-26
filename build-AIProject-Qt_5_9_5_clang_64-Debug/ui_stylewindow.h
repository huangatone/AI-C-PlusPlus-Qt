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
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "dfsplitter.h"
#include "fileselectwidget.h"
#include "iconbutton.h"

QT_BEGIN_NAMESPACE

class Ui_StyleWindow
{
public:
    QAction *actionOpen;
    QAction *actionClose;
    QWidget *centralwidget;
    QGridLayout *gridLayout_6;
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
    DFSplitter *splitter;
    QWidget *widget_3;
    QGridLayout *gridLayout_5;
    QTreeWidget *treeWidget;
    QMdiArea *mdiArea;
    QWidget *widget_2;
    QGridLayout *gridLayout_7;
    QPushButton *btnDo;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout;
    FileSelectWidget *fileWidget1;
    FileSelectWidget *fileWidgetw;
    QSlider *horizontalSlider_3;
    QSlider *horizontalSlider;
    FileSelectWidget *fileWidget3;
    QSpacerItem *verticalSpacer_3;
    FileSelectWidget *fileWidget4;
    QSlider *horizontalSlider_2;
    QSlider *horizontalSlider_4;
    QWidget *page_2;
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
        gridLayout_6 = new QGridLayout(centralwidget);
        gridLayout_6->setSpacing(1);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(1, 1, 1, 1);
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


        gridLayout_6->addWidget(widget, 0, 0, 1, 1);

        splitter = new DFSplitter(centralwidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        widget_3 = new QWidget(splitter);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(80, 0));
        gridLayout_5 = new QGridLayout(widget_3);
        gridLayout_5->setSpacing(1);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(1, 1, 1, 1);
        treeWidget = new QTreeWidget(widget_3);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setDragEnabled(true);

        gridLayout_5->addWidget(treeWidget, 0, 0, 1, 1);

        splitter->addWidget(widget_3);
        mdiArea = new QMdiArea(splitter);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));
        splitter->addWidget(mdiArea);
        widget_2 = new QWidget(splitter);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(100, 0));
        gridLayout_7 = new QGridLayout(widget_2);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(2, 2, 2, 2);
        btnDo = new QPushButton(widget_2);
        btnDo->setObjectName(QStringLiteral("btnDo"));

        gridLayout_7->addWidget(btnDo, 0, 0, 1, 2);

        stackedWidget = new QStackedWidget(widget_2);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setMinimumSize(QSize(0, 150));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        gridLayout = new QGridLayout(page);
        gridLayout->setSpacing(1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        fileWidget1 = new FileSelectWidget(page);
        fileWidget1->setObjectName(QStringLiteral("fileWidget1"));
        fileWidget1->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(fileWidget1, 0, 0, 1, 1);

        fileWidgetw = new FileSelectWidget(page);
        fileWidgetw->setObjectName(QStringLiteral("fileWidgetw"));
        fileWidgetw->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(fileWidgetw, 1, 0, 1, 1);

        horizontalSlider_3 = new QSlider(page);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_3, 6, 0, 1, 1);

        horizontalSlider = new QSlider(page);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 4, 0, 1, 1);

        fileWidget3 = new FileSelectWidget(page);
        fileWidget3->setObjectName(QStringLiteral("fileWidget3"));
        fileWidget3->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(fileWidget3, 2, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 8, 0, 1, 1);

        fileWidget4 = new FileSelectWidget(page);
        fileWidget4->setObjectName(QStringLiteral("fileWidget4"));
        fileWidget4->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(fileWidget4, 3, 0, 1, 1);

        horizontalSlider_2 = new QSlider(page);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_2, 5, 0, 1, 1);

        horizontalSlider_4 = new QSlider(page);
        horizontalSlider_4->setObjectName(QStringLiteral("horizontalSlider_4"));
        horizontalSlider_4->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_4, 7, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        stackedWidget->addWidget(page_2);

        gridLayout_7->addWidget(stackedWidget, 1, 0, 1, 2);

        splitter->addWidget(widget_2);

        gridLayout_6->addWidget(splitter, 0, 1, 1, 1);

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
        btnDo->setText(QApplication::translate("StyleWindow", "Work", Q_NULLPTR));
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
