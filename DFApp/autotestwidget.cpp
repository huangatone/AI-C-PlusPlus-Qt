#include "autotestwidget.h"
#include "ui_autotestwidget.h"
#include <QtTest>
#include <QDesktopWidget>
#include <QScreen>
#include <QMouseEvent>

//#include <Foundation/Foundation.h>
//#include <ApplicationServices/ApplicationServices.h>

#include <QDialog>
#include "testdialog.h"
#include <QDialogButtonBox>
#include <QPushButton>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <list>
#include <vector>
#include <thread>
#include <iostream>


#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>

using namespace std;


void test()
{
	qDebug() << "--------" <<  0.000244140625* 288895.288400;
}
CGKeyCode QtKeyCode2MacKeyCode(Qt::Key k){
	switch(k){
	case Qt::Key_0:
		return kVK_ANSI_0;
	case Qt::Key_1:
		return kVK_ANSI_1;
	case Qt::Key_2:
		return kVK_ANSI_2;
	case Qt::Key_3:
		return kVK_ANSI_3;
	case Qt::Key_4:
		return kVK_ANSI_4;
	case Qt::Key_5:
		return kVK_ANSI_5;
	case Qt::Key_6:
		return kVK_ANSI_6;
	case Qt::Key_7:
		return kVK_ANSI_7;
	case Qt::Key_8:
		return kVK_ANSI_8;
	case Qt::Key_9:
		return kVK_ANSI_9;
	case Qt::Key_Q:
		return kVK_ANSI_Q;
	case Qt::Key_W:
		return kVK_ANSI_W;
	case Qt::Key_E:
		return kVK_ANSI_E;
	case Qt::Key_R:
		return kVK_ANSI_R;
	case Qt::Key_T:
		return kVK_ANSI_T;
	case Qt::Key_Y:
		return kVK_ANSI_Y;
	case Qt::Key_U:
		return kVK_ANSI_U;
	case Qt::Key_I:
		return kVK_ANSI_I;
	case Qt::Key_O:
		return kVK_ANSI_O;
	case Qt::Key_P:
		return kVK_ANSI_P;
	case Qt::Key_A:
		return kVK_ANSI_A;
	case Qt::Key_S:
		return kVK_ANSI_S;
	case Qt::Key_D:
		return kVK_ANSI_D;
	case Qt::Key_F:
		return kVK_ANSI_F;
	case Qt::Key_G:
		return kVK_ANSI_G;
	case Qt::Key_H:
		return kVK_ANSI_H;
	case Qt::Key_J:
		return kVK_ANSI_J;
	case Qt::Key_K:
		return kVK_ANSI_K;
	case Qt::Key_L:
		return kVK_ANSI_L;
	case Qt::Key_Z:
		return kVK_ANSI_Z;
	case Qt::Key_X:
		return kVK_ANSI_X;
	case Qt::Key_C:
		return kVK_ANSI_C;
	case Qt::Key_V:
		return kVK_ANSI_V;
	case Qt::Key_B:
		return kVK_ANSI_B;
	case Qt::Key_N:
		return kVK_ANSI_N;
	case Qt::Key_M:
		return kVK_ANSI_M;
	case Qt::Key_F1:
		return kVK_F1;
	case Qt::Key_F2:
		return kVK_F2;
	case Qt::Key_F3:
		return kVK_F3;
	case Qt::Key_F4:
		return kVK_F4;
	case Qt::Key_F5:
		return kVK_F5;
	case Qt::Key_F6:
		return kVK_F6;
	case Qt::Key_F7:
		return kVK_F7;
	case Qt::Key_F8:
		return kVK_F8;
	case Qt::Key_F9:
		return kVK_F9;
	case Qt::Key_F10:
		return kVK_F10;
	case Qt::Key_F11:
		return kVK_F11;
	case Qt::Key_F12:
		return kVK_F12;
	case Qt::Key_Left:
		return kVK_LeftArrow;
	case Qt::Key_Right:
		return kVK_RightArrow;
	case Qt::Key_Down:
		return kVK_DownArrow;
	case Qt::Key_Up:
		return kVK_UpArrow;
	case Qt::Key_Return:
		return kVK_Return;
	case Qt::Key_Tab:
		return kVK_Tab;
	case Qt::Key_Space:
		return kVK_Space;
	case Qt::Key_Delete:
		return kVK_Delete;
	case Qt::Key_Escape:
		return kVK_Escape;
	case Qt::Key_Shift:
		return kVK_Shift;
	case Qt::Key_Control:
		return kVK_Control;
	case Qt::Key_CapsLock:
		return kVK_CapsLock;
	default:
		return kVK_Escape;

	}
}

