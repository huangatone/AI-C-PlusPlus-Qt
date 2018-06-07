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

protected slots:
	void slot_view_item();

private:
	QStringList _view_menu_items;
};

#endif // MAINWINDOW_H
