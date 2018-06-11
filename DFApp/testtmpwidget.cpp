#include "testtmpwidget.h"
#include "ui_testtmpwidget.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QFileDialog>

#include <QDesktopWidget>
#include <QScreen>


#include "osevent.h"
#include "mycv.h"

TestTmpWidget::TestTmpWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TestTmpWidget)
{
	ui->setupUi(this);
	_test_thread =  new TestThread(this);
}

TestTmpWidget::~TestTmpWidget()
{
	delete ui;
}



//do test.
void TestTmpWidget::on_pushButton_clicked()
{
	//dbclick(163,325);
	_test_thread->doTakeScreenshot("/rong/screen_shot.png");
	_test_thread-> doDBClick("/rong/app.png");
	return;
	//int md = ui->lineEdit_3->text().toInt();
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

void TestTmpWidget::on_toolButton_clicked()
{
	auto a = QFileDialog::getOpenFileName();
	if(a.isEmpty())
		return;
	ui->label->setPixmap( QPixmap(a));
}

void TestTmpWidget::on_toolButton_2_clicked()
{
	auto a = QFileDialog::getOpenFileName();
	if(a.isEmpty())
		return;
	ui->label_2->setPixmap( QPixmap(a));
}

//take a screen shot of screen 1;
void TestTmpWidget::on_pushButton_2_clicked()
{
	QDesktopWidget* app_desk = QApplication::desktop();
	QScreen *screen = QGuiApplication::primaryScreen();
	screen->grabWindow(app_desk->screen(0)->winId(),0,0,app_desk->screen(0)->geometry().width()/2,app_desk->screen(0)->geometry().height()/2).save(ui->lineEdit_4->text(),"PNG");
	qDebug() << app_desk->screen(0)->geometry();

}

void TestTmpWidget::on_btnProcess_clicked()
{
	auto txt = ui->lineEdit_6->text();
	//s.start( txt);
	click(100,200);
	s.start("open \"" + txt + "\"");

}

int shiftKeyCode = 56;
bool shiftIsDown = false;

void postKeyboardEvent( int keyCode, bool keyUp = false )
{



	if( keyCode == shiftKeyCode ){

		if( keyUp ){

			shiftIsDown = false;

		}else{

			shiftIsDown = true;

		}

	}

	CGEventSourceRef source = CGEventSourceCreate( kCGEventSourceStateHIDSystemState );
	CGEventRef keyEvent = CGEventCreateKeyboardEvent( source, (CGKeyCode) keyCode, !keyUp );

	if( shiftIsDown ){

		// Use Shift flag
		CGEventSetFlags( keyEvent, CGEventGetFlags( keyEvent ) | kCGEventFlagMaskShift );

	}else{

		// Use all existing flag except Shift
		CGEventSetFlags( keyEvent, CGEventGetFlags( keyEvent ) & ~kCGEventFlagMaskShift );

	}

	CGEventPost( kCGHIDEventTap, keyEvent );

	CFRelease( keyEvent );
	CFRelease( source );

}

void TestTmpWidget::on_btnKey_clicked()
{
	ui->lineEdit_5->setFocus();
	key('*');
	key('9');
	key('>');
	key('.');
	key('m');
	key('M');
	qDebug() <<"flags = " <<  getFlags('>');


	CGKeyCode keyCode = kVK_ANSI_7;
	CGEventFlags flags = kCGEventFlagMaskShift;
	CGEventRef ev;
	CGEventSourceRef source = CGEventSourceCreate (kCGEventSourceStateCombinedSessionState);

	ui->lineEdit_5->setFocus();
	//press down
	ev = CGEventCreateKeyboardEvent (source, keyCode, true);
	CGEventSetFlags(ev,flags | CGEventGetFlags(ev)); //combine flags
	CGEventPost(kCGHIDEventTap,ev);
	CFRelease(ev);

	//press up
	ev = CGEventCreateKeyboardEvent (source, keyCode, false);
	CGEventSetFlags(ev,flags | CGEventGetFlags(ev)); //combine flags
	CGEventPost(kCGHIDEventTap,ev);
	CFRelease(ev);

	CFRelease(source);
}
