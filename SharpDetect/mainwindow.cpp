#include <tesseract/baseapi.h>
#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>
#include <leptonica/allheaders.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>

#include "cvfunction.h"
#include <QDebug>

#include "cvdialog.h"
#include "car.h"
#include "mlfunction.h"
#include "textareadetect.h"





int MainWindow::_fun_index = 0;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//load ini file
    QFile ini( "/home/parallels/devel/mine/cv.ini");
	ini.open(QIODevice::ReadOnly);
	QTextStream iniS(&ini);
    QString all = iniS.readAll();
    QStringList lt = all.split("\n");
	ui->comboBox->addItems(lt);


	QString tempFileDir = QDir::tempPath();
		if (!tempFileDir.endsWith(QDir::separator()))
				tempFileDir += QDir::separator();
		qDebug() << tempFileDir;

	MLFunction ml;
	//ml.buildAll();
	TextAreaDetect dt;
    //dt.work();

    //新建tess基类

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
        // Initialize tesseract-ocr with English, without specifying tessdata path
        if (api->Init("/home/parallels/devel/lib/tessdata", "eng")) {
            qDebug() <<  "Could not initialize tesseract.\n";
            exit(1);
        }

        // Open input image with leptonica library
        Pix *image = pixRead("/home/parallels/devel/mine/tmp/abc.png");
        api->SetImage(image);
        char *outText;
        // Get OCR result
            outText = api->GetUTF8Text();
            qDebug() << "OCR output:\n%s" << outText;

            // Destroy used object and release memory
            api->End();
            delete [] outText;
            pixDestroy(&image);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_toolButton_clicked()
{
	CvDialog dlg;
	////find edge
	//ui->label_2->setPixmap( QPixmap::fromImage( cd.find_edge(255,100,srcImage)));

	// find sharp
	_fun_index = 0;
	dlg.setCallbackFunc( do_findSharp);
	dlg.exec();
	return;


}


void MainWindow::find()
{	
}




QImage MainWindow::do_findSharp(Mat src)
{
	CVFunction cd;
	//switch _fun_index to:
	//return cd.find_edge(255,100,src);
	return cd.find_sharp2(src);
}

//do search.
void MainWindow::on_toolButton_2_clicked()
{
	MLFunction car;
	car.doWork(ui->lineEdit->text().toStdString());
}

//trainning
void MainWindow::on_toolButton_3_clicked()
{
	MLFunction car;
	car.train();
}

void MainWindow::on_toolButton_4_clicked()
{
	MLFunction car;
	car.findNumber();
}