AutoTestWidget::AutoTestWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AutoTestWidget)
{
	ui->setupUi(this);
	ui->comboBox->addItems(QStringList() << "C1" << "D2" << "E3" << "F4");
	QDesktopWidget* app = QApplication::desktop();
/*
	qDebug() << app->screenCount() << app->numScreens() << app->screenGeometry(0) << app->screenGeometry(1) << app->primaryScreen();
	app->screen(0)->  grab( QRect(0,0, 1440,900) ).save("/rong/screen_sub.png","PNG");
	grab().save("/rong/screen1.png","PNG");
	qDebug() << app->screen(0)->geometry();
	for(int i=0; i< 25; i++)
	{
		QTest::mousePress( app->screen(),Qt::LeftButton,Qt::NoModifier,QPoint(i*100,700));
	}
	QTest::mousePress( app->screen(),Qt::LeftButton,Qt::NoModifier,QPoint(2700,700));

	QTestEventList *eventos = new QTestEventList();
	eventos->addMouseClick(Qt::LeftButton, 0, QPoint(5,5), -1);
	eventos->simulate(this);

	QScreen *screen = QGuiApplication::primaryScreen();
	screen->grabWindow(app->screen(0)->winId(),0,0,app->screen(0)->geometry().width()/2,app->screen(0)->geometry().height()/2).save("/rong/screen_1.png","PNG");
	screen->grabWindow(app->screen(1)->winId(),app->screen(1)->geometry().x(),app->screen(1)->geometry().y(),app->screen(1)->geometry().width()/2,app->screen(1)->geometry().height()/2).save("/rong/screen_2.png","PNG");


	QPoint pos(0,0);
	QMouseEvent event(QEvent::MouseButtonPress,pos,Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
	//QApplication::sendEvent(nullptr, &event);
	//std::thread th(testing);


	//WorkerThread *workerThread = new WorkerThread(this);
		 // connect(workerThread, &WorkerThread::resultReady, this, &MyObject::handleResults);
		 // connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
	//	  workerThread->start();

  std::thread *dd = new std::thread(test);
  //dd.join();


  QImage desk = qApp->screens().at(0)->grabWindow(
  QDesktopWidget().winId(),
  0,
  0,
  1000,
  800).toImage();
  desk.save("/rong/tt.png","PNG");



  //p.start("/rong/click  " );
  //QString tmp = p.readAll();

  p.start("/rong/click 1450 850 " );
  //qDebug() << tmp;


*/
}

AutoTestWidget::~AutoTestWidget()
{
	delete ui;
}

