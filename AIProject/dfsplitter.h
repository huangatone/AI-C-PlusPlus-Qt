#ifndef DFSPLITTER_H
#define DFSPLITTER_H

#include <QObject>
#include <QSplitter>
#include <QToolButton>

class DFSplitterHandle: public QSplitterHandle
{
	Q_OBJECT
public:
	DFSplitterHandle(Qt::Orientation orientation, QSplitter *parent=0);
	void setDeriction(int dr =1);
	void setMinimumSize();
	bool isLeftController() const;
	void setIsLeftController(bool isLeftController);

signals:
	void doubleClicked();
protected:
	// QWidget interface
	void paintEvent(QPaintEvent *event);
	//void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	QPixmap left;
	QPixmap right;
	QPixmap up;
	QPixmap down;
	QList<int> _size_list;		
	bool _isLeftController; //up-bottom
	// QWidget interface
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

	// QWidget interface
protected:
	void moveEvent(QMoveEvent *event);

	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
	void drawerButtonPainter(QRect btn_rt, QPainter *P, bool isHover, bool isleft);
};

class DFSplitter : public QSplitter
{
	Q_OBJECT
public:
	DFSplitter(Qt::Orientation orientation= Qt::Horizontal, QWidget *parent = 0);
	DFSplitter( QWidget *parent = 0);

	bool bLeftControler() const;
	void setBLeftControler(bool bLeftControler);

protected:
	QSplitterHandle *createHandle();

	// QWidget interface

	void resizeEvent(QResizeEvent *event);

	  QList<int> _sizes_last;
	  QList<int> _sizs_minimum;

	  bool _bLeftControler;

protected slots:
	  void doubleclicked_handle_event();
	  void slotHandleMoved(int pos,int index);
};

#endif // DFSPLITTER_H
