#include "stylewindow.h"
#include "ui_stylewindow.h"
#include <QComboBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "cvfunction.h"
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QMdiSubWindow>
#include "pictureview.h"
#include "cvpictureview.h"
#include "dealview.h"
#include "tourist.h"

StyleWindow::StyleWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::StyleWindow)
{
	ui->setupUi(this);
	qApp->setStyleSheet("QMainWindow { background-color: rgba(33, 65, 10, 255);color: rgb(255, 255, 255); }"
						"QPushButton { background-color: rgba(33, 65, 10, 255);color: rgb(255, 255, 255);}"
						"QToolBar { background-color: rgba(33, 65, 10, 255);color: rgb(255, 255, 255);}"
						"QMenuBar { background-color: rgba(33, 65, 10, 255);color: rgb(255, 255, 255);}"
						"QLabel { background-color: rgba(33, 65, 10, 255);color: rgb(255, 255, 255);}"
						"QToolButton { background-color: rgba(33, 65, 10, 255);color: rgb(255, 255, 255);}");

	//setStyleSheet("background-color: rgba(33, 90, 29, 255);color: rgb(255, 255, 255);");

	auto co = new QComboBox(this);
	co->addItems( QStringList() << "Pro 1" << "Pro 2" << "Pro 3");

	ui->toolBar->addWidget( co);
	QWidget* blank_w = new QWidget (this);
	blank_w->resize(50,20);
	blank_w->setStyleSheet("background-color: rgba(33, 90, 29, 255);");
	blank_w->setMinimumWidth(50);
	ui->toolBar->insertWidget( ui->actionOpen,blank_w);

	connect( ui->iconbtn1, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));
	connect( ui->iconbtn2, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));
	connect( ui->iconbtn3, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));
	connect( ui->iconbtn4, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));
	connect( ui->iconbtn5, SIGNAL(clicked()), this, SLOT(slot_icon_buttun_clicked()));

	QFile f("opencv.json");
	if(f.open(QIODevice::ReadOnly))
	{
		initButtons( f.readAll());
	}

	DealView *pView = new DealView (this);
	auto a = ui->mdiArea->addSubWindow(pView);
	a->setWindowTitle("Test View");
	pView->show();

	ui->treeWidget->setColumnWidth(0,200);

}

StyleWindow::~StyleWindow()
{
	delete ui;
}

void StyleWindow::initButtons(QString str)
{
	qDebug() <<"my god";
	QJsonDocument doc = QJsonDocument::fromJson( str.toUtf8());
	auto obj = doc.object();
	auto btn_objs = obj["buttons"];
	auto btn_arr = btn_objs.toArray();

	QList<IconButton*> lt_iconbtn;
	lt_iconbtn << ui->iconbtn1 << ui->iconbtn2 <<ui->iconbtn3<<ui->iconbtn4<< ui->iconbtn5;

	for(int i=0; i< btn_arr.count(); i++)
	{
		auto sub_obj =  btn_arr[i].toObject();
		qDebug() << sub_obj.value("text").toString() << sub_obj.value("icon").toString();

		lt_iconbtn[i]->SetText(	sub_obj.value("text").toString());
		lt_iconbtn[i]->SetIcon( QPixmap(sub_obj.value("icon").toString()));

	}

	auto tree_objs = obj["trees"];

	auto tree_arr = tree_objs.toArray();


	for(int i=0; i< tree_arr.count(); i++)
	{
		auto sub_obj =  tree_arr[i].toObject();
		QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
		item->setText(0,	sub_obj.value("text").toString());
		item->setToolTip(0,	sub_obj.value("tooltip").toString());
	}

	auto colors = obj["colors"].toObject();
	QString clr = "%1{ background-color: rgba(%2);color: rgb(%3);}";
	QString style = "QMainWindow { %1 }"
					"QPushButton { %2 }"
					"QToolBar { %3}"
					"QMenuBar { %4}"
					"QLabel { %5}"
					"QToolButton { %6}";
	QString bk = colors.value("background").toString();
	QString fk =colors.value("forecolor").toString();
	QString style_str = clr.arg("QMainWindow").arg(bk).arg(fk);
	style_str += clr.arg("QPushButton").arg(bk).arg(fk);
	style_str += clr.arg("QToolBar").arg(bk).arg(fk);
	style_str += clr.arg("QMenuBar").arg(bk).arg(fk);
	style_str += clr.arg("QLabel").arg(bk).arg(fk);
	style_str += clr.arg("QToolButton").arg(bk).arg(fk);

	//style_str = clr.arg("QMainWindow").arg(colors.value("background").toString()).arg(colors.value("forecolor").toString());


	//QString clr_value = clr.arg( clr.arg(colors.value("background").toString()).arg(colors.value("forecolor").toString()));
	//style = style.arg( clr_value).arg( clr_value).arg( clr_value).arg( clr_value).arg( clr_value).arg( clr_value).arg( clr_value);
qDebug() << style_str;
	qApp->setStyleSheet(style_str);
}

