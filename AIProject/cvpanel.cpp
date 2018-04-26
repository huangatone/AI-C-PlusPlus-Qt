#include "cvpanel.h"
#include "ui_cvpanel.h"

CVPanel::CVPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CVPanel)
{
	ui->setupUi(this);
}

CVPanel::~CVPanel()
{
	delete ui;
}
