/********************************************************************************
** Form generated from reading UI file 'iconbutton.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICONBUTTON_H
#define UI_ICONBUTTON_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IconButton
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label;

    void setupUi(QWidget *IconButton)
    {
        if (IconButton->objectName().isEmpty())
            IconButton->setObjectName(QStringLiteral("IconButton"));
        IconButton->resize(81, 115);
        gridLayout = new QGridLayout(IconButton);
        gridLayout->setSpacing(1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(1, 1, 1, 1);
        label_2 = new QLabel(IconButton);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(80, 16777215));
        label_2->setTextFormat(Qt::RichText);
        label_2->setAlignment(Qt::AlignCenter);
        label_2->setWordWrap(true);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(IconButton);
        label->setObjectName(QStringLiteral("label"));
        label->setPixmap(QPixmap(QString::fromUtf8("../../../images/pic/linux.png")));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(IconButton);

        QMetaObject::connectSlotsByName(IconButton);
    } // setupUi

    void retranslateUi(QWidget *IconButton)
    {
        IconButton->setWindowTitle(QApplication::translate("IconButton", "Form", Q_NULLPTR));
        label_2->setText(QApplication::translate("IconButton", "TextLabel", Q_NULLPTR));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class IconButton: public Ui_IconButton {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICONBUTTON_H
