#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit TestWindow(QWidget *parent = nullptr);
	~TestWindow();

private:
	Ui::TestWindow *ui;

	// QWidget interface
protected:
	virtual void paintEvent(QPaintEvent *event);
private slots:
	void on_toolButton_clicked();
};

#endif // TESTWINDOW_H
