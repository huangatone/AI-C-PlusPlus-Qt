#ifndef DFPIXMAPWDIGET_H
#define DFPIXMAPWDIGET_H

#include <QWidget>

namespace Ui {
class DFPixmapWdiget;
}

class DFPixmapWdiget : public QWidget
{
	Q_OBJECT

public:
	explicit DFPixmapWdiget(QWidget *parent = 0);
	~DFPixmapWdiget();

	QString getFileNmae();

	void setImage(QImage &img);
	void setImage(QPixmap &img);

private slots:
	void on_btnOpen_clicked();

	void on_toolButton_clicked();

private:
	Ui::DFPixmapWdiget *ui;
};

#endif // DFPIXMAPWDIGET_H
