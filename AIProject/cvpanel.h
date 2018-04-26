#ifndef CVPANEL_H
#define CVPANEL_H

#include <QWidget>

namespace Ui {
class CVPanel;
}

class CVPanel : public QWidget
{
	Q_OBJECT

public:
	explicit CVPanel(QWidget *parent = 0);
	~CVPanel();

private:
	Ui::CVPanel *ui;
};

#endif // CVPANEL_H
