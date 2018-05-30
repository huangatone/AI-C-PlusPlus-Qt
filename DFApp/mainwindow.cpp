#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serverwidget.h"
#include "clientwidget.h"
#include "autotestwidget.h"

MainWindow::MainWindow(QWidget *parent) :
	StyleWindow(parent)
{

	ServerWidget* s= new ServerWidget(this);
	addSubWindow(s);
	ClientWidget* c = new ClientWidget(this);
	addSubWindow(c);
	AutoTestWidget * a = new AutoTestWidget(this);
	addSubWindow(a);

}

MainWindow::~MainWindow()
{

}
