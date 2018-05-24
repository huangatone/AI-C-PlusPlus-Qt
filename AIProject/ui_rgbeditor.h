/********************************************************************************
** Form generated from reading UI file 'rgbeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RGBEDITOR_H
#define UI_RGBEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RGBEditor
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *btn;
    QLineEdit *LERGB;

    void setupUi(QWidget *RGBEditor)
    {
        if (RGBEditor->objectName().isEmpty())
            RGBEditor->setObjectName(QStringLiteral("RGBEditor"));
        RGBEditor->resize(185, 30);
        gridLayout = new QGridLayout(RGBEditor);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);

        btn = new QToolButton(RGBEditor);
        btn->setObjectName(QStringLiteral("btn"));
        btn->setMaximumSize(QSize(24, 24));

        gridLayout->addWidget(btn, 0, 2, 1, 1);

        LERGB = new QLineEdit(RGBEditor);
        LERGB->setObjectName(QStringLiteral("LERGB"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LERGB->sizePolicy().hasHeightForWidth());
        LERGB->setSizePolicy(sizePolicy);
        LERGB->setMinimumSize(QSize(50, 0));
        LERGB->setMaxLength(32767);

        gridLayout->addWidget(LERGB, 0, 1, 1, 1);


        retranslateUi(RGBEditor);

        QMetaObject::connectSlotsByName(RGBEditor);
    } // setupUi

    void retranslateUi(QWidget *RGBEditor)
    {
        RGBEditor->setWindowTitle(QApplication::translate("RGBEditor", "Form", Q_NULLPTR));
        btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RGBEditor: public Ui_RGBEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RGBEDITOR_H
