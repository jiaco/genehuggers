#ifndef	GH_TABLE_MODEL
#define	GH_TABLE_MODEL	1
#include <QtGui>
#include "ghDef.h"
#include "ghVariant.h"
//#include "ghTableData.h"

namespace	GH
{
class   SortObject
{
public:
        SortObject( const int& column,
	 const Qt::SortOrder& order = Qt::AscendingOrder );

        bool operator()( const QList<QVariant>& e1, const QList<QVariant>& e2 );
private:
        int     _column;
	Qt::SortOrder	_order;
};
class	GH_DLL_EXPORT TableModel	:	public	QAbstractTableModel
{
	Q_OBJECT

public:
	TableModel( QObject* parent = 0 );

	void	beginChange();
	void	endChange();
	//void	setExternalData( TableData *data );
	void	setExternalData( QList<QList<QVariant> > *data );
	void	setExternalHeader( QStringList	*header );
	void	setExternalVerticalHeader( QStringList	*header );

	int	rowCount( const QModelIndex& parent = QModelIndex() ) const;
	int	columnCount( const QModelIndex& parent = QModelIndex() ) const;
	QVariant	data( const QModelIndex& index,
			 int role = Qt::DisplayRole ) const;
	QVariant	headerData( int section, Qt::Orientation orientation,
			 int role = Qt::DisplayRole ) const;

	bool	editable() const;
	void	setEditable( const bool& tf = true );
	Qt::ItemFlags	flags( const QModelIndex& index ) const;
	bool	setData( const QModelIndex& index, const QVariant& value,
		 int role = Qt::EditRole );

	//bool	stringSort( const QList<QVariant>& e1, const QList<QVariant>& e2 );
	void	sort( int column, Qt::SortOrder order = Qt::AscendingOrder );
private:
	//TableData		*_data;
	QList<QList<QVariant> >	*_data;
	QStringList		*_header;
	QStringList		*_verticalHeader;
	bool			_editable;
};
}	//	GH namespace
#endif	//	GH_TABLE_MODEL
