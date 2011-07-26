#ifndef	GH_HEADERVIEW_H
#define	GH_HEADERVIEW_H	1
#include <QtGui>
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT HeaderView	: public QHeaderView
{
	Q_OBJECT

public:
	HeaderView( Qt::Orientation orientation, QWidget* parent = 0 );

	void	contextMenuEvent( QContextMenuEvent *e );
};
}	//	GH namespace
#endif	//	GH_HEADERVIEW_H
