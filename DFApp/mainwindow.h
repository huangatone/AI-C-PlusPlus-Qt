#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "stylewindow.h"
class MainWindow : public StyleWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();


};

#endif // MAINWINDOW_H
