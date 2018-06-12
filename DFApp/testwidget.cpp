#include "testwidget.h"
#include "ui_testwidget.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QDesktopWidget>
#include <QScreen>


#include "osevent.h"
#include "mycv.h"


TestWidget::TestWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TestWidget)
{
	ui->setupUi(this);
	_test_thread =  new TestThread(this);
	connect(_test_thread, SIGNAL(result_message(QString)), this, SLOT(slot_threadMsg(QString )));
}

TestWidget::~TestWidget()
{
	delete ui;
}


void TestWidget::on_btnExcute_clicked()
{
	/*auto a = QFileDialog::getOpenFileName();
	if(a.isEmpty())
		return;
	doTestCase(a);

	return;*/
	_test_thread->clear();
	foreach (auto item, ui->lwCase->selectedItems() ) {
		QJsonObject obj = item->data(Qt::UserRole+1).toJsonObject();
		_test_thread->add_case(obj);
	}
	_test_thread->start();

}

bool TestWidget::doTestCase(QString casefile)
{
	QFile fi(casefile);
	if(fi.open(QIODevice::ReadWrite) == false)
	{
		qDebug() << "file error";
		return false;
	}
	QString str = fi.readAll();
	QJsonDocument doc = QJsonDocument::fromJson( str.toUtf8());
	auto obj = doc.object();
	auto case_name = obj["TestCase"];
	ui->listWidget_2->addItem( case_name.toString( ) + " - " + "Start...");
	_test_thread->clear();
	_test_thread->add_case(obj);
	_test_thread->start();

	return true;
}

void TestWidget::on_tbnLoad_clicked()
{
	auto a = QFileDialog::getOpenFileName();
	if(a.isEmpty())
		return;

	QFile fi(a);
	if(fi.open(QIODevice::ReadWrite) == false)
	{
		qDebug() << "file error";
		return;
	}
	QString str = fi.readAll();
	QJsonDocument doc = QJsonDocument::fromJson( str.toUtf8());
	auto obj = doc.object();

	QListWidgetItem* pItem = new QListWidgetItem(obj["TestCase"].toString(),ui->lwCase);
	//pItem->setCheckState( Qt::Unchecked);
	pItem->setData(Qt::UserRole+1,obj);
}


void TestWidget::on_pushButton_3_clicked()
{
	//_bPause = !_bPause;
	_test_thread->setBPause( !_test_thread->bPause());
}

void TestWidget::on_pushButton_4_clicked()
{
	//_bStop = true;
	_test_thread->setBStop( true);
}

void TestWidget::slot_threadMsg(QString str)
{
	ui->listWidget_2->addItem(str);
}

void TestWidget::on_btnClear_clicked()
{
	ui->lwCase->clear();
	ui->listWidget_2->clear();
	ui->listWidget_3->clear();
	_test_thread->clear();
}

void TestWidget::on_lwCase_itemClicked(QListWidgetItem *item)
{
	ui->listWidget_3->clear();
	//ui->listWidget->addItem( obj["TestCase"].toString() );
	QJsonObject obj = item->data(Qt::UserRole+1).toJsonObject();

	auto pre = obj["PreCodition"].toObject();
	auto app = pre["APP"].toString();
	auto isClearDesk = pre["ClearDesk"].toString();

	ui->listWidget_3->addItem( app);
	ui->listWidget_3->addItem( isClearDesk);
	auto steps = obj["steps"].toArray();
	int case_index = 1;
	foreach(auto s, steps)
	{
		auto so = s.toObject();
		ui->listWidget_3->addItem( "Step "+ QString::number(case_index) + ": " + so["type"].toString() + " - " + so["text"].toString());
		if(so["type"].toString() != "ScreenShot")
			case_index++;
	}
}

void TestWidget::on_tbAll_clicked()
{
	ui->lwCase->selectAll();
}

void TestWidget::on_tbNone_clicked()
{
	foreach (auto item, ui->lwCase->selectedItems() ) {
		item->setSelected(false);
	}
}

void TestWidget::on_tbReverse_clicked()
{
	auto sel_item = ui->lwCase->selectedItems() ;
	ui->lwCase->selectAll();
	foreach (auto item, sel_item ) {
		item->setSelected(false);
	}
}
