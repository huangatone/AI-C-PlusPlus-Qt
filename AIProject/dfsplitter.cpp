#include "dfsplitter.h"
#include <QPainter>
#include <QMouseEvent>

#include <QDebug>
const int handle_button_width = 80;
const int minWidth = 100;

DFSplitterHandle::DFSplitterHandle(Qt::Orientation orientation, QSplitter *parent)
	:QSplitterHandle(orientation,parent)

{
	right = QPixmap ("/rong/tmp/splliter2.png");
	left =  QPixmap ("/rong/tmp/splitter2.png");
	up = QPixmap ("/rong/tmp/Triangle_up.png");
	down = QPixmap ("/rong/tmp/Triangle_down.png");
	//MYMEMO: using splitter to left or right


}


void DFSplitterHandle::drawerButtonPainter(QRect btn_rt, QPainter *P, bool isHover, bool isleft)
{
	QPainterPath PP, PA;
	P->setRenderHint(QPainter::Antialiasing);
	int X = btn_rt.x();
	int Y = btn_rt.y();
	int H = btn_rt.height();
	int W = btn_rt.width();

	Y = Y + H / 2 - 60;
	PP.addRect(X, Y, W, 120);
	if (isHover) {
		P->fillPath(PP, QColor(31, 127, 253, 96));
	} else {
		P->fillPath(PP, QColor(31, 127, 253, 50));
	}
	X = X + 2;
	W = W - 4;
	Y = btn_rt.y() + H / 2 - W / 2 ;
	if (isleft) {
		PA.moveTo(X, Y + W );
		PA.lineTo(X + W, Y);
		PA.lineTo(X + W, Y + 2*W);
		PA.lineTo(X, Y + W );
	} else {
		PA.moveTo(X, Y);
		PA.lineTo(X + W, Y + W );
		PA.lineTo(X, Y + W*2);
		PA.lineTo(X, Y);
	}
	P->fillPath(PA, QColor(96, 96, 96));

}

void DFSplitterHandle::setDeriction(int dr)//0-->collapse to left,, 1 to right
{

}

void DFSplitterHandle::setMinimumSize() {}

void DFSplitterHandle::paintEvent(QPaintEvent *event)
{

	QPainter painter(this);
	int i = 0;
	for( i=1 ; i< splitter()->count(); i++)
	  {
		  if( splitter()->handle(i) == this)
			  break;
	  }
	  QRect h_rt = rect();
	  QRect fr_rt;
	  QRect icon;	 
	  QLine l;
	  auto pt = QCursor::pos();
	  auto pt1 = mapFromGlobal(pt);


	  if (orientation() == Qt::Horizontal)
	  {
		  bool left_icon = true;
		  if(i==1 && splitter()->sizes()[0] == 0)
			  left_icon = false;
		  else if(i==2 && splitter()->sizes()[2] != 0)
			  left_icon = false;

		  fr_rt = QRect(0, h_rt.height()/2 -handle_button_width/2, h_rt.width(), handle_button_width);
		  drawerButtonPainter(fr_rt, &painter, fr_rt.contains( pt1), left_icon);
		  return;
	  }
	  else
	  {		 
		  fr_rt= QRect(h_rt.width() / 2 -handle_button_width/2, 0, handle_button_width, h_rt.height() );
		  icon = QRect( h_rt.width() / 2 -8, 0, 16, h_rt.height());
		  l = QLine( QPoint( h_rt.width() / 2 -8 ,h_rt.height()/2 ), QPoint(h_rt.width() / 2 +8,h_rt.height()/2 ));
	  }



	  if(fr_rt.contains( pt1) )
		  painter.fillRect( fr_rt, QBrush(QColor("#58ACFA")));
	  else
		 painter.fillRect( fr_rt, QBrush(QColor("#81BEF7")));

	  QPolygon pl;
	  pl << QPoint( icon.x(), icon.y() + icon.height()/2 )
		 << QPoint( icon.x() + icon.width()/2, icon.y())
		 << QPoint( icon.right(), icon.y() + icon.height()/2)
		 << QPoint( icon.x() + icon.width()/2, icon.bottom());
	  painter.drawPolygon( pl);
	  painter.drawLine(l);



 }

