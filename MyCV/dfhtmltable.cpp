#include "dfhtmltable.h"

DFHtmlTable::DFHtmlTable()
{
}

void DFHtmlTable::addTR(QString trFormat)
{
	_table << "<tr " + trFormat + "> %1 </tr>";
}

void DFHtmlTable::addTD(int nTR, QString td, QString tdFormat)
{
	QString td_str = "<td %1> %2 </td>";
	td_str = td_str.arg(tdFormat).arg(td);
	if(nTR >= _tr_list.count())
		_tr_list << (QStringList() << td_str);
	else
		_tr_list[nTR] << td_str;

}
