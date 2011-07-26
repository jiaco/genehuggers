#include "ghListModel.h"

namespace	GH
{
	ListModel::ListModel( QObject* parent )
	: QStringListModel( parent )
{
}
QModelIndex	ListModel::createIndex( int row, int column, void *ptr ) const
{
	Q_UNUSED( ptr );
	return( QAbstractItemModel::createIndex( row, column ) );
}
/*
int	ListModel::rowCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	int rv = 0;
	if( _data ) {
		rv = _data->size();
	}
	return( rv );
}
QVariant	ListModel::data( const QModelIndex& index, int role ) const
{
	QVariant	rv;

	if( !index.isValid() ) {
		return( rv );
	}
	int pos = index.row();
	if( pos < rowCount() ) {
		if( role == Qt::DisplayRole ) {
			rv = QVariant( _data->at( pos ) );
		}
	}
	return( rv );
}
*/
}	//	GH namespace
