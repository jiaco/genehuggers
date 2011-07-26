#ifndef	GH_HTGSCONCURRENT_H
#define	GH_HTGSCONCURRENT_H	1
#include <GhCore.h>
#include <GhBio.h>
#include <GhGui.h>

namespace	GH
{
GH_DLL_EXPORT QList<WigSet>	Load_WigSetList(
				 const QVariant& param, const bool& isGui );
GH_DLL_EXPORT QList<BedSet>	Load_BedSetList(
				 const QVariant& param, const bool& isGui );
template<class T>
void	Write_BaseSet( QList<T>& data,
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
			//i == 0 ? prun[ "WriteMode" ] = "" : prun[ "WriteMode" ] = "append";
			prun["WriteMode"] = i == 0 ? "" : "append";
			data[ i ].injectParams( prun );
		}
		RunFuture<void>( QtConcurrent::run( WriteDacList<T>, data ) );
	}
}
/*
GH_DLL_EXPORT	void		Write_WigSetList(
				 const WigSetList& data
				);


GH_DLL_EXPORT QList<WigSet>	QtC_WigSetList_reader(
				 const QStringList& files,
				 const bool& isGui );
GH_DLL_EXPORT QList<BedSet>	QtC_BedSetList_reader( const QStringList& files,
				 const bool& isGui = false );


GH_DLL_EXPORT	WigList	RunWigListSmooth( const WigList& data,
			 const int& dataSize, const int& windowSize,
			 const int& minPosition, const int& incPosition );
*/

/*
 * next two functions are never used, implemented at a time when
  i should have been doing the above Smooth (for Plotter), need
 to figure out where they are (wizard?) and replace them
 */
GH_DLL_EXPORT void	MapSmooth( QList<WigSet>& data,
	 		 const QMap<QString,QVariant>& params,
	 		 const bool& isGui );

GH_DLL_EXPORT WigSet	RunSmooth( const WigSet& data,
	 		 const QMap<QString,QVariant>& params,
	 		 const bool& isGui );

GH_DLL_EXPORT WigList	RunBounds( const WigSetList& input,
			 const BedSetList& features,
	 		 const int& dws, const int& sws,
			 const bool& showDialog = true );

/*
GH_DLL_EXPORT WigList	Bounds_wrapper( const WigSetList& input,
			 const BedSetList& features,
	 		 const int& dws, const int& sws,
			 const bool& showDialog = true );
*/

}	//	GH namespace
#endif	//	GH_HTGSCONCURRENT_H
