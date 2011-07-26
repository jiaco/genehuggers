#ifndef	GH_GUIDACUTILS_H
#define	GH_GUIDACUTILS_H	1
#include "GhCore.h"
#include "ghGuiDac.h"

namespace	GH
{
template<class T>
void	WriteDacList( QList<T>& data,
		const QString& path, const QString& format,
		const bool& multi = false,
		const QString& baseLabel = QString() )
{
	QMap<QString,QVariant>	prun;

	prun.insert( "WriteFormat", format );
	prun.insert( "WriteMode", "" );
	prun.insert( "WritePath", "" );

	if( multi ) {
		for( int i = 0; i < data.size(); ++i ) {
			QString _path = QString( "%1/%2%3.%4" )
			 .arg( path ).arg( baseLabel )
			 .arg( data[ i ].sid ).arg( format );
			prun[ "WritePath" ] = V( _path );
			data[ i ].injectParams( prun );
		}
		RunFuture<void>( QtConcurrent::map( data, &T::write ) );
	} else {
		prun.insert( "WritePath", path );
		for( int i = 0; i < data.size(); ++i ) {
			prun["WriteMode"] = i == 0 ? "" : "append";
			data[ i ].injectParams( prun );
		}
		RunFuture<void>( QtConcurrent::run( WriteDacList<T>, data ) );
	}
}

}	//	GH namespace
#endif	//	GH_GUIDACUTILS_H
