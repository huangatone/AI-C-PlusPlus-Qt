#include "stylewindow.h"
#include "ui_stylewindow.h"
#include <QComboBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QMdiSubWindow>


#include <QImage>
using namespace std;


StyleWindow::StyleWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::StyleWindow)
{
	ui->setupUi(this);
	ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

	auto co = new QComboBox(this);
	co->addItems( QStringList() << "Pro 1" << "Pro 2" << "Pro 3");

	ui->toolBar->addWidget( co);

	QWidget* blank_w = new QWidget (this);
	blank_w->resize(50,20);
	blank_w->setStyleSheet("background-color: rgba(33, 90, 29, 255);");
	blank_w->setMinimumWidth(50);
	ui->toolBar->insertWidget( ui->actionOpen,blank_w);

	connect( ui->iconbtn1, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));
	connect( ui->iconbtn2, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));
	connect( ui->iconbtn3, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));
	connect( ui->iconbtn4, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));
	connect( ui->iconbtn5, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));



	QFile f(":/opencv.json");
	if(f.open(QIODevice::ReadOnly))
	{
		initWidgets( f.readAll());
	}

	/*DealView *pView = new DealView (this);
	auto a = ui->mdiArea->addSubWindow(pView);
	a->setWindowTitle("Test View");
	pView->show();
*/


}

StyleWindow::~StyleWindow()
{
	delete ui;
}

void StyleWindow::initWidgets(QString str)
{
	//qDebug() << "init";
	QJsonDocument doc = QJsonDocument::fromJson( str.toUtf8());
	auto obj = doc.object();
	auto btn_objs = obj["buttons"];
	auto btn_arr = btn_objs.toArray();
	QList<IconButton*> lt_iconbtn;
	lt_iconbtn << ui->iconbtn1 << ui->iconbtn2 <<ui->iconbtn3<<ui->iconbtn4<< ui->iconbtn5;

	for(int i=0; i< btn_arr.count(); i++)
	{
		auto sub_obj =  btn_arr[i].toObject();
		//qDebug() << sub_obj.value("text").toString() << sub_obj.value("icon").toString();

		lt_iconbtn[i]->SetText(	sub_obj.value("text").toString());
		lt_iconbtn[i]->SetIcon( QPixmap(sub_obj.value("icon").toString()));

	}

	auto tree_objs = obj["trees"];

	auto tree_arr = tree_objs.toArray();

	auto colors = obj["colors"].toObject();
	QString clr = "%1 background-color: rgba(%2);color: rgb(%3);";

	QString bk = colors.value("background").toString();
	QString fk =colors.value("forecolor").toString();
	QString style_str = clr.arg("QMainWindow").arg(bk).arg(fk);
	style_str += clr.arg("QPushButton").arg(bk).arg(fk);
	style_str += clr.arg("QToolBar").arg(bk).arg(fk);
	style_str += clr.arg("QMenuBar").arg(bk).arg(fk);
	style_str += clr.arg("QLabel").arg(bk).arg(fk);
	style_str += clr.arg("QToolButton").arg(bk).arg(fk);

	QString single_style =  clr.arg("").arg(bk).arg(fk);
	//qDebug() << single_style;
	ui->widget->setStyleSheet(single_style);
	ui->toolBar->setStyleSheet(single_style+tr("border-color: rgb(%1);").arg(bk));
	ui->statusbar->setStyleSheet(single_style);
	ui->iconbtn1->setStyleSheet(single_style);
	ui->iconbtn2->setStyleSheet(single_style);
	ui->iconbtn3->setStyleSheet(single_style);
	ui->iconbtn4->setStyleSheet(single_style);
	ui->iconbtn5->setStyleSheet(single_style);

	//ui->widget_2->setStyleSheet(single_style);

	//qDebug() << single_style;
	//qApp->setStyleSheet(style_str);

}

void StyleWindow::addSubWindow(QWidget *w)
{
	ui->stackedWidget->addWidget(w);
}

void StyleWindow::setCurrentWidget(int index)
{
	ui->stackedWidget->setCurrentIndex(index);
}

void StyleWindow::setCurrentWidget(QWidget *w)
{
	ui->stackedWidget->setCurrentWidget(w);
}

void StyleWindow::slot_icon_buttun_clicked()
{
	IconButton* btn = qobject_cast<IconButton*>( sender());
	//if(btn)
	//	qDebug() << btn->getText() << "  Clicked ";
	if(btn == ui->iconbtn1)
	{
		ui->stackedWidget->setCurrentIndex(0);
		/*DealView *pView = new DealView (this);
		auto a = ui->mdiArea->addSubWindow(pView);
		a->setWindowTitle("CV View");
		pView->show();*/
	}
	if(btn == ui->iconbtn2)
	{
		ui->stackedWidget->setCurrentIndex(1);
	}
	if(btn == ui->iconbtn3)
	{
		ui->stackedWidget->setCurrentIndex( ui->stackedWidget->currentIndex() - 1);
	}
	if(btn == ui->iconbtn4)
	{
		ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
	}
	if(btn == ui->iconbtn5)
	{
		ui->stackedWidget->setCurrentIndex(4);
	}
}

void StyleWindow::on_toolButton_8_clicked()
{
	auto f_name = QFileDialog::getOpenFileName();
	if (f_name == "")
		return;
	QFile f(f_name);
	if(f.open(QIODevice::ReadOnly))
	{
		initWidgets( f.readAll());
	}
}