void StyleWindow::initTree(QString ) {}

void StyleWindow::initButtons(QString , int i) {}

void StyleWindow::initTree(QString , int i) {}


void StyleWindow::slot_icon_buttun_clicked()
{
	IconButton* btn = qobject_cast<IconButton*>( sender());
	if(btn)
		qDebug() << btn->getText() << "  Clicked ";
	if(btn == ui->iconbtn1)
	{
		DealView *pView = new DealView (this);
		auto a = ui->mdiArea->addSubWindow(pView);
		a->setWindowTitle("Test View");
		pView->show();
	}
	if(btn == ui->iconbtn2)
	{
		CVPictureView *pView = new CVPictureView (this);
		auto a = ui->mdiArea->addSubWindow(pView);
		a->setWindowTitle("Test View");
		pView->show();
	}
	if(btn == ui->iconbtn3)
	{
		PictureView *pView = new PictureView (this);
		auto a = ui->mdiArea->addSubWindow(pView);
		a->setWindowTitle("Test View");
		pView->show();
	}
	if(btn == ui->iconbtn4)
	{
		DealView *pView = new DealView (this);
		auto a = ui->mdiArea->addSubWindow(pView);
		a->setWindowTitle("Test View");
		pView->show();
	}
	if(btn == ui->iconbtn5)
	{
		QFile f("opencv.json");
		if(f.open(QIODevice::ReadOnly))
		{
			initButtons( f.readAll());
		}
	}
}

void StyleWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int )
{
	auto txt = item->text(0);
	auto s = ui->mdiArea->currentSubWindow();
	s->setWindowTitle( txt);
	auto w = s->widget();
	DealView* d_w = dynamic_cast<DealView*>(w);
	if(d_w)
		d_w->setCmd( txt );
}

void StyleWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int )
{
	auto txt = item->text(0);
	auto s = ui->mdiArea->currentSubWindow();
	s->setWindowTitle( txt);
	auto w = s->widget();
	DealView* d_w = dynamic_cast<DealView*>(w);
	if(d_w)
		d_w->setCmd( txt );
}

void StyleWindow::onImage_Stitching(QString file_name1, QString file_name2)
{
	if(file_name1 == "" || file_name2 =="")
	{
		QMessageBox::warning(this,"Files","Required two files");
		return;
	}
	auto out = Image_Stitching(file_name1,file_name2);
	displayResult(out);
}

void StyleWindow::onAdvanced_Edge_Detection(QString file_name, QString dir_name)
{
	auto out = Advanced_Edge_Detection( file_name,  dir_name);
	displayResult(out);
}

void StyleWindow::onColour_Segmentation(QString file_name)
{
	auto out = Colour_Segmentation(file_name);
	displayResult(out);
}

void StyleWindow::onColour_Transfer(QStringList file_lt)
{
	auto out = Colour_Transfer( file_lt);
	foreach(auto m, out)
		displayResult(m);
}

void StyleWindow::onContrast_Enhancement(QString file_name)
{
	auto out = Contrast_Enhancement( file_name);
	displayResult(out);
}

void StyleWindow::onEdge_Detection(QString file_name)
{
	auto out = Edge_Detection( file_name);
	displayResult(out);
}

void StyleWindow::onHistogram_Equalization(QString f_name)
{
	auto out = Histogram_Equalization( f_name);
	displayResult(out);
}

void StyleWindow::onHarris_Corner_Detector(QString f_name)
{
	auto out = Harris_Corner_Detector( f_name);
	displayResult(out);
}

void StyleWindow::onImage_Sharpening(QString f_name)
{
	auto out = Image_Sharpening( f_name);
	displayResult(out);
}

void StyleWindow::onImage_Smoothing(QStringList f_lt)
{
	if(f_lt.count() != 2)
	{
		QMessageBox::warning(this,"Files","Required two files");
		return;
	}
	auto out = Image_Smoothing( f_lt);
	displayResult(out);
}

void StyleWindow::onImage_Stitching(QStringList f_lt)
{
	auto out = Image_Stitching( f_lt);
	displayResult(out);
}

void StyleWindow::onPoint_Operations_on_Digital_Images(QString f_n)
{
	auto out = Point_Operations_on_Digital_Images( f_n);
	displayResult(out);
}

void StyleWindow::onseam_carving(QString f_n)
{
	auto out = seam_carving( f_n);
	displayResult(out);
}

