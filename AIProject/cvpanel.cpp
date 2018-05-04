#include "cvpanel.h"
#include "ui_cvpanel.h"
#include <QMdiSubWindow>
#include "cvpictureview.h"
#include "pictureview.h"
#include "cvpictureview.h"
#include "dealview.h"
#include "tourist.h"
#include "cvfunction.h"
#include "iconbutton.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QMessageBox>

CVPanel::CVPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CVPanel)
{
	ui->setupUi(this);
	_pictureView = nullptr;

	connect( ui->fileWidget1, SIGNAL( preview()), this, SLOT(slotPreview()));
	connect( ui->fileWidget1, SIGNAL( useCurrentPic()), this, SLOT(slotSetFile()));

	connect( ui->fileWidget2, SIGNAL( preview()), this, SLOT(slotPreview()));
	connect( ui->fileWidget2, SIGNAL( useCurrentPic()), this, SLOT(slotSetFile()));

	connect( ui->fileWidget3, SIGNAL( preview()), this, SLOT(slotPreview()));
	connect( ui->fileWidget3, SIGNAL( useCurrentPic()), this, SLOT(slotSetFile()));

	connect( ui->fileWidget4, SIGNAL( preview()), this, SLOT(slotPreview()));
	connect( ui->fileWidget4, SIGNAL( useCurrentPic()), this, SLOT(slotSetFile()));

	ui->fileWidget2->setLabel_text("Mask");
	ui->fileWidget1->setLabel_text("Src");
	ui->treeWidget->setColumnWidth(0,200);
}

CVPanel::~CVPanel()
{
	delete ui;
}



void CVPanel::initWidgets(QString str)
{
	qDebug() <<"my god";
	QJsonDocument doc = QJsonDocument::fromJson( str.toUtf8());
	auto obj = doc.object();
	auto btn_objs = obj["buttons"];
	auto btn_arr = btn_objs.toArray();



	auto tree_objs = obj["trees"];

	auto tree_arr = tree_objs.toArray();


	for(int i=0; i< tree_arr.count(); i++)
	{
		auto sub_obj =  tree_arr[i].toObject();
		/*QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
		item->setText(0,	sub_obj.value("text").toString());
		item->setToolTip(0,	sub_obj.value("tooltip").toString());
*/
		QListWidgetItem* pListItem = new QListWidgetItem(ui->functionList);
		//pListItem->setToolTip(sub_obj.value("tooltip").toString());
		pListItem->setSizeHint(QSize(96,96));

		auto btn = new IconButton(this);
		btn->SetIcon(QPixmap( "icon/" + sub_obj.value("icon").toString()));
		btn->SetText(sub_obj.value("text").toString());
		ui->functionList->setItemWidget( pListItem, btn);
	}

	auto colors = obj["colors"].toObject();
	QString clr = "%1 background-color: rgba(%2);color: rgb(%3);";

	QString bk = colors.value("background").toString();
	QString fk =colors.value("forecolor").toString();
	QString style_str = clr.arg("QMainWindow").arg(bk).arg(fk);
	style_str += clr.arg("QPushButton").arg(bk).arg(fk);
	style_str += clr.arg("QToolBar").arg(bk).arg(fk);
	style_str += clr.arg("QMenuBar").arg(bk).arg(fk);
	style_str += clr.arg("QLabel").arg(bk).arg(fk);
	style_str += clr.arg("QToolButton").arg(bk).arg(fk);


	//ui->widget_2->setStyleSheet(single_style);

	//qDebug() << single_style;
	//qApp->setStyleSheet(style_str);
}

void CVPanel::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int )
{
	auto txt = item->text(0);
	/*auto s = ui->mdiArea->currentSubWindow();
	s->setWindowTitle( txt);
	auto w = s->widget();
	DealView* d_w = dynamic_cast<DealView*>(w);
	if(d_w)
		d_w->setCmd( txt );*/
	ui->btnDo->setText("Process " + txt);
}

void CVPanel::on_treeWidget_itemClicked(QTreeWidgetItem *item, int )
{
	auto txt = item->text(0);
	/*auto s = ui->mdiArea->currentSubWindow();
	s->setWindowTitle( txt);
	auto w = s->widget();
	DealView* d_w = dynamic_cast<DealView*>(w);
	if(d_w)
		d_w->setCmd( txt );*/

	ui->btnDo->setText("Process " + txt);
}



void CVPanel::on_btnDo_clicked()
{
	auto cmd = ui->treeWidget->currentItem()->text(0);
	process(cmd);
	return;
}

void CVPanel::displayResult(Mat &m,QString title)
{
	if(m.empty())
		return;


	imwrite(title.toStdString() + ".png",m);

	if(_pictureView == nullptr)
	{
		_pictureView = new PictureView(this);
		_pictureView->installEventFilter(this);
		auto a = ui->mdiArea->addSubWindow(_pictureView);
		a->setWindowTitle("Result Picture");
		Qt::WindowFlags flags = 0;
		// s ^= Qt::WindowCloseButtonHint;
		flags = Qt::WindowMinMaxButtonsHint;
		//a->setWindowFlags(flags);
		a->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
	}
	_pictureView->setFile(title + ".png");
	_pictureView->show();
}

void CVPanel::addSubWindow(QWidget* w)
{
	ui->mdiArea->addSubWindow(w);
}



