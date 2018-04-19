#include "dealview.h"
#include "ui_dealview.h"
#include <QFileDialog>
#include <QDebug>

#include "tourist.h"
#include "stylewindow.h"


Mat  test( double a, double b )
{
   double alpha = 0.5; double beta; double input;
   Mat src1, src2, dst;

   src1 = imread( "/rong/testfile/d1.jpg" );
   src2 = imread( "/rong/testfile/dst.png" );
   if( src1.empty() ) { cout << "Error loading src1" << endl; return Mat(); }
   if( src2.empty() ) { cout << "Error loading src2" << endl; return Mat(); }
   beta = ( 1.0 - alpha );
   addWeighted( src1, a, src2, b, 0.0, dst);

   //imshow( "Linear Blend", dst );
   //waitKey(0);
   return dst;
}

DealView::DealView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DealView),
	pa(parent)
{
	ui->setupUi(this);
	lb = new QLabel(this);
	ui->scrollArea->setWidget(lb);
	ui->scrollArea->setWidgetResizable(false);
	_pictureView = nullptr;

	QStringList fun_list;
	fun_list << "blue" << "GaussianBlur" << "medianBlur" << "bilateralFilter";
	ui->comboBox->addItems(fun_list);
}

DealView::~DealView()
{
	delete ui;
}

void DealView::on_listWidget_itemClicked(QListWidgetItem *item)
{
	lb->setScaledContents(false);
	auto _img =QPixmap(item->text());
	lb->resize( _img.width(), _img.height());
	lb->setPixmap( _img);
}

void DealView::on_horizontalSlider_sliderMoved(int position)
{
on_btnRun_clicked();
}

void DealView::on_horizontalSlider_2_sliderMoved(int position)
{
on_btnRun_clicked();
}

void DealView::on_btnAdd_clicked()
{
	auto fi = QFileDialog::getOpenFileName(this,"Select Image","/rong/testfile/","Images (*.png *.jpeg *.jpg)");
	if(fi == "")
		return;
	ui->listWidget->addItem(fi);

	lb->setScaledContents(false);
	auto _img =QPixmap(fi);
	lb->resize( _img.width(), _img.height());
	lb->setPixmap( _img);
}

void DealView::on_btnDel_clicked()
{
	ui->listWidget->takeItem( ui->listWidget->currentRow());
}

void DealView::on_btnRun_clicked()
{
	Tourist t;
	Mat m;
	if( ui->comboBox->currentIndex() == 0)
	{
		m =t.addImage(ui->listWidget->item(0)->text(),
						  ui->listWidget->item(1)->text(),
						  ui->horizontalSlider->value() / 100.00, ui->horizontalSlider_2->value()/ 100.00);
	}
	else if( ui->comboBox->currentIndex() == 1)
	{
		m = t.onCandy(ui->listWidget->item(0)->text(),ui->horizontalSlider->value());
	}
	else if( ui->comboBox->currentIndex() == 2)
	{
		m =t.Seamless_Clone(ui->listWidget->item(0)->text(),
						  ui->listWidget->item(1)->text(),
						  ui->horizontalSlider->value());
	}

	else if(ui->comboBox->currentIndex() == 3)
	{
		QString m_f = ui->listWidget->count()>=3? ui->listWidget->item(2)->text():"";
		m =t.Seamless_Clone(ui->listWidget->item(0)->text(),
						  ui->listWidget->item(1)->text(),
						  m_f);
	}
	else
	{
		m = t.onCandy(ui->listWidget->item(0)->text(), ui->horizontalSlider->value());
	}



	if( _pictureView == nullptr)
	{
		StyleWindow* p = dynamic_cast<StyleWindow*>( pa );
		if(!p)
		{
			qDebug() << "No Parent";
			return;
		}
		_pictureView = new PictureView(p);
		_pictureView->installEventFilter(this);
		//ui->mdiArea->addSubWindow(view);

		 p->addSubWindow(_pictureView);
		_pictureView->show();

	}
	//_pictureView->setFile(info.absoluteFilePath());

	imwrite("/rong/testfile/cv_tmp.png",m);
	_pictureView->setFile("/rong/testfile/cv_tmp.png");
}

void DealView::on_btnRefresh_clicked()
{
	ui->listWidget->clear();
}

bool DealView::eventFilter(QObject *watched, QEvent *event)
{
	if(watched == _pictureView && event->type() == QEvent::Close)
	{
		_pictureView = nullptr;
	}
	return false;
}

void DealView::on_btnReset_clicked()
{
	auto fi = QFileDialog::getOpenFileName(this,"Select Image","/rong/testfile/","Images (*.png *.jpeg *.jpg)");
	if(fi == "")
		return;
	ui->listWidget->currentItem()->setText(fi);
	//ui->listWidget->addItem(fi);

	lb->setScaledContents(false);
	auto _img =QPixmap(fi);
	lb->resize( _img.width(), _img.height());
	lb->setPixmap( _img);
}
