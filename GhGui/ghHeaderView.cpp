#include "ghHeaderView.h"

namespace	GH
{
	HeaderView::HeaderView( Qt::Orientation orientation, QWidget* parent )
	: 	QHeaderView( orientation, parent )
{
	setClickable( true );
}

void	HeaderView::contextMenuEvent( QContextMenuEvent *e )
{
	qDebug() << "Houston ";
	QHeaderView::contextMenuEvent( e );
}

}	//	GH namespace
