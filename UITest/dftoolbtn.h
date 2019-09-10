#ifndef DFTOOLBTN_H
#define DFTOOLBTN_H

#include <QWidget>
#include <QToolButton>

class DFToolBtn : public QToolButton
{
public:
	DFToolBtn(QWidget* p = nullptr);

	void set(void (*p)(QPainter *P, const QRect &R, const QColor &C, int status ) )
	{
		drawFun = p;
	}

	// QWidget interface
protected:
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
	virtual void paintEvent(QPaintEvent *event);

	void (*drawFun) (QPainter *P, const QRect &R, const QColor &C, int status);

	QColor _bkColor;

	// QWidget interface
protected:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);

	// QAbstractButton interface
protected:
	virtual void checkStateSet();
};

#endif // DFTOOLBTN_H
