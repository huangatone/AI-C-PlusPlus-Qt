#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QWidget>
#include "sharui_global.h"

namespace Ui {
class IconButton;
}

class SHARUISHARED_EXPORT IconButton : public QWidget
{
	Q_OBJECT

public:
	explicit IconButton(QWidget *parent = 0);
	~IconButton();
	void SetIcon(QPixmap pix);
	void SetText(QString text);

	QString getText();

private:
	Ui::IconButton *ui;

	// QObject interface
public:
	bool eventFilter(QObject *watched, QEvent *event);

signals:
	void clicked();
};

#endif // ICONBUTTON_H
