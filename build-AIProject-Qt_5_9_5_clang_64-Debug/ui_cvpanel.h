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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "fileselectwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CVPanel
{
public:
    QGridLayout *gridLayout;
    QToolButton *toolButton;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_2;
    FileSelectWidget *widget;
    FileSelectWidget *widget_2;
    QSpacerItem *verticalSpacer;
    QWidget *page_2;

    void setupUi(QWidget *CVPanel)
    {
        if (CVPanel->objectName().isEmpty())
            CVPanel->setObjectName(QStringLiteral("CVPanel"));
        CVPanel->resize(400, 300);
        gridLayout = new QGridLayout(CVPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        toolButton = new QToolButton(CVPanel);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        gridLayout->addWidget(toolButton, 0, 0, 1, 1);

        stackedWidget = new QStackedWidget(CVPanel);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        gridLayout_2 = new QGridLayout(page);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widget = new FileSelectWidget(page);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 28));

        gridLayout_2->addWidget(widget, 0, 0, 1, 1);

        widget_2 = new FileSelectWidget(page);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(0, 28));

        gridLayout_2->addWidget(widget_2, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 141, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 2, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 1, 0, 1, 1);


        retranslateUi(CVPanel);

        QMetaObject::connectSlotsByName(CVPanel);
    } // setupUi

    void retranslateUi(QWidget *CVPanel)
    {
        CVPanel->setWindowTitle(QApplication::translate("CVPanel", "Form", Q_NULLPTR));
        toolButton->setText(QApplication::translate("CVPanel", "...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CVPanel: public Ui_CVPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVPANEL_H
