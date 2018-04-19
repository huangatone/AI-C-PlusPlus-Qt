
#include <QApplication>
#include <QDebug>

#include "stylewindow.h"
#include "cvfunction.h"
#include <QStyleFactory>

#include "tourist.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StyleWindow w;
	w.show();
	return a.exec();
}
