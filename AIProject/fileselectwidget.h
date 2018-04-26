#ifndef FILESELECTWIDGET_H
#define FILESELECTWIDGET_H

#include <QWidget>

namespace Ui {
class FileSelectWidget;
}

class FileSelectWidget : public QWidget
{
	Q_OBJECT

public:
	explicit FileSelectWidget(QWidget *parent = 0);
	~FileSelectWidget();

	QString file_path() const;
	void setFile_path(const QString &file_path);

	QString label_text() const;
	void setLabel_text(const QString &label_text);

private slots:
	void on_toolButton_clicked();

	void on_btnPreview_clicked();

	void on_btnFromWidget_clicked();


signals:
	void preview();
	void useCurrentPic();
private:
	Ui::FileSelectWidget *ui;

	QString _file_path;
	QString _label_text;
};

#endif // FILESELECTWIDGET_H