void DFSplitterHandle::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (orientation() == Qt::Horizontal)
	{
		QRect h_rt = rect();
		QRect fr_rt(0, h_rt.height()/2 -handle_button_width/2, h_rt.width(), handle_button_width);
		auto pt1 = event->pos();

		int i = 0;
		for( i=1 ; i< splitter()->count(); i++)
		{
			if( splitter()->handle(i) == this)
				break;
		}


		if(fr_rt.contains(pt1))
		{
			auto szs = splitter()->sizes();
			emit(doubleClicked());
		}
	}
	else
	{
		QRect h_rt = rect();
		QRect fr_rt(h_rt.width() / 2 -handle_button_width/2, 0, handle_button_width, h_rt.height() );
		auto pt1 = event->pos();

		int i = 0;
		for( i=1 ; i< splitter()->count(); i++)
		{
			if( splitter()->handle(i) == this)
				break;
		}

		if(fr_rt.contains(pt1))
		{
			auto szs = splitter()->sizes();
			emit(doubleClicked());
		}
	}
}
void DFSplitterHandle::mouseMoveEvent(QMouseEvent *event)
{
	auto pt1 = event->pos();
	QRect h_rt = rect();
	QRect icon( 1, h_rt.height()/2 -8, h_rt.width()-2, 16);
	QRect fr_rt(-1, h_rt.height()/2 -handle_button_width/2, h_rt.width()+2, handle_button_width);
	setCursor(Qt::SplitHCursor);
	if(fr_rt.contains(pt1))
	{
		qDebug() << "in handle";
		setCursor(Qt::ArrowCursor);
		return;
	}
	if( event->button()  | Qt::LeftButton)
	{
		qDebug() << "left button  handle";
		if(pt1.y() > fr_rt.y() && pt1.y() < fr_rt.bottom())
			return;
	}
	qDebug() << "out handle";
	update();
	QSplitterHandle::mouseMoveEvent(event);
}

bool DFSplitterHandle::isLeftController() const
{
	return _isLeftController;
}

void DFSplitterHandle::setIsLeftController(bool isLeftController)
{
	_isLeftController = isLeftController;
}

void DFSplitterHandle::enterEvent(QEvent *event)
{
	QSplitterHandle::enterEvent(event);
}
void DFSplitterHandle::leaveEvent(QEvent *event)
{
	update();
	QSplitterHandle::leaveEvent(event);
}


void DFSplitterHandle::moveEvent(QMoveEvent *event)
{

}

void DFSplitterHandle::resizeEvent(QResizeEvent *event)
{

}


DFSplitter::DFSplitter(Qt::Orientation orientation, QWidget *parent )
	:QSplitter (orientation, parent)
{
	setHandleWidth(8);
	connect(this, SIGNAL(splitterMoved(int , int )), this , SLOT(slotHandleMoved(int, int)));
}

DFSplitter::DFSplitter( QWidget *parent )
	:QSplitter ( parent)
{
	setHandleWidth(8);
	connect(this, SIGNAL(splitterMoved(int , int )), this , SLOT(slotHandleMoved(int, int)));
}

QSplitterHandle *DFSplitter::createHandle()
  {
	  auto hd =  new DFSplitterHandle(orientation(), this);
	  connect(hd, SIGNAL(doubleClicked()), this, SLOT(doubleclicked_handle_event()));
	  _sizes_last<< minWidth;
	  _sizs_minimum << minWidth;
	  return hd;
}


void DFSplitter::resizeEvent(QResizeEvent *event)
{
	 QSplitter::resizeEvent(event);
}

bool DFSplitter::bLeftControler() const
{
	return _bLeftControler;
}

void DFSplitter::setBLeftControler(bool bLeftControler)
{
	_bLeftControler = bLeftControler;
}

void DFSplitter::slotHandleMoved(int pos,int index)
{
	auto szs = sizes();
	_sizes_last = szs;
	QList<int > n_szs;
	if(count() == 2)
	{
		if( szs[0] < minWidth)
			n_szs << minWidth << width() - minWidth;
		else if( szs[1] < minWidth)
			n_szs << width() - minWidth << minWidth;
	}
	else
	{

	}
	if(n_szs.count() > 1 )
		setSizes(n_szs);

}

 void DFSplitter::doubleclicked_handle_event()
 {
	 DFSplitterHandle* hd = (DFSplitterHandle*) sender();
	 auto szs = sizes();
	 if( hd == handle(1) && count() == 2)
	 {

		 if(szs[1] != 0)
			 setSizes( QList<int>() << width() << 0);
		 else
			 setSizes( QList<int>() << width()-_sizes_last[1] << _sizes_last[1]);
	 }
	 else if( hd == handle(1) && count() > 2)
	 {
		 QList<int> n_szs;
		if(szs[0] == 0)
		{
			setSizes( _sizes_last );
		}
		else
		{
			n_szs << 0 << szs[0] + szs[1];
			for(int n =2; n < count(); n++)
			{
				n_szs << szs[n];
			}
			setSizes(n_szs);
		}

	 }
	 else if( hd == handle( count() -1 ) && count() > 2)
	 {

		 if( szs[ count() -1] ==0 )
		 {
			 setSizes( _sizes_last );
		 }
		 else
		 {
			QList<int> n_szs;
			for(int n= 0; n < count()-2; n++)
				n_szs << szs[n];
			n_szs << szs[count() -2] + szs[count() - 1] << 0;
			setSizes(n_szs);
		 }

	 }
	 else
	 {

		 int i = 0;
		 for( i=1 ; i< count(); i++)
		 {
			 if( handle(i) == hd)
				 break;
		 }
		 i-=1;

		 QList<int> n_szs;
		if(szs[i] == 0)
		{
			setSizes( _sizes_last );
		}
		else
		{
			for(int n=0; n<i; n++)
				n_szs  << szs[n];
			n_szs << 0 << szs[i] + szs[i+1];
			for(int n= i+2; n < count(); n++)
				n_szs << szs[n];

			setSizes(n_szs);
		}
	 }
 }
