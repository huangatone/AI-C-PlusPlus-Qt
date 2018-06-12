#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QThread>
#include <QProcess>
#include <QJsonObject>
class QListWidgetItem;

#include "testthread.h"

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TestWidget(QWidget *parent = 0);
	~TestWidget();

	bool doTestCase(QString casefile);

private slots:
	void on_tbnLoad_clicked();
	void on_btnExcute_clicked();
	void on_pushButton_3_clicked();
	void on_pushButton_4_clicked();

	void slot_threadMsg(QString );


	void on_btnClear_clicked();

	void on_lwCase_itemClicked(QListWidgetItem *item);

	void on_tbAll_clicked();

	void on_tbNone_clicked();

	void on_tbReverse_clicked();

private:
	Ui::TestWidget *ui;

	TestThread* _test_thread;
};

#endif // TESTWIDGET_H
