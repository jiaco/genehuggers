#include "ghTableDialog.h"

/*
	close event on parent is not getting propogated to this
 */

namespace	GH
{
	TableDialog::TableDialog( QWidget* parent )
	 : QDialog( parent, Qt::Window )
{
	_tableModel = new TableModel( parent );
	_tableView = new TableView( parent );
	_tableView->setModel( _tableModel );

	_scrollArea = new QScrollArea( parent );

	_scrollArea->setWidget( _tableView );
	_scrollArea->setWidgetResizable( true );


	_btnClose = PushButton( "Close", this );
/*
	_btnSave = PushButton( "Save As", this );
	_btnExcel = PushButton( "Excel", this );
*/

	connect( _btnClose, SIGNAL( clicked() ),
	 this, SLOT( reject() ) );
/*
	connect( _btnSave, SIGNAL( clicked() ),
	 this, SLOT( doSave() ) );
	connect( _btnExcel, SIGNAL( clicked() ),
	 this, SLOT( doExcel() ) );
*/

	_layout = new QGridLayout;

	_layout->addWidget( _scrollArea, 0, 0, 1, 2 );
	_layout->setRowStretch( 0, 1 );
	_layout->addWidget( _btnClose, 1, 1 );
/*
	lay->addWidget( _btnSave, 1, 2 );
	lay->addWidget( _btnExcel, 1, 3 );
*/
	setLayout( _layout );
}
void	TableDialog::reject()
{
	done( QDialog::Rejected );
}
/*
bool	TableDialog::saveAs( const QString& path )
{
	
}
QString	TableDialog::getSavePath()
{
	QString	path = QFileDialog::getSaveFileName( this,
		"Select a file to save as",
}
bool	TableDialog::doSave()
{
	QString	path = getSavePath();
	if( !path.isEmpty() ) {
		saveAs( path );
	}
}
bool	TableDialog::doExcel()
{
	QString	path = getSavePath();
	if( !path.isEmpty() ) {
		saveAs( path );
		OpenWith( path );
	}
}
*/
void	TableDialog::setData( const QStringList& hdr, const QList<Row>& data )
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
void	TableDialog::setData( const QList<QMap<QString,QVariant> >& data )
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
void	TableDialog::setData( const QStringList& header,
	 const QList<QList<QVariant> >& data )
{
	_tableModel->beginChange();
	_tableHeader = header;
	_tableData = data;
	_tableModel->setExternalHeader( &_tableHeader );
	_tableModel->setExternalData( &_tableData );
	_tableModel->endChange();
}
void	TableDialog::ShowTable( const QStringList& hdr, const QList<Row>& data,
			const QString& title, QWidget* parent )
{
	TableDialog *dialog = new TableDialog( parent );
	dialog->setWindowTitle( title );
	dialog->setData( hdr, data );
	dialog->show();
/*
	if( dialog.isModal() ) {
		if( dialog.exec() == QDialog::Accepted ) {
			return;
		}
	} else {
		dialog.show();
	}
*/
	return;
}
void	TableDialog::ShowTable( const QList<QMap<QString,QVariant> >& data,
			QWidget* parent )
{
	TableDialog *dialog = new TableDialog( parent );
	dialog->setData( data );
	dialog->show();
/*
	if( dialog.isModal() ) {
		if( dialog.exec() == QDialog::Accepted ) {
			return;
		}
	} else {
		dialog.show();
	}
*/
	return;
}
}	//	GH namespace
