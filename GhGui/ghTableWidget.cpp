#include "ghTableWidget.h"

namespace	GH
{

TableWidget::TableWidget( QWidget* parent )
	: QTableWidget( parent )
{
	setSelectionBehavior( QAbstractItemView::SelectRows );
	// trying to use QWidget::addAction() for now
	//_contextMenu = 0;
}
void	TableWidget::setupRowsAndColumns( const int& nRows,
	 const QStringList& hdr )
{
	setRowCount( nRows );
	setColumnCount( hdr.size() );
	for( int i = 0; i < hdr.size(); ++i ) {
		setHorizontalHeaderItem( i, 
		 new QTableWidgetItem( hdr.at( i ) ) );
	}
}
/*
void	TableWidget::attachContextMenu( QMenu* m )
{
	_contextMenu = m;
}
void	TableWidget::contextMenuEvent( QContextMenuEvent *event )
{
	if( _contextMenu ) {
		_contextMenu->exec( event->globalPos() );
	}
}
*/
}	//	GH namespace
