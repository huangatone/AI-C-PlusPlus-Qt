#include "dealview.h"
#include "ui_dealview.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "tourist.h"
#include "stylewindow.h"
#include "cvfunction.h"



DealView::DealView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DealView),
	pa(parent)
{
	ui->setupUi(this);

	_pictureView = nullptr;

	QStringList fun_list;
	fun_list << "blue" << "GaussianBlur" << "medianBlur" << "bilateralFilter";

}

DealView::~DealView()
{
	delete ui;
}

void DealView::on_listWidget_itemClicked(QListWidgetItem *item)
{

	auto _img =QPixmap(item->text());
	ui->cvpictureWidget->setViewFile( item->text() );

}


void DealView::on_btnAdd_clicked()
{
	auto fi = QFileDialog::getOpenFileName(this,"Select Image","/rong/testfile/","Images (*.png *.jpeg *.jpg)");
	if(fi == "")
		return;
	ui->listWidget->addItem(fi);

	ui->cvpictureWidget->setViewFile( fi );
}

void DealView::on_btnDel_clicked()
{
	ui->listWidget->takeItem( ui->listWidget->currentRow());
}

void DealView::on_btnRun_clicked()
{
	Tourist t;
	CVFunction cv;
	Mat m;
	QString cmd = _cmd;
	int nitem = ui->listWidget->count();
	if(nitem == 0)
	{
		QMessageBox::warning(this,"Files","At least one file Required ");
		return;
	}
	try {

		if( cmd.compare("Image_Stitching",Qt::CaseInsensitive) == 0 )
		{
			if(nitem <2 )
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}
			m = Image_Stitching(ui->listWidget->item(0)->text(),ui->listWidget->item(1)->text());
		}
		else if( cmd.compare("Advanced_Edge_Detection",Qt::CaseInsensitive) == 0 )
		{
			m = Advanced_Edge_Detection( ui->listWidget->item(0)->text(),"/rong/tmp");
		}
		else if( cmd.compare("Colour_Segmentation",Qt::CaseInsensitive) == 0 )
		{
			m = Colour_Segmentation(ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("Colour_Transfer",Qt::CaseInsensitive) == 0 )
		{
			if(nitem <2 )
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}
			QStringList file_lt;
			file_lt << ui->listWidget->item(0)->text() <<ui->listWidget->item(1)->text();
			auto out = Colour_Transfer( file_lt);
				foreach(auto m, out)
					displayResult(m);
				return;
		}
		else if( cmd.compare("Contrast_Enhancement",Qt::CaseInsensitive) == 0 )
		{
			m = Contrast_Enhancement( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("Edge_Detection",Qt::CaseInsensitive) == 0 )
		{
			m = Edge_Detection( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("Histogram_Equalization",Qt::CaseInsensitive) == 0 )
		{
			m = Histogram_Equalization( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("Harris_Corner_Detector",Qt::CaseInsensitive) == 0 )
		{
			m = Harris_Corner_Detector( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("Image_Sharpening",Qt::CaseInsensitive) == 0 )
		{
			m = Image_Sharpening( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("Image_Smoothing",Qt::CaseInsensitive) == 0 )
		{
			if(nitem <2 )
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}
			QStringList file_lt;
			file_lt << ui->listWidget->item(0)->text() <<ui->listWidget->item(1)->text();
				auto out = Image_Smoothing( file_lt);
				displayResult(out);
				return;
		}
		else if( cmd.compare("Image_Stitching",Qt::CaseInsensitive) == 0 )
		{
			if(nitem <2 )
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}
			QStringList file_lt;
			file_lt << ui->listWidget->item(0)->text() <<ui->listWidget->item(1)->text();
			m = Image_Stitching( file_lt);
		}
		else if( cmd.compare("Point_Operations_on_Digital_Images",Qt::CaseInsensitive) == 0 )
		{
			m = Point_Operations_on_Digital_Images( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("seam_carving",Qt::CaseInsensitive) == 0 )
		{
			m = seam_carving( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("Text_Read",Qt::CaseInsensitive) == 0 )
		{
			m = seam_carving( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("make_rect_circle",Qt::CaseInsensitive) == 0 )
		{
			m = make_rect_circle( ui->listWidget->item(0)->text());
		}
		else if( cmd.compare("find_sharp",Qt::CaseInsensitive) == 0)
		{

			Mat d = imread(ui->listWidget->item(0)->text().toStdString());
			 m = cv.find_sharp2(d);
		}
		else if(cmd.compare("addImage",Qt::CaseInsensitive) == 0)
		{
			if( nitem < 2)
				{
					QMessageBox::warning(this,"Files","Required two files");
					return;
				}
			m =t.addImage(ui->listWidget->item(0)->text(),
							  ui->listWidget->item(1)->text(),
							  ui->sliderA->value() / 100.00, ui->sliderB->value()/ 100.00);
		}
		else if(cmd.compare("Candy",Qt::CaseInsensitive) == 0)
		{
			m = t.onCandy(ui->listWidget->item(0)->text(),ui->sliderA->value());
		}
		else if(cmd.compare("Seamless_Clone_Color",Qt::CaseInsensitive) == 0)
		{
			if( nitem < 2)
				{
					QMessageBox::warning(this,"Files","Required two files");
					return;
				}
			m =t.Seamless_Clone(ui->listWidget->item(0)->text(),
							  ui->listWidget->item(1)->text(),
							  ui->sliderA->value());
		}

		else if(cmd.compare("Seamless_Clone_Mask",Qt::CaseInsensitive) == 0)
		{
			if( nitem < 2)
				{
					QMessageBox::warning(this,"Files","Required two files");
					return;
				}
			QString m_f = ui->listWidget->count()>=3? ui->listWidget->item(2)->text():"";
			m =t.Seamless_Clone(ui->listWidget->item(0)->text(),
							  ui->listWidget->item(1)->text(),
							  m_f);
		}
		else if( cmd.compare("Advanced_Edge_Detection_1",Qt::CaseInsensitive) == 0 )
		{
			m = t.Advanced_Edge_Detection( ui->listWidget->item(0)->text(),3,ui->sliderA->value(),ui->sliderB->value());
		}

		else
			return;
	}
	catch (cv::Exception& e) {
			qDebug() << "Error opencv-- " << QString ::fromStdString(e.msg)  ;
			return;
		}


	displayResult(m);
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
	if(ui->listWidget->count() == 0)
		return;
	auto fi = QFileDialog::getOpenFileName(this,"Select Image","/rong/testfile/","Images (*.png *.jpeg *.jpg)");
	if(fi == "")
		return;
	if(ui->listWidget->currentItem() == nullptr)
		ui->listWidget->setCurrentRow(0);
	ui->listWidget->currentItem()->setText(fi);
	//ui->listWidget->addItem(fi);

	ui->cvpictureWidget->setViewFile( fi );
}

void DealView::on_btnAddFromTree_clicked()
{
	if(ui->cvpictureWidget->getViewFile() != "")
		ui->listWidget->addItem( ui->cvpictureWidget->getViewFile());
}

QString DealView::cmd() const
{
	return _cmd;
}

void DealView::setCmd(const QString &cmd)
{
	_cmd = cmd;
}


void DealView::displayResult(Mat &m,QString )
{
	if(m.empty())
		return;
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

void DealView::on_sliderA_sliderMoved(int )
{
	on_btnRun_clicked();
}
void DealView::on_sliderB_sliderMoved(int )
{
	on_btnRun_clicked();
}
void DealView::on_sliderC_sliderMoved(int )
{
	on_btnRun_clicked();
}
void DealView::on_sliderD_sliderMoved(int )
{
	on_btnRun_clicked();
}
