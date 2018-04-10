#ifndef DFHTMLTABLE_H
#define DFHTMLTABLE_H

#include <QString>
#include <QStringList>

class DFHtmlTable
{
public:
	DFHtmlTable();

	void addTR(QString trFormat="");
	void addTD(int nTR, QString td, QString tdFormat="");

	QString GetHtml();

private:
	QStringList _table;
	QList<QStringList> _tr_list;

};

#endif // DFHTMLTABLE_H
