#ifndef	GH_LISTMODEL_H
#define	GH_LISTMODEL_H	1
#include <QtGui>
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT	ListModel	:	public	QStringListModel
{
	Q_OBJECT

public:
	ListModel( QObject* parent = 0 );

	QModelIndex	createIndex( int row, int column, void *ptr = 0 ) const;

/*
	QVariant	data( const QModelIndex& index,
			 int role = Qt::DisplayRole ) const;
	int		rowCount(
			 const QModelIndex& parent = QModelIndex() ) const;

	void		setData( QStringList* data );


private:
	QStringList	*_data;
*/
};
}	//	GH namespace
#endif	//	GH_LISTMODEL_H
