#include "testwidget.h"
#include "ui_testwidget.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QFileDialog>

#include <QDesktopWidget>
#include <QScreen>

#include "mycv.h"
#include "osevent.h"

TestWidget::TestWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TestWidget)
{
	ui->setupUi(this);
	_screen_shot_file = "/rong/screen_shot.png";
	_result_file = "/rong/result_test.png";
	_img_folder = "/rong/";
}

TestWidget::~TestWidget()
{
	delete ui;
}


void TestWidget::on_btnExcute_clicked()
{
	auto a = QFileDialog::getOpenFileName();
	if(a.isEmpty())
		return;
	doTestCase(a);
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
	bool bSucc =  doTest(obj);
	if(bSucc)
		ui->listWidget_2->addItem( case_name.toString( ) + " - " + "Passed");
	else
		ui->listWidget_2->addItem( case_name.toString( ) + " - " + "Failed");
	return bSucc;
}

bool TestWidget::doResult(QString pix_file)
{
	//take a screen shot
	doTakeScreenshot(_screen_shot_file);
	//find position
	auto pos= MatchingMethod( _screen_shot_file,pix_file,4);
	//get sub pixmap;
	QPixmap screen_pix(_screen_shot_file);
	QPixmap dst_pix(pix_file);
	QPixmap sub_pix = screen_pix.copy(pos.x(),pos.y(),dst_pix.width(),dst_pix.height());
	qDebug() << pos << screen_pix.width() << screen_pix.height();
	if( sub_pix.save(_result_file,"png") == false)
	{
		qDebug() << "Save error";
		return false;
	}
	sub_pix.detach();
	screen_pix.detach();
	dst_pix.detach();
	qDebug() << "starting match ...";
	//compare sub pixmap and dst pixmap
	float r = hisMatch(_result_file,pix_file);
	if( r < 0.05)
		qDebug() << " Matched ";
	else
		qDebug() << " un Matched ";
	return ( r < 0.05);
}


void TestWidget::doText(QString pix_file,QString text)
{
	//take a screen shot;
	doTakeScreenshot(_screen_shot_file);
	auto pos= MatchingMethod( _screen_shot_file,pix_file,4);
	qDebug() << "pos " << pos ;
	CGPoint point;
	QPixmap pix(pix_file);
	point.x = pos.x() + pix.width() ;
	point.y = pos.y()+ pix.height()/2;

	qDebug() <<"click at" <<  point.x << point.y;
	CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, theEvent);
	CFRelease(theEvent);

	theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, point, kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, theEvent);
	CFRelease(theEvent);
	sleep(1);
	auto str = text;
	foreach(char ch , str.toUtf8())
	{
		CGKeyCode k = QtKeyCode2MacKeyCode(ch);
		CGEventRef mkey = CGEventCreateKeyboardEvent(NULL, k, true);
		CGEventPost(kCGHIDEventTap, mkey);
		CFRelease(mkey);
	}
}

void TestWidget::doClick(QString pix_file)
{
	auto img_file =  pix_file;
	qDebug() << img_file;

	auto pos= MatchingMethod( _screen_shot_file,img_file,4);
	qDebug() << "pos " << pos ;
	CGPoint point;
	QPixmap pix(img_file);

	point.x = pos.x()+pix.width()/2;
	point.y = pos.y()+pix.height()/2;

	qDebug() <<"click at" <<  point.x << point.y;
	CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, theEvent);
	CFRelease(theEvent);

	theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, point, kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, theEvent);
	CFRelease(theEvent);	
	sleep(2);
}

void TestWidget::doTakeScreenshot(QString img_file)
{
	QDesktopWidget* app_desk = QApplication::desktop();
	QScreen *screen = QGuiApplication::primaryScreen();
	screen->grabWindow(app_desk->screen(0)->winId(),0,0,app_desk->screen(0)->geometry().width()/2,app_desk->screen(0)->geometry().height()/2).save(img_file,"PNG");
}

