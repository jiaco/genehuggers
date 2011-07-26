#ifndef	GH_TABLE_DATA_H
#define	GH_TABLE_DATA_H	1
#include <QtCore>
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT	TableData	:	public	QList<QList<QVariant> >
{
public:
	void	setValue( const int& row, const int& col,
		 const QVariant& value );
};
}	//	GH namespace
#endif