void StyleWindow::on_btnDo_clicked()
{
	auto cmd = ui->treeWidget->currentItem()->text(0);
	auto t1 = ui->lbFile1->text();
	auto t2 = ui->lbFile2->text();
	auto t3 = ui->lbFile3->text();
	auto t4 = ui->lbFile4->text();
	if( cmd.compare("Image_Stitching",Qt::CaseInsensitive) == 0 )
	{
		onImage_Stitching(t1,t2);
	}
	else if( cmd.compare("Advanced_Edge_Detection",Qt::CaseInsensitive) == 0 )
	{
		onAdvanced_Edge_Detection(t1,t4);
	}
	else if( cmd.compare("Colour_Segmentation",Qt::CaseInsensitive) == 0 )
	{
		onColour_Segmentation(t1);
	}
	else if( cmd.compare("Colour_Transfer",Qt::CaseInsensitive) == 0 )
	{
		onColour_Transfer(QStringList() << t1<<t2);
	}
	else if( cmd.compare("Contrast_Enhancement",Qt::CaseInsensitive) == 0 )
	{
		onContrast_Enhancement(t1);
	}
	else if( cmd.compare("Edge_Detection",Qt::CaseInsensitive) == 0 )
	{
		onEdge_Detection(t1);
	}
	else if( cmd.compare("Histogram_Equalization",Qt::CaseInsensitive) == 0 )
	{
		onHistogram_Equalization(t1);
	}
	else if( cmd.compare("Harris_Corner_Detector",Qt::CaseInsensitive) == 0 )
	{
		onHarris_Corner_Detector(t1);
	}
	else if( cmd.compare("Image_Sharpening",Qt::CaseInsensitive) == 0 )
	{
		onImage_Sharpening(t1);
	}
	else if( cmd.compare("Image_Smoothing",Qt::CaseInsensitive) == 0 )
	{
		onImage_Smoothing(QStringList() << t1<<t2);
	}
	else if( cmd.compare("Image_Stitching",Qt::CaseInsensitive) == 0 )
	{
		onImage_Stitching(t1,t2);
	}
	else if( cmd.compare("Point_Operations_on_Digital_Images",Qt::CaseInsensitive) == 0 )
	{
		onPoint_Operations_on_Digital_Images(t1);
	}
	else if( cmd.compare("seam_carving",Qt::CaseInsensitive) == 0 )
	{
		onseam_carving(t1);
	}
	else if( cmd.compare("Text_Read",Qt::CaseInsensitive) == 0 )
	{
		onseam_carving(t1);
	}
	else if( cmd.compare("make_rect_circle",Qt::CaseInsensitive) == 0 )
	{
		//onGetArea(t1);

		auto out = make_rect_circle( t1);
		displayResult(out);
	}
	else if( cmd.compare("find_sharp",Qt::CaseInsensitive) == 0)
	{
		CVFunction cv;
		Mat m = imread(t1.toStdString());
		auto d = cv.find_sharp2(m);
		displayResult(d, "sharp2");
		//d = cv.find_sharp(m);
		//displayResult(d, "sharp1");
		//QMessageBox::warning(this, "Error", "Not such functions");
	}
	else if(cmd.compare("Seamless",Qt::CaseInsensitive) == 0)
	{
		Tourist t;
		//t.test("rong/testfile/p1.png","rong/testfile/dst.png","rong/testfile/src.png");
		t.Seamless_Clone(t1,t2,t3);
	}
	else
	{

	}

}

void StyleWindow::on_btnFile1_clicked()
{
	setFileName( ui->lbFile1);
}

void StyleWindow::on_btnFile2_clicked()
{
	setFileName( ui->lbFile2);
}

void StyleWindow::on_btnFile3_clicked()
{
	setFileName( ui->lbFile3);
}

void StyleWindow::on_btnFile4_clicked()
{
	auto fi = QFileDialog::getExistingDirectory(this,"Select Folder","");
	if(fi == "")
		return;
	ui->lbFile4->setText(fi);
	//setFileName( ui->lbFile4);

	QString js = R"delimiter({"buttons":[{ "text":"Project", "icon":32 },{ "text":"Function", "icon":33 },
				 { "text":"Admin", "icon":31 },{ "text":"Explore", "icon":34 }],"tree":[{ "text":"Project", "icon":32 },{ "text":"Function", "icon":33 },
				 { "text":"Admin", "icon":31 },{ "text":"Explore", "icon":34 }]} )delimiter";
}
void StyleWindow::setFileName(QLabel* p)
{
	auto fi = QFileDialog::getOpenFileName(this,"Select Image","","Images (*.png *.jpeg *.jpg)");
	if(fi == "")
		return;
	p->setText(fi);

	PictureView* view = new PictureView(this);
	view->setFile(fi);

	ui->mdiArea->addSubWindow(view);
	view->show();
}

void StyleWindow::displayResult(Mat &m,QString title)
{
	QScrollArea *area = new QScrollArea (this);
	area->setWidgetResizable(false);
	QLabel * lb = new QLabel (this);

	imwrite(title.toStdString() + ".png",m);
	QPixmap pix("tmp.png");

	lb->setPixmap( pix);

	area->setWidget(lb);

	auto a = ui->mdiArea->addSubWindow(area);
	a->setWindowTitle(title);
	area->show();
}

void StyleWindow::on_horizontalScrollBar_sliderMoved(int position)
{

}

void StyleWindow::addSubWindow(QWidget* w)
{
	ui->mdiArea->addSubWindow(w);
}