void AutoTestWidget::on_toolButton_clicked()
{
	QTest::keyClick(ui->lineEdit, Qt::Key_0);
	QTest::keyClick(ui->lineEdit, Qt::Key_0);
	QTest::keyClick(ui->lineEdit, Qt::Key_Tab);
	QTest::keyClick(this, Qt::Key_0);
	QTest::keyClick(this, Qt::Key_0);
	//QVERIFY2(ui->lineEdit->text() == "0g0", "error txt");

	QTest::mouseClick( ui->comboBox,Qt::LeftButton);

	QDesktopWidget* app = QApplication::desktop();
	QScreen *screen = QGuiApplication::primaryScreen();
	 if (const QWindow *window = windowHandle())
	 {
		 qDebug() << "window handle";
		 screen = window->screen();
	 }
	 if (!screen)
		 return;
	 //auto originalPixmap = screen->grabWindow(1);
	 //originalPixmap.save("/rong/sceen.png","PNG");
	 app->screen(0)->  grab().save("/rong/screen1.png","PNG");


	 QTest::mousePress( app->screen(0),Qt::LeftButton,Qt::NoModifier,QPoint(1350,850));

}
//QTEST_MAIN(AutoTestWidget)

void AutoTestWidget::on_toolButton_2_clicked()
{
	int w = ui->lineEdit->text().toInt();
	int h = ui->lineEdit_2->text().toInt();

	QDesktopWidget* app = QApplication::desktop();
	QScreen *screen = QGuiApplication::primaryScreen();

	 auto img = screen->grabWindow(app->screen(0)->winId(),0,0,w,h);
	 ui->label_2->setPixmap( img);
}



WorkerThread::WorkerThread(QObject *pa):QThread(pa)
{
}

void WorkerThread::run()
{
	QString result = "s";
	qDebug() << "testing" ;
	while(true)
	{
		auto a  = QApplication::activeWindow();
		QLineEdit * p = dynamic_cast<QLineEdit*>(a);
		if(p)
		{

			qDebug() << p->text();
			sleep(2);
		}
		TestDialog  *d = dynamic_cast<TestDialog*>(a);
		if(d)
		{
			qDebug() << d->windowTitle();
			auto l1 = d->findChild<QLineEdit*>("ltName",Qt::FindDirectChildrenOnly);
			auto l2 = d->findChild<QLineEdit*>("ltPass",Qt::FindDirectChildrenOnly);
			if(l1)
			{
				QTest::keyClick(l1, Qt::Key_A);
				QTest::keyClick(l1, Qt::Key_0);
			}
			if(l2)
			{
				QTest::keyClick(l2, Qt::Key_A);
				QTest::keyClick(l2, Qt::Key_A);
			}
			int x = 300;
			int y= 25;
			auto b = d->findChild<QDialogButtonBox*>("buttonBox");
			if(b)
			{
				x = b->button(QDialogButtonBox::Ok)->geometry().x() + 4;
				y = b->button(QDialogButtonBox::Ok)->geometry().y() + 4;

				sleep(10);
				QProcess p;
				p.start("./rong/click 2,2 " );
				//QTest::mouseClick( b->button(QDialogButtonBox::Ok) ,Qt::LeftButton,Qt::NoModifier,QPoint(10,10));
			}
			QTest::mouseClick( d,Qt::LeftButton,Qt::NoModifier,QPoint(x,y));

			QPoint pos(x,y);
			QMouseEvent event(QEvent::MouseButtonPress,pos,Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
			QApplication::sendEvent(d, &event);

			break;
		}
	}
	//emit resultReady(result);
}

void AutoTestWidget::on_toolButton_3_clicked()
{
	/*TestDialog dlg;
	dlg.setWindowTitle("Test Dialog");
	dlg.exec();*/


	CGPoint point;
	point.x = 1400;
	point.y = 850;
	CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, theEvent);
	CFRelease(theEvent);


	theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, point, kCGMouseButtonLeft);
		CGEventPost(kCGHIDEventTap, theEvent);
		CFRelease(theEvent);


		CGEventRef mkey = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)kVK_ANSI_0, true);
		CGEventPost(kCGHIDEventTap, mkey);
		CFRelease(mkey);



}

void AutoTestWidget::on_toolButton_4_clicked()
{
	p.start("/rong/click -x 1400 -y 850 " );
QString tmp = p.readAll();
p.waitForFinished(-1);
	ui->lineEdit->setFocus();

	p.start("/rong/key" );
}
