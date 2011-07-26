#ifndef	GH_TABLE_VIEW_H
#define	GH_TABLE_VIEW_H	1
#include <QtGui>
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT TableView	:	public	QTableView
{
	Q_OBJECT

public:
	TableView( QWidget* parent = 0 );
};
}	//	GH namespace
#endif	//	GH_TABLE_VIEW_H
