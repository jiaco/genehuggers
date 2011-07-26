#include "ghTableModel.h"

namespace	GH
{
	SortObject::SortObject( const int& column, const Qt::SortOrder& order )
{
	_column = column;
	_order = order;
}
bool	SortObject::operator()( const QList<QVariant>& e1, const QList<QVariant>& e2 )
{
	QVariant::Type _type = e1.at( _column ).type();
	bool	rv = false;

	if( _type == QVariant::String ) {
		if( _order == Qt::DescendingOrder ) {
			rv = S( e1.at( _column ) ) > S( e2.at( _column ) );
		} else {
			rv = S( e1.at( _column ) ) < S( e2.at( _column ) );
		}
	} else {
		if( _order == Qt::DescendingOrder ) {
			rv = D( e1.at( _column ) ) > D( e2.at( _column ) );
		} else {
			rv = D( e1.at( _column ) ) < D( e2.at( _column ) );
		}
	}
	return( rv );
}
	TableModel::TableModel( QObject* parent )
	: QAbstractTableModel( parent )
{
	_data = 0;
	_header = 0;
	_editable = false;
}
void	TableModel::beginChange()
{
	emit( beginResetModel() );
}
void	TableModel::endChange()
{
	emit( endResetModel() );
}
//void	TableModel::setExternalData( TableData *data )
void	TableModel::setExternalData( QList<QList<QVariant> > *data )
{
	_data = data;
}
void	TableModel::setExternalHeader( QStringList *header )
{
	_header = header;
}
void	TableModel::setExternalVerticalHeader( QStringList *header )
{
	_verticalHeader = header;
}
int	TableModel::rowCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	int	rv = 0;

	if( _data ) {
		rv = _data->size();
	}
	return( rv );
}
int	TableModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	int	rv = 0;
	if( _data && _data->size() > 0 ) {
		rv = _data->at( 0 ).size();
	}
	return( rv );
}
QVariant	TableModel::data( const QModelIndex& index, int role ) const
{
	QVariant	rv;

	int	r, c;
	if( !index.isValid() ) {
		return( rv );
	}
	r = index.row();
	c = index.column();

	if( r < rowCount() && c < columnCount() ) {
		if( role == Qt::DisplayRole ) {
			rv = _data->at( r ).at( c );
		}
	}
	return( rv );
}
QVariant	TableModel::headerData( int section,
		 Qt::Orientation orientation, int role ) const
{
	QVariant rv;

	if( role == Qt::DisplayRole ) {
		if( orientation == Qt::Horizontal ) {
			rv = _header->at( section );
		} else if( orientation == Qt::Vertical ) {
			if( _verticalHeader->size() > 0 &&
			 section < _verticalHeader->size() ) {
				rv = _verticalHeader->at( section );
			}
		}
/*
		} else if( orientation == Qt::Vertical ) {
			rv = _data->at( section ).at( 0 );
		}
*/
	} else if( role == Qt::ToolTipRole ) {
		if( orientation == Qt::Horizontal ) {
			rv = _header->at( section );
		}
	}

	return( rv );
}
bool	TableModel::editable() const
{
	return( _editable );
}
void	TableModel::setEditable( const bool& tf )
{
	_editable = tf;
}
Qt::ItemFlags	TableModel::flags( const QModelIndex& index ) const
{
	Q_UNUSED( index );

	if( _editable ) {
		return( Qt::ItemIsEnabled | Qt::ItemIsSelectable |
		 Qt::ItemIsEditable );
	} else {
		return( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
	}
}
bool	TableModel::setData( const QModelIndex& index, const QVariant& value,
		int role )
{
	int	r, c;
	if( !index.isValid() ) {
		return( false );
	}
	r = index.row();
	c = index.column();

	if( r < rowCount() && c < columnCount() ) {
		if( role == Qt::EditRole ) {
			(*_data)[ r ][ c ] = value;
			//_data->setValue( r, c, value );
/*
			QVariant v = value;
			_data->at( r ).replace( c, v );
			(*_data).at( r )[ c ] = value;
*/
			emit( dataChanged( index, index ) );
			return( true );
		}
	}
	return( false );
}
/*
bool	TableModel::stringSort( const QList<QVariant>& e1, const QList<QVariant>& e2 )
{
	qDebug() << "ok";
	return( true );
}
*/
void	TableModel::sort( int column, Qt::SortOrder order )
{
	SortObject	so( column, order );
	beginChange();
	qSort( _data->begin(), _data->end(), so );
	endChange();
	reset();
}
}	//	GH namespace
