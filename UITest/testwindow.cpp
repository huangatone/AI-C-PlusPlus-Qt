#include "testwindow.h"
#include "ui_testwindow.h"

#include "modeltest.h"
#include <QRect>
#include <QPainter>
#include <QPen>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QPushButton>
#include <QTabBar>
#include <QDebug>
#include <QScreen>
#include <QWidget>

#include <QDesktopWidget>

TestWindow::TestWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::TestWindow)
{
	ui->setupUi(this);

	QWidget* cn = new QWidget (this);
	QHBoxLayout* ly = new QHBoxLayout (cn);
	for(int n=0; n<3; n++){
		QToolButton* btn1 = new QToolButton (cn);
		btn1->resize(100,24);
		btn1->setText("!");
		btn1->show();
		ly->addWidget(btn1);
	}
	cn->resize(100,24);
	cn->setLayout(ly);
	ly->setMargin(1);
	ly->setSpacing(1);
	cn->show();
	ui->tabWidget->setCornerWidget(cn, Qt::TopRightCorner);

	QWidget* pTabCornerWidget = new QWidget(this);

	QLabel* pLabelTime = new QLabel(pTabCornerWidget);
	pLabelTime->setText("10:22:20");

	QPushButton* pButton = new QPushButton(pTabCornerWidget);
	pButton->setText("?");
	pButton->setMaximumSize(QSize(25, 25));

	QHBoxLayout* pHLayout = new QHBoxLayout(pTabCornerWidget);
	pHLayout->addWidget(pLabelTime);
	pHLayout->addWidget(pButton);
	pTabCornerWidget->setLayout(pHLayout);
	//ui->tabWidget->setCornerWidget(pTabCornerWidget, Qt::TopRightCorner);
	//ui->tabWidget->tabBar()->setTabButton(1,QTabBar::RightSide,pTabCornerWidget);

	ModelTest test;


	qDebug() << "-----------------------------------------";

	QPalette pal = ui->label->palette();
	pal.setBrush(QPalette::Window, QColor(104, 128, 164));
	pal.setBrush(QPalette::WindowText, QColor(255, 255, 255));
	ui->label->setPalette(pal);
	ui->label->setAutoFillBackground(true);
}

TestWindow::~TestWindow()
{
	delete ui;
}


void TestWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QFont font = painter.font();
	font.setPixelSize(48);
	painter.setFont(font);

	const QRect rectangle = QRect(10, 50, 200, 100);
	QRect boundingRect;
	painter.drawText(rectangle,					 Qt::AlignCenter, tr("Hello"), &boundingRect);

	QPen pen = painter.pen();
	pen.setStyle(Qt::DotLine);
	painter.setPen(pen);
	painter.drawRect(boundingRect.adjusted(0, 0, -pen.width(), -pen.width()));

	pen.setStyle(Qt::DashLine);
	painter.setPen(pen);
	painter.drawRect(rectangle.adjusted(0, 0, -pen.width(), -pen.width()));

}

void TestWindow::on_toolButton_clicked()
{
	ui->toolButton->setChecked(true);
}
