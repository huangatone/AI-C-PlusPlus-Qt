#ifndef CVDIALOG_H
#define CVDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class CvDialog;
}

#include <QLabel>
#include "cvfunction.h"

class CvDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CvDialog(QWidget *parent = 0);
	~CvDialog();

	//the return image will display at scroll area 2.
	//the src come from scroll area 1.
	void setCallbackFunc(QImage (*func) (Mat src))
	{
		callbackFunc = func;
	}



private slots:
	void on_toolButton_clicked();

	void on_toolButton_2_clicked();

	void on_buttonBox_clicked(QAbstractButton *button);

private:
	Ui::CvDialog *ui;
	QLabel* _pic_label1;
	QLabel* _pic_label2;

	QImage (*callbackFunc)(Mat src);

};

#endif // CVDIALOG_H
