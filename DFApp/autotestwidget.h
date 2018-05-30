#ifndef AUTOTESTWIDGET_H
#define AUTOTESTWIDGET_H

#include <QWidget>
#include <QThread>
#include <QProcess>
namespace Ui {
class AutoTestWidget;
}


class WorkerThread : public QThread
  {
	  Q_OBJECT

public:
	WorkerThread(QObject* pa= nullptr);
	void run();
signals:
	  void resultReady(const QString &s);
};

class AutoTestWidget : public QWidget
{
	Q_OBJECT

public:
	explicit AutoTestWidget(QWidget *parent = 0);
	~AutoTestWidget();

private slots:
	void on_toolButton_clicked();

	void on_toolButton_2_clicked();

	void on_toolButton_3_clicked();

	void on_toolButton_4_clicked();

private:
	Ui::AutoTestWidget *ui;
	QProcess p;
};

#endif // AUTOTESTWIDGET_H
