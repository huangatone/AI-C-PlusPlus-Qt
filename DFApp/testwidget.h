#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QThread>
#include <QProcess>
#include <QJsonObject>

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

	void on_pushButton_clicked();

	void on_toolButton_clicked();

	void on_toolButton_2_clicked();

	void on_pushButton_2_clicked();
	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

protected:
	void doText(QString pix_file, QString text);
	void doClick(QString pix_file);
	bool doResult(QString pix_file);
	void doTakeScreenshot(QString img_file);
	bool doTest(QJsonObject obj);
	static void doTestThread(QJsonObject obj,TestWidget* w);
private:
	Ui::TestWidget *ui;
	QProcess s;
	QString _src_file;
	QString _dst_file;
	QString _screen_shot_file;
	QString _img_folder;
	QString _result_file;

	TestThread* _test_thread;
};

#endif // TESTWIDGET_H
