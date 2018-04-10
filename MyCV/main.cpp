#include "mainwindow.h"
#include <QApplication>
#include "opencvwindow.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenCVWindow w;
	//MainWindow w;
	w.show();

	return a.exec();
}
