#include "testthread.h"

#include <QDebug>
#include <QPixmap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QFileDialog>
#include <QApplication>

#include <QDesktopWidget>
#include <QScreen>

#include "osevent.h"
#include "mycv.h"



TestThread::TestThread(QObject *parent)
	:QThread (parent)
{
	_screen_shot_file = "/rong/screen_shot.png";
	_result_file = "/rong/result_test.png";
	_img_folder = "/rong/";
}

void TestThread::add_case(QJsonObject obj)
{
	_case_list.append(obj);
}

void TestThread::clear()
{
	_case_list.clear();
}

void TestThread::run()
{
	_bStop = false;
	_bPause = false;
	foreach (auto obj, _case_list)
	{
		while(_bPause)
		{
			sleep(1);
		}
		if(_bStop )
			break;

		bool res = doTest(obj);
		qDebug() << obj["TestCase"] << res;
		emit result_message("finised case");
	}
}

void TestThread::doText(QString pix_file,QString text)
{
	//take a screen shot;
	doTakeScreenshot(_screen_shot_file);
	auto pos= MatchingMethod( _screen_shot_file,pix_file,4);
	qDebug() << "pos " << pos ;

	QPixmap pix(pix_file);
	click(pos.x() + pix.width() ,pos.y()+ pix.height()/2);


	sleep(1);
	auto str = text;
	foreach(char ch , str.toUtf8())
	{
		key(ch);
	}
}

void TestThread::doClick(QString pix_file)
{
	auto img_file =  pix_file;
	qDebug() << img_file;

	auto pos= MatchingMethod( _screen_shot_file,img_file,4);
	qDebug() << "pos " << pos ;

	QPixmap pix(img_file);
	click(pos.x()+pix.width()/2 ,pos.y()+pix.height()/2);

	sleep(1);
}

void TestThread::doDBClick(QString pix_file)
{
	auto img_file =  pix_file;
	qDebug() << img_file;

	auto pos= MatchingMethod( _screen_shot_file,img_file,4);
	qDebug() << "pos " << pos ;

	QPixmap pix(img_file);
	dbclick(pos.x()+pix.width()/2 ,pos.y()+pix.height()/2);

	sleep(1);
}

void TestThread::doTakeScreenshot(QString img_file)
{
	QDesktopWidget* app_desk = QApplication::desktop();
	QScreen *screen = QGuiApplication::primaryScreen();
	screen->grabWindow(app_desk->screen(0)->winId(),0,0,app_desk->screen(0)->geometry().width()/2,app_desk->screen(0)->geometry().height()/2).save(img_file,"PNG");
}


bool TestThread::doResult(QString pix_file)
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



bool TestThread::doTest(QJsonObject obj)
{
	//s.setProcessChannelMode(QProcess::MergedChannels);
	auto case_name = obj["TestCase"];
	qDebug() << "Start Case -----" << case_name.toString();

	auto pre = obj["PreCodition"].toObject();
	auto app = pre["APP"].toString();
	auto isClearDesk = pre["ClearDesk"].toString();

	s.waitForFinished(-1);
	s.startDetached("open \"/Applications/DFdiscover 5.0.0/"  + app + "\"" );//start
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
	bool res = false;
	foreach(auto s, steps)
	{
		while(_bPause)
		{
			sleep(1);
		}
		if(_bStop )
			break;


		sleep(1);

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
		else if(so["type"].toString().compare("dbclick",Qt::CaseInsensitive) ==0 )
		{
			doDBClick(img_file);
		}
		else if(so["type"].toString().compare("input",Qt::CaseInsensitive) ==0 )
		{
			doText(img_file,str);
		}
		else if(so["type"].toString().compare("result",Qt::CaseInsensitive) == 0)
		{
			res = doResult(img_file);
		}
		step_index++;
	}

	qDebug() << "Clear -------";
	steps = obj["clearup"].toArray();
	foreach(auto s, steps)
	{
		sleep(1);

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
		else if(so["type"].toString().compare("dbclick",Qt::CaseInsensitive) ==0 )
		{
			doDBClick(img_file);
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
	return res;
}

bool TestThread::bStop() const
{
	return _bStop;
}

void TestThread::setBStop(bool bStop)
{
	_bStop = bStop;
}

bool TestThread::bPause() const
{
	return _bPause;
}

void TestThread::setBPause(bool bPause)
{
	_bPause = bPause;
}