void CVPanel::slotSetFile()
{
	auto nSub = ui->mdiArea->subWindowList();
	foreach(auto sw, nSub)
	{
		{
			qDebug() << "2";
			CVPictureView* view= dynamic_cast<CVPictureView*>(sw->widget()) ;
			if(view)
			{
				qDebug() << "3";
				FileSelectWidget* sl = (FileSelectWidget*) sender();
				qDebug() << view->getViewFile();
				sl->setFile_path( view->getViewFile() );
			}
		}
	}
}

void CVPanel::slotPreview()
{
	FileSelectWidget* sl = (FileSelectWidget*) sender();
	PictureView* _pictureView = new PictureView(this);
	_pictureView->installEventFilter(this);
	//ui->mdiArea->addSubWindow(view);
	_pictureView->setFile( sl->file_path());

	 addSubWindow(_pictureView);
	_pictureView->show();

}

void CVPanel::on_functionList_clicked(const QModelIndex &index)
{
	//
	QString cmd = ui->functionList->currentItem()->text();

}

void CVPanel::process(QString cmd)
{
	Tourist t;
	CVFunction cv;
	Mat m;

	QString str1 = ui->fileWidget1->file_path();
	QString str2 = ui->fileWidget2->file_path();

	QStringList file_lt;
	file_lt << str1<<str2;

	if( str1 == "")
	{
		QMessageBox::warning(this,"Files","At least one file Required ");
		return;
	}

	try {

		if( cmd.compare("Image_Stitching",Qt::CaseInsensitive) == 0 )
		{
			if(str2 == "" )
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}
			m = Image_Stitching(str1,str2);
		}
		else if( cmd.compare("Advanced_Edge_Detection",Qt::CaseInsensitive) == 0 )
		{
			m = Advanced_Edge_Detection( str1,"/rong/tmp");
		}
		else if( cmd.compare("Colour_Segmentation",Qt::CaseInsensitive) == 0 )
		{
			m = Colour_Segmentation(str1);
		}
		else if( cmd.compare("Colour_Transfer",Qt::CaseInsensitive) == 0 )
		{
			if(str2 == "" )
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}

			auto out = Colour_Transfer( file_lt);
			foreach(auto m, out)
				displayResult(m);
			return;
		}
		else if( cmd.compare("Contrast_Enhancement",Qt::CaseInsensitive) == 0 )
		{
			m = Contrast_Enhancement( str1);
		}
		else if( cmd.compare("Edge_Detection",Qt::CaseInsensitive) == 0 )
		{
			m = Edge_Detection( str1);
		}
		else if( cmd.compare("Histogram_Equalization",Qt::CaseInsensitive) == 0 )
		{
			m = Histogram_Equalization( str1);
		}
		else if( cmd.compare("Harris_Corner_Detector",Qt::CaseInsensitive) == 0 )
		{
			m = Harris_Corner_Detector( str1);
		}
		else if( cmd.compare("Image_Sharpening",Qt::CaseInsensitive) == 0 )
		{
			m = Image_Sharpening( str1);
		}
		else if( cmd.compare("Image_Smoothing",Qt::CaseInsensitive) == 0 )
		{
			if(str2 == "" )
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}

			m = Image_Smoothing( file_lt);
		}
		else if( cmd.compare("Image_Stitching",Qt::CaseInsensitive) == 0 )
		{
			if(str2 == "" )
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}

			m = Image_Stitching( file_lt);
		}
		else if( cmd.compare("Point_Operations_on_Digital_Images",Qt::CaseInsensitive) == 0 )
		{
			m = Point_Operations_on_Digital_Images( str1);
		}
		else if( cmd.compare("seam_carving",Qt::CaseInsensitive) == 0 )
		{
			m = seam_carving( str1);
		}
		else if( cmd.compare("Text_Read",Qt::CaseInsensitive) == 0 )
		{
			m = seam_carving( str1);
		}
		else if( cmd.compare("make_rect_circle",Qt::CaseInsensitive) == 0 )
		{
			m = make_rect_circle( str1);
		}
		else if( cmd.compare("find_sharp",Qt::CaseInsensitive) == 0)
		{

			Mat d = imread(str1.toStdString());
			m = cv.find_sharp2(d);
		}
		else if(cmd.compare("addImage",Qt::CaseInsensitive) == 0)
		{
			if( str2 == "")
				{
					QMessageBox::warning(this,"Files","Required two files");
					return;
				}
			m =t.addImage(str1,str2,ui->sliderA->value() / 100.00, ui->sliderB->value()/ 100.00);
		}
		else if(cmd.compare("Candy",Qt::CaseInsensitive) == 0)
		{
			m = t.onCandy(str1,ui->sliderA->value());
		}
		else if(cmd.compare("Seamless_Clone_Color",Qt::CaseInsensitive) == 0)
		{
			if( str2 == "")
				{
					QMessageBox::warning(this,"Files","Required two files");
					return;
				}
			m =t.Seamless_Clone(str1,str2,ui->sliderA->value());
		}

		else if(cmd.compare("Seamless_Clone_Mask",Qt::CaseInsensitive) == 0)
		{
			if( ui->fileWidget2->file_path() == "")
			{
				QMessageBox::warning(this,"Files","Required two files");
				return;
			}
			m =t.Seamless_Clone(str1,str2, ui->fileWidget3->file_path());
		}
		else if( cmd.compare("Advanced_Edge_Detection_1",Qt::CaseInsensitive) == 0 )
		{
			m = t.Advanced_Edge_Detection( str1,3,ui->sliderA->value(),ui->sliderB->value());
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


bool CVPanel::eventFilter(QObject *watched, QEvent *event)
{
	if(watched == _pictureView && event->type() == QEvent::Close)
	{
		_pictureView = nullptr;
	}
	return false;
}
