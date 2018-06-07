#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serverwidget.h"
#include "clientwidget.h"
#include "autotestwidget.h"
#include "testwidget.h"

MainWindow::MainWindow(QWidget *parent) :
	StyleWindow(parent)
{
	//menuBar()->clear();
	QMenuBar *pBar = new QMenuBar(this);

	QMenu* view = new QMenu(tr("View"),this);

	ServerWidget* s= new ServerWidget(this);
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


	TestWidget * t = new TestWidget(this);
	addSubWindow(t);
	ac = view->addAction( tr("AutoTest"));
	connect( ac, SIGNAL(triggered()), this, SLOT(slot_view_item()));

	pBar->addMenu(view);
	setMenuBar(pBar);
	setCurrentWidget(3);

	_view_menu_items << "Server" << "Client" << "Test" << "AutoTest";
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
