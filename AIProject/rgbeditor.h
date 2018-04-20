#ifndef RGBEDITOR_H
#define RGBEDITOR_H
/*********
 * *                                                                                   *
 *	   **************************************************************************      *
 *	   **            Company Confidential - For Internal Use Only              **      *
 *	   **                  DF/Net Software ULC                   	           **      *
 * 1, allow user set colors- parameters : color and default_color
 * 2,  A) Color is valid, input box display the color’s name like #FFFFFF and show the color in the button.
	   B) Color is invalid, input box show nothing and show default_color in button
 * 3, when user delete whole string in input box, set color to invalid color.
 * 4, when input 7 characters with “#" or 6 characters without “#”, set the value to color.
 * 5, input less then 7 characters with “#" or 6 characters without “#”, no changes.
 * 6, when finished input, if color is valid, reset input box with the color’s name or keep it as blank.
 * 7, emit a signal when color is changed.
 * *	   **                          Copyright 2017                              **      *

 ***********/
#include <QWidget>

#include "edcspec.h"

namespace Ui {
class RGBEditor;
}

class DF_DECLSPEC RGBEditor : public QWidget
{
	Q_OBJECT

public:
	explicit RGBEditor(QWidget *parent = 0);
	~RGBEditor();
	QColor color();
	void setColor(QColor clr,QColor defualt_clr);
	void setEditAble(bool bEdit);
	bool editAble();
private slots:
	void slotTextChanged();
	void slotTextFinished();
	void slotBtnClicked();
signals:
	void colorChanged(QColor );
private:
	Ui::RGBEditor *ui;
	QColor _clr;
	QColor _default;
};

#endif // RGBEDITOR_H
