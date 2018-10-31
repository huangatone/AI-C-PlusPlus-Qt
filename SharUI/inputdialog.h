#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QString>

#include <QLabel>
#include <QDebug>
#include <QSpacerItem>
#include <QListWidget>
#include "ui_inputdialog.h"

using namespace std;

namespace Ui {
class InputDialog;
}
class QListWidget;


class CustomInputDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CustomInputDialog(QWidget *parent = 0);

	~CustomInputDialog();

	void addWidget(QString lbs, QWidget* w);
	void addWidget(QWidget* l, QWidget* r);


private:
	Ui::InputDialog *ui;

	QGridLayout* _layout;
	int index;

	void endAdd();

	// QDialog interface
public slots:
	int exec();
};


template< class Type,int num >
class InputDialog : public QDialog
{

public:
	explicit InputDialog(QWidget *parent = 0) :
		QDialog(parent),
		ui(new Ui::InputDialog)
	{
		ui->setupUi(this);
		_layout = new QGridLayout(this);
		setLayout(_layout);
		setWindowTitle( tr("Please input datas"));

		int n=0;
		for(int mm =0; mm < num; mm ++)
		{
			Type* widget= new Type();
			QLabel* lbl = new QLabel();
			lbl->setAlignment(Qt::AlignTop);
			_layout->addWidget(lbl, n,0);
			_layout->addWidget(widget, n,1);

			_ltWidget.append(widget);
			_ltLabel.append(lbl);
			n++;
		}

		_layout->addItem( new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Expanding) ,n,0);
		n++;
		_layout->addWidget( ui->buttonBox,n,0,1,2);
	}

	void setLabels(QStringList lbs)
	{
		for(int n=0; n< lbs.count(); n++)
		{
			if(n >= _ltLabel.count())
				break;
			_ltLabel[n]->setText(lbs[n]);
		}
	}


	Type* getWidget(int n)
	{
		return _ltWidget[n];
	}

	~InputDialog()
	{

	}


private:
	Ui::InputDialog *ui;
	QList<Type*> _ltWidget;
	QGridLayout* _layout;
	QList<QLabel*> _ltLabel;
};



template< class Type1,int num1 , class Type2,int num2>
class MultipleInputDialog : public QDialog
{

public:
	explicit MultipleInputDialog(QWidget *parent = 0) :
		QDialog(parent),
		ui(new Ui::InputDialog)
	{
		ui->setupUi(this);
		_layout = new QGridLayout(this);
		setLayout(_layout);
		setWindowTitle( tr("Please input datas"));

		int n=0;
		for(int mm =0; mm < num1; mm ++)
		{
			Type1* widget= new Type1();
			QLabel* lbl = new QLabel();
			lbl->setAlignment(Qt::AlignTop);
			_layout->addWidget(lbl, n,0);
			_layout->addWidget(widget, n,1);

			_ltWidget.append(widget);
			_ltLabel.append(lbl);
			n++;
		}

		for(int mm =0; mm < num2; mm ++)
		{
			Type2* widget= new Type2();
			QLabel* lbl = new QLabel();
			lbl->setAlignment(Qt::AlignTop);
			_layout->addWidget(lbl, n,0);
			_layout->addWidget(widget, n,1);

			_ltWidget2.append(widget);
			_ltLabel.append(lbl);
			n++;
		}

		_layout->addItem( new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Expanding) ,n,0);
		n++;
		_layout->addWidget( ui->buttonBox,n,0,1,2);
	}

	void setLabels(QStringList lbs)
	{
		for(int n=0; n< lbs.count(); n++)
		{
			if(n >= _ltLabel.count())
				break;
			_ltLabel[n]->setText(lbs[n]);
		}
	}


	Type1* getType1Widget(int n)
	{
		return _ltWidget[n];
	}
	Type2* getType2Widget(int n)
	{
		return _ltWidget2[n];
	}

	~MultipleInputDialog()
	{

	}


private:
	Ui::InputDialog *ui;
	QList<Type1*> _ltWidget;
	QList<Type2*> _ltWidget2;
	QGridLayout* _layout;
	QList<QLabel*> _ltLabel;
};



template< class Type1, class Type2,int num>
class MultInputDialog : public QDialog
{

public:
	explicit MultInputDialog(QWidget *parent = 0) :
		QDialog(parent),
		ui(new Ui_InputDialog)
	{
		ui->setupUi(this);
		_layout = new QGridLayout(this);
		setLayout(_layout);
		setWindowTitle( tr("Please input datas"));

		int n=0;
		for(int mm =0; mm < num; mm ++)
		{
			Type1* widget= new Type1();
			QLabel* lbl = new QLabel();
			lbl->setAlignment(Qt::AlignTop);
			_layout->addWidget(lbl, n,0);
			_layout->addWidget(widget, n,1);

			_ltWidget.append(widget);
			_ltLabel.append(lbl);
			n++;

			Type2* widget2= new Type2();
			QLabel* lbl2 = new QLabel();
			lbl2->setAlignment(Qt::AlignTop);
			_layout->addWidget(lbl2, n,0);
			_layout->addWidget(widget2, n,1);

			_ltWidget2.append(widget2);
			_ltLabel.append(lbl2);
			n++;
		}

		_layout->addItem( new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Expanding) ,n,0);
		n++;
		_layout->addWidget( ui->buttonBox,n,0,1,2);
	}

	void setLabels(QStringList lbs)
	{
		for(int n=0; n< lbs.count(); n++)
		{
			if(n >= _ltLabel.count())
				break;
			_ltLabel[n]->setText(lbs[n]);
		}
	}


	Type1* getType1Widget(int n)
	{
		return _ltWidget[n];
	}
	Type2* getType2Widget(int n)
	{
		return _ltWidget2[n];
	}

	~MultInputDialog()
	{

	}


private:
	Ui_InputDialog *ui;
	QList<Type1*> _ltWidget;
	QList<Type2*> _ltWidget2;
	QGridLayout* _layout;
	QList<QLabel*> _ltLabel;
};

#endif // INPUTDIALOG_H