bool TestWidget::doTest(QJsonObject obj)
{
	_bPause = false;
	_bStop = false;

	//s.setProcessChannelMode(QProcess::MergedChannels);
	auto case_name = obj["TestCase"];
	qDebug() << "Start Case -----" << case_name.toString();

	auto pre = obj["PreCodition"].toObject();
	auto app = pre["APP"].toString();
	auto isClearDesk = pre["ClearDesk"].toString();

	s.waitForFinished(-1);
	s.start("open /Applications/DFdiscover 5.0.0/"  + app );
	//s.start("open /rong/"  + app );

	s.waitForFinished(-1);
	qDebug() << "finished";

	QString s_file = _screen_shot_file;
	QDesktopWidget* app_desk = QApplication::desktop();
	QScreen *screen = QGuiApplication::primaryScreen();
	screen->grabWindow(app_desk->screen(0)->winId(),0,0,app_desk->screen(0)->geometry().width()/2,app_desk->screen(0)->geometry().height()/2).save(s_file,"PNG");

	auto steps = obj["steps"].toArray();
	auto img_folder = obj["ImageFolder"].toString();
	int step_index = 1;
	foreach(auto s, steps)
	{
		sleep(1);

		while(_bPause)
		{
			sleep(1);
		}
		if(_bStop )
			break;

		auto so = s.toObject();
		qDebug() <<"Step " << step_index <<  so["type"].toString() << so["text"].toString();
		if(so["type"].toString().compare( "ScreenShot",Qt::CaseInsensitive) == 0)
		{
			screen->grabWindow(app_desk->screen(0)->winId(),0,0,app_desk->screen(0)->geometry().width()/2,app_desk->screen(0)->geometry().height()/2).save(_screen_shot_file,"PNG");
			continue;
		}

		auto img_file = img_folder + so["pic"].toString();
		auto str = so["text"].toString();

		qDebug() << img_file;
		if(so["type"].toString().compare("click",Qt::CaseInsensitive) ==0 )
		{
			doClick(img_file);
		}
		else if(so["type"].toString().compare("input",Qt::CaseInsensitive) ==0 )
		{
			doText(img_file,str);
		}
		else if(so["type"].toString().compare("result",Qt::CaseInsensitive) == 0)
		{
			return doResult(img_file);
		}
		step_index++;
	}

	qDebug() << "End Case -----"<< case_name.toString();
	return false;
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

	ui->listWidget->clear();
	ui->listWidget_2->clear();
	ui->listWidget_3->clear();


	QString str = fi.readAll();
	QJsonDocument doc = QJsonDocument::fromJson( str.toUtf8());
	auto obj = doc.object();


	ui->listWidget->addItem( obj["TestCase"].toString() );

	auto pre = obj["PreCodition"].toObject();
	auto app = pre["APP"].toString();
	auto isClearDesk = pre["ClearDesk"].toString();

	ui->listWidget_2->addItem( app);
	ui->listWidget_2->addItem( isClearDesk);
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

//do test.
void TestWidget::on_pushButton_clicked()
{
	int md = ui->lineEdit_3->text().toInt();
	//MatchingMethod( _src_file,_dst_file,md);
	//MatchingMethod( QString("/rong/s1.png"),QString("/rong/dst.png"),md);
	//qDebug() << "reslut = " <<  diff( ui->lineEdit->text(),ui->lineEdit_2->text());

	//take a screen shot
	on_pushButton_2_clicked();
	//find position
	auto pos= MatchingMethod( ui->lineEdit_4->text(),ui->lineEdit_2->text(),4);
	//get sub pixmap;
	QPixmap screen_pix(ui->lineEdit_4->text());
	QPixmap dst_pix(ui->lineEdit_2->text());
	QPixmap sub_pix = screen_pix.copy(pos.x(),pos.y(),dst_pix.width(),dst_pix.height());
	qDebug() << pos << screen_pix.width() << screen_pix.height();
	if( sub_pix.save("/rong/suggb.png","png") == false)
	{
		qDebug() << "Save error";
		return;
	}
	sub_pix.detach();
	screen_pix.detach();
	dst_pix.detach();
	qDebug() << "starting match ...";
	//compare sub pixmap and dst pixmap
	float r = hisMatch(QString("/rong/suggb.png"),ui->lineEdit_2->text());
	if( r < 0.05)
		qDebug() << " Matched ";
	else
		qDebug() << " un Matched ";

}

void TestWidget::on_toolButton_clicked()
{
	auto a = QFileDialog::getOpenFileName();
	if(a.isEmpty())
		return;
	_src_file = a;
	ui->label->setPixmap( QPixmap(a));
}

void TestWidget::on_toolButton_2_clicked()
{
	auto a = QFileDialog::getOpenFileName();
	if(a.isEmpty())
		return;
	_dst_file = a;
	ui->label_2->setPixmap( QPixmap(a));
}

//take a screen shot of screen 1;
void TestWidget::on_pushButton_2_clicked()
{
	QDesktopWidget* app_desk = QApplication::desktop();
	QScreen *screen = QGuiApplication::primaryScreen();
	screen->grabWindow(app_desk->screen(0)->winId(),0,0,app_desk->screen(0)->geometry().width()/2,app_desk->screen(0)->geometry().height()/2).save(ui->lineEdit_4->text(),"PNG");
	qDebug() << app_desk->screen(0)->geometry();

}

void TestWidget::on_pushButton_3_clicked()
{
	//_bPause = !_bPause;
}

void TestWidget::on_pushButton_4_clicked()
{
	//_bStop = true;
}
