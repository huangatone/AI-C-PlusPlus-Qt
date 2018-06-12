#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QProcess>
#include <QJsonObject>

class TestThread : public QThread
{
	Q_OBJECT
public:
	TestThread(QObject *parent = Q_NULLPTR);

	// QThread interface
	void add_case(QJsonObject obj);
	void clear();

	bool bPause() const;
	void setBPause(bool bPause);

	bool bStop() const;
	void setBStop(bool bStop);

	void doText(QString pix_file, QString text);
	void doClick(QString pix_file);
	void doDBClick(QString pix_file);
	bool doResult(QString pix_file);
	void doTakeScreenshot(QString img_file);
	bool doTest(QJsonObject obj);

signals:
	void result_message(QString);

protected:
	void run();

	QString _screen_shot_file;
	QString _img_folder;
	QString _result_file;

	bool _bPause;
	bool _bStop;
	QList<QJsonObject> _case_list;
	QProcess s;


};

#endif // TESTTHREAD_H
