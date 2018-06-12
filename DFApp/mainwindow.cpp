#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serverwidget.h"
#include "clientwidget.h"
#include "autotestwidget.h"
#include "testwidget.h"
#include "testtmpwidget.h"

MainWindow::MainWindow(QWidget *parent) :
	StyleWindow(parent)
{
	//menuBar()->clear();
	QMenuBar *pBar = new QMenuBar(this);

	QMenu* view = new QMenu(tr("View"),this);
	QAction* ac;

	/*ServerWidget* s= new ServerWidget(this);
	addSubWindow(s);
	auto ac = view->addAction( tr("Server"));
	connect( ac, SIGNAL(triggered()), this, SLOT(slot_view_item()));
	ClientWidget* c = new ClientWidget(this);
	addSubWindow(c);
	ac = view->addAction( tr("Client"));
	connect( ac, SIGNAL(triggered()), this, SLOT(slot_view_item()));

	AutoTestWidget * a = new AutoTestWidget(this);
	addSubWindow(a);
	ac = view->addAction( tr("Test"));
	connect( ac, SIGNAL(triggered()), this, SLOT(slot_view_item()));

*/
	TestWidget * t = new TestWidget(this);
	addSubWindow(t);
	ac = view->addAction( tr("AutoTest"));
	connect( ac, SIGNAL(triggered()), this, SLOT(slot_view_item()));


	TestTmpWidget * tmp = new TestTmpWidget(this);
	addSubWindow(tmp);
	ac = view->addAction( tr("Tmp widget"));
	connect( ac, SIGNAL(triggered()), this, SLOT(slot_view_item()));

	pBar->addMenu(view);
	setMenuBar(pBar);
	setCurrentWidget(0);

	_view_menu_items << /*"Server" << "Client" << */ "Test" << "AutoTest" << "Tmp widget";

	QString st = R"-dd({
				   "buttons":[{ "text":"Main", "icon":"/rong/images/pic/qt/app_list_256.png" },
						   { "text":"Test", "icon":"/rong/images/pic/qt/apps_bookmark_bar_icon.png" },
						   { "text":"Pre-View", "icon":"/rong/images/pic/qt/browser_back_hover.png" },
						   { "text":"Next-view", "icon":"/rong/images/pic/qt/browser_forward_normal.png" },
						   { "text":"Help", "icon":":/icon/help.png" }],

				   "colors" :{ "background":"32, 65, 10, 255",
							   "forecolor":"255, 255, 255",
							   "frame":"255, 255, 255"}
				   })-dd";
	initWidgets(st);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slot_view_item()
{
	QAction *ac = dynamic_cast<QAction*>( sender());
	auto title = ac->text();
	auto index = _view_menu_items.indexOf(title);
	setCurrentWidget(index);
}
