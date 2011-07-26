#ifndef	GH_SALSADAC_H
#define	GH_SALSADAC_H	1
#include "GhCore.h"
#include "GhBio.h"
#include "GhGui.h"
namespace	GH
{
/*
 *	Any type of read will be returning a QList<X> and
 *	the input will be a QVariant() which will be
 *	either a StringList or a String for path information
 */
	// _input = LOAD<WigSet>( param( "Input" ), isGui() );
	// _input = LOAD<WigSet>( param( "Input" ). isGui() );
inline QList<WigSet>	Load_WigSetList( const QVariant& param, const bool& isGui )
{
	QStringList	files = SL( param );
	if( files.size() == 1 && !files.at( 0 ).isEmpty() ) {
		QFuture<WigSetList>	f = 
		 RunFuture(
		 QtConcurrent::run( WigSetList_reader, files.at( 0 ) ), isGui );
		return( f.result() );
	} else if( files.size() > 1 ) {
		QFuture<WigSet> f =
		 RunFuture(
		 QtConcurrent::mapped( files, WigSet_reader ), isGui );
		return( f.results() );
	} else {
		return( QList<WigSet>() );
	}
}
inline QList<BedSet>	Load_BedSetList( const QVariant& param, const bool& isGui )
{
	QStringList	files = SL( param );
	if( files.size() == 1 && !files.at( 0 ).isEmpty() ) {
		QFuture<BedSetList>	f = 
		 RunFuture(
		 QtConcurrent::run( BedSetList_reader, files.at( 0 ) ), isGui );
		return( f.result() );
	} else if( files.size() > 1 ) {
		QFuture<BedSet> f =
		 RunFuture(
		 QtConcurrent::mapped( files, BedSet_reader ), isGui );
		return( f.results() );
	} else {
		return( QList<BedSet>() );
	}
}

}	//	GH namespace
#endif	//	GH_SALSADAC_H
