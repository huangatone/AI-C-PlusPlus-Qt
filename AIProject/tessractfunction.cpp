#include "tessractfunction.h"
#include <QDebug>

tessractFunction::tessractFunction(QString data_folder, QString lan)
{

	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init("//rong/lib/tessdata", "eng")) {
		qDebug() <<  "Could not initialize tesseract.\n";
		exit(1);
	}

	// Open input image with leptonica library
	Pix *image = pixRead("/rong/tmp/demo.crop.png");
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


