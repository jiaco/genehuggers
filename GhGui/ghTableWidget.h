#ifndef	GH_TABLE_WIDGET_H
#define	GH_TABLE_WIDGET_H	1
#include <QtGui>
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT TableWidget	:	public	QTableWidget
{
	Q_OBJECT

public:
	TableWidget( QWidget* parent = 0 );
	void	setupRowsAndColumns( const int& nRows, const QStringList& hdr );
	//void	attachContextMenu( QMenu* m );
	//void	contextMenuEvent( QContextMenuEvent* event );

private:
	//QMenu	*_contextMenu;
};
}	//	GH namespace
#endif	//	GH_TABLE_WIDGET_H
