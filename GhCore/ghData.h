#ifndef	GH_DATA_H
#define	GH_DATA_H	1
#include "ghRow.h"
#include "ghToken.h"

namespace	GH
{
GH_DLL_EXPORT	QList<QVariant>	CollectColumn( const QString& colName,
				 const QList<Row>& data );
GH_DLL_EXPORT	bool	ReadData( const QString& path,
			 QStringList& hdr, QList<Row>& data );
GH_DLL_EXPORT	bool	LoadData( Ifp& fp,
			 QStringList& hdr, QList<Row>& data );
GH_DLL_EXPORT	bool	WriteData( const QString& path,
			 const QStringList& hdr, const QList<Row>& data );
GH_DLL_EXPORT	bool	ShowData( Ofp& fp,
			 const QStringList& hdr, const QList<Row>& data );

GH_DLL_EXPORT void	TransposeData( QStringList& hdr, QList<Row>& data );

}	//	GH namespace
#endif	//	GH_DATA_H
