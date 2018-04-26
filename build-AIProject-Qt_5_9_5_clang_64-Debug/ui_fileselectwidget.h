/********************************************************************************
** Form generated from reading UI file 'fileselectwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILESELECTWIDGET_H
#define UI_FILESELECTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileSelectWidget
{
public:
    QGridLayout *gridLayout;
    QToolButton *btnPreview;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
    QLabel *label;
    QToolButton *btnFromWidget;

    void setupUi(QWidget *FileSelectWidget)
    {
        if (FileSelectWidget->objectName().isEmpty())
            FileSelectWidget->setObjectName(QStringLiteral("FileSelectWidget"));
        FileSelectWidget->resize(342, 81);
        gridLayout = new QGridLayout(FileSelectWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnPreview = new QToolButton(FileSelectWidget);
        btnPreview->setObjectName(QStringLiteral("btnPreview"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/preview.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPreview->setIcon(icon);

        gridLayout->addWidget(btnPreview, 1, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        lineEdit = new QLineEdit(FileSelectWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setDragEnabled(true);
        lineEdit->setReadOnly(false);

        gridLayout->addWidget(lineEdit, 0, 1, 1, 5);

        toolButton = new QToolButton(FileSelectWidget);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        gridLayout->addWidget(toolButton, 1, 5, 1, 1);

        label = new QLabel(FileSelectWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        btnFromWidget = new QToolButton(FileSelectWidget);
        btnFromWidget->setObjectName(QStringLiteral("btnFromWidget"));
        btnFromWidget->setArrowType(Qt::RightArrow);

        gridLayout->addWidget(btnFromWidget, 1, 3, 1, 1);


        retranslateUi(FileSelectWidget);

        QMetaObject::connectSlotsByName(FileSelectWidget);
    } // setupUi

    void retranslateUi(QWidget *FileSelectWidget)
    {
        FileSelectWidget->setWindowTitle(QApplication::translate("FileSelectWidget", "Form", Q_NULLPTR));
        btnPreview->setText(QApplication::translate("FileSelectWidget", "...", Q_NULLPTR));
        toolButton->setText(QApplication::translate("FileSelectWidget", "...", Q_NULLPTR));
        label->setText(QApplication::translate("FileSelectWidget", "File:", Q_NULLPTR));
        btnFromWidget->setText(QApplication::translate("FileSelectWidget", "...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FileSelectWidget: public Ui_FileSelectWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILESELECTWIDGET_H
