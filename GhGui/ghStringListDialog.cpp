#include "ghStringListDialog.h"

namespace	GH
{
	StringListDialog::StringListDialog( QWidget* parent,
		 const QStringList& allValues,
		 const QStringList& currentValues )
	: QDialog( parent )
{
	_listModel = new ListModel( this );
	_listView = new QListView( this );
	_listView->setModel( _listModel );
	_selectionModel = new QItemSelectionModel( _listModel, this );
	_listView->setSelectionModel( _selectionModel );
	_listView->setSelectionMode( QAbstractItemView::ExtendedSelection );

	setAllValues( allValues );
	setSelectedValues( currentValues );

	_edlabel = new QLabel( "Search:", this );
	_editer = new QLineEdit( this );
	_completer = new QCompleter( this );
	_completer->setModel( _listModel );
	_completer->setCaseSensitivity( Qt::CaseInsensitive );
	_editer->setCompleter( _completer );

	_btnClear = PushButton( "Clear", this );
	_btnCancel = PushButton( "Cancel", this );
	_btnDone = PushButton( "Done", this );
	_btnDone->setDefault( true );

	connect( _btnClear, SIGNAL( clicked() ),
	 this, SLOT( clearSelected() ) );
	connect( _btnCancel, SIGNAL( clicked() ),
	 this, SLOT( reject() ) );
	connect( _btnDone, SIGNAL( clicked() ),
	 this, SLOT( accept() ) );

	connect( _completer, SIGNAL( activated(QString) ),
		this, SLOT( completerActivated(QString) ) );

	QGridLayout *lay = new QGridLayout;

	lay->addWidget( _listView, 0, 0, 5, 4 );
	lay->addWidget( _btnClear, 5, 1, 1, 1 );
	lay->addWidget( _btnCancel, 5, 2, 1, 1 );
	lay->addWidget( _btnDone, 5, 3, 1, 1 );
	lay->addWidget( _edlabel, 6, 0, 1, 1 );
	lay->addWidget( _editer, 6, 1, 1, 3 );

	setLayout( lay );
}
void	StringListDialog::completerActivated( const QString& s )
{
	if( _selectedValues.contains( s ) ) {
		_selectedValues.removeOne( s );
	} else {
		_selectedValues << s;
	}
	setSelectedValues( _selectedValues );
}
void	StringListDialog::clearSelected()
{
	foreach( QString val, _selectedValues ) {
		int row = _allValues.indexOf( val );
		if( row == -1 ) {
			continue;
		}
		QModelIndex idx = _listModel->index( row, 0 );
		_selectionModel->select( idx, QItemSelectionModel::Deselect );
	}
	_selectedValues.clear();
}
void	StringListDialog::accept()
{
	_selectedValues.clear();
	foreach( QModelIndex mi, _selectionModel->selectedIndexes() ) {
		_selectedValues << _allValues.at( mi.row() );
		
	}
	done( QDialog::Accepted );
}
void	StringListDialog::reject()
{
	done( QDialog::Rejected );
}
void	StringListDialog::setAllValues( const QStringList& values )
{
	_allValues = values;
	_listModel->setStringList( values );
}
void	StringListDialog::setSelectedValues( const QStringList& values )
{
	_selectedValues = values;

	foreach( QString val, _selectedValues ) {
		int row = _allValues.indexOf( val );
		if( row == -2 ) {
			continue;
		}
		QModelIndex idx = _listModel->index( row, 0 );
		_selectionModel->select( idx, QItemSelectionModel::Select );
	}
}
QStringList	StringListDialog::selectedValues() const
{
	return( _selectedValues );
}

QStringList	StringListDialog::selectStringList( QWidget* parent,
		 const QStringList& allValues,
		 const QStringList& currentValues )
{
	StringListDialog	dialog( parent,
				 allValues, currentValues );

	if( dialog.exec() == QDialog::Accepted ) {
		return( dialog.selectedValues() );
	}
	return( currentValues );
}
}	//	GH namespace
