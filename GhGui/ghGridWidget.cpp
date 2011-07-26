#include "ghGridWidget.h"

namespace	GH
{
	GridWidget::GridWidget( QWidget* parent, Qt::WindowFlags f )
	 : QWidget( parent, f )
{
	_tableModel = new TableModel( parent );
	_tableView = new TableView( parent );
/* this killed sorting...
	REALLY WANT TO SEE IF IT WORKS MINUS NEXT 2 LINES OF CODE
	_headerView = new HeaderView( Qt::Horizontal, parent );
	_tableView->setHorizontalHeader( _headerView );
*/

	_tableView->setSortingEnabled( true );
	_tableView->setModel( _tableModel );

	_scrollArea = new QScrollArea( parent );

	_scrollArea->setWidget( _tableView );
	_scrollArea->setWidgetResizable( true );

	_layout = new QGridLayout;

	_layout->addWidget( _scrollArea, 0, 0 );
	setLayout( _layout );
	setWindowModality( Qt::NonModal );

	connect( _tableView, SIGNAL( clicked(QModelIndex) ),
	 this, SLOT( clickHandler(QModelIndex) ) );
}
void	GridWidget::clickHandler( const QModelIndex& index )
{
	QModelIndex midx = _tableModel->index( index.row(), index.column() );
	emit( clicked( 
	 _tableModel->headerData( midx.row(), Qt::Vertical ),
	 _tableModel->headerData( midx.column(), Qt::Horizontal )
	) );
}
QVariant	GridWidget::dataAt( const int& row, const QString& colName ) const
{
	int	cidx = _tableHeader.indexOf( colName );
	if( cidx < 0 ) {
		return( QVariant() );
	}
	return( _tableData.at( row ).at( cidx ) );
}
TableView*	GridWidget::tableView()
{
	return( _tableView );
}
TableView*	GridWidget::tableView() const
{
	return( _tableView );
}
QStringList	GridWidget::tableHeader() const
{
	return( _tableHeader );
}
/*
void	GridWidget::setVerticalHeader( const QStringList& hdr )
{
	_tableModel->beginChange();
	_verticalHeader = hdr;
	_tableModel->setExternalVerticalHeader( &_verticalHeader );
	_tableModel->endChange();
}
*/
void	GridWidget::setData( const QStringList& hdr, const QList<Row>& data )
{
	_tableModel->beginChange();
	_tableHeader = hdr;
	_tableData.clear();
	for( int r = 0; r < data.size(); ++r ) {
		QList<QVariant> row;
		for( int c = 0; c < hdr.size(); ++c ) {
			row << data.at( r )[ hdr.at( c ) ];
		}
		_tableData << row;
	}
	_tableModel->setExternalHeader( &_tableHeader );
	_tableModel->setExternalData( &_tableData );

	_tableModel->endChange();
}
void	GridWidget::setData( const QStringList& hdr,
	 const QStringList& vhdr, const QList<Row>& data )
{
	_tableModel->beginChange();
	_tableHeader = hdr;
	_verticalHeader = vhdr;
	_tableData.clear();
	for( int r = 0; r < data.size(); ++r ) {
		QList<QVariant> row;
		for( int c = 0; c < hdr.size(); ++c ) {
			row << data.at( r )[ hdr.at( c ) ];
		}
		_tableData << row;
	}
	_tableModel->setExternalHeader( &_tableHeader );
	_tableModel->setExternalVerticalHeader( &_verticalHeader );
	_tableModel->setExternalData( &_tableData );
	_tableModel->endChange();
}
void	GridWidget::setData( const QList<QMap<QString,QVariant> >& data )
{
	// convert first row to a header
	// QStringList for header,
	// QList<QList<QVariant> > for data

	_tableModel->beginChange();
	_tableData.clear();
	_tableHeader.clear();
	if( data.isEmpty() ) {
		return;
	}
	for( int i = 0; i < data.at( 0 ).size(); ++i ) {
		_tableHeader << data.at( 0 ).keys().at( i );
	}
	for( int j = 0; j < data.size(); ++j ) {
		QList<QVariant>	row;
		for( int i = 0; i < data.at( j ).size(); ++i ) {
			row << data.at( j ).values().at( i );
		}
		_tableData << row;
	}
	_tableModel->setExternalHeader( &_tableHeader );
	_tableModel->setExternalData( &_tableData );
	_tableModel->endChange();
}
void	GridWidget::setData( const QStringList& header,
	 const QList<QList<QVariant> >& data )
{
	_tableModel->beginChange();
	_tableHeader = header;
	_tableData = data;
	_tableModel->setExternalHeader( &_tableHeader );
	_tableModel->setExternalData( &_tableData );
	_tableModel->endChange();
}
void	GridWidget::setData( const QStringList& header,
	 const QStringList& vheader,
	 const QList<QList<QVariant> >& data )
{
	_tableModel->beginChange();
	_tableHeader = header;
	_verticalHeader = vheader;
	_tableData = data;
	_tableModel->setExternalHeader( &_tableHeader );
	_tableModel->setExternalVerticalHeader( &_verticalHeader );
	_tableModel->setExternalData( &_tableData );
	_tableModel->endChange();
}
void	GridWidget::ShowTable( const QStringList& hdr, const QList<Row>& data,
			const QString& title, QWidget* parent )
{
	GridWidget *w = new GridWidget( parent, Qt::Window );
	w->setWindowModality( Qt::NonModal );
	w->setWindowTitle( title );
	w->setData( hdr, data );
	w->show();
	return;
}
void	GridWidget::ShowTable( const QStringList& hdr,
	const QStringList& vhdr, const QList<Row>& data,
			const QString& title, QWidget* parent )
{
	GridWidget *w = new GridWidget( parent, Qt::Window );
	w->setWindowModality( Qt::NonModal );
	w->setWindowTitle( title );
	w->setData( hdr, vhdr, data );
	w->show();
	return;
}
void	GridWidget::ShowTable( const QList<QMap<QString,QVariant> >& data,
			QWidget* parent )
{
	GridWidget *w = new GridWidget( parent, Qt::Window );
	w->setWindowModality( Qt::NonModal );
	w->setData( data );
	w->show();
	return;
}
}	//	GH namespace
