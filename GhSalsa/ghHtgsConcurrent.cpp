#include "ghHtgsConcurrent.h"

namespace	GH
{
/*
 *	Any type of read will be returning a QList<X> and
 *	the input will be a QVariant() which will be
 *	either a StringList or a String for path information
 */
	// _input = LOAD<WigSet>( param( "Input" ), isGui() );
	// _input = LOAD<WigSet>( param( "Input" ). isGui() );
QList<WigSet>	Load_WigSetList( const QVariant& param, const bool& isGui )
{
	QStringList	files = SL( param );
	if( files.size() == 1 && !files.at( 0 ).isEmpty() ) {
		QFuture<WigSetList>	f = 
		 RunFuture(
		 QtConcurrent::run( WigSetList_reader, files.at( 0 ) ), isGui );
		return( f.result() );
/*
		return( RUNLOAD<WigSetList>( files.at( 0 ),
		 WigSetList_reader, isGui ) );
*/
	} else if( files.size() > 1 ) {
		QFuture<WigSet> f =
		 RunFuture(
		 QtConcurrent::mapped( files, WigSet_reader ), isGui );
		return( f.results() );
/*
		return( MAPPEDLOAD<WigSet>( files,
		 WigSet_reader, isGui ) );
*/
	} else {
		return( QList<WigSet>() );
	}
}
QList<BedSet>	Load_BedSetList( const QVariant& param, const bool& isGui )
{
	QStringList	files = SL( param );
	if( files.size() == 1 && !files.at( 0 ).isEmpty() ) {
		QFuture<BedSetList>	f = 
		 RunFuture(
		 QtConcurrent::run( BedSetList_reader, files.at( 0 ) ), isGui );
		return( f.result() );
		
/*
		return( RUNLOAD<BedSetList>( files.at( 0 ),
		 BedSetList_reader, isGui ) );
*/
	} else if( files.size() > 1 ) {
		QFuture<BedSet> f =
		 RunFuture(
		 QtConcurrent::mapped( files, BedSet_reader ), isGui );
		return( f.results() );
/*
		return( MAPPEDLOAD<BedSet>( files,
		 BedSet_reader, isGui ) );
*/
	} else {
		return( QList<BedSet>() );
	}
}
/*
	Try Write_ methods a template
 */
/*
 */
//
//	There is really no other way to do the loading as
//	if there is a single file, we need to separate all the
//	WigSets by sid. The multi-file reader is a different beast.
//
/*********************************************************************
QList<WigSet>	QtC_WigSetList_reader( const QVariant& param,
		 const bool& isGui )
{
	QStringList	files = SL( param );

	if( files.size() == 1 && !files.at( 0 ).isEmpty() ) {
		return( FileLoader<WigSetList>( files[ 0 ],
		 WigSetList_reader, isGui ) );
	} else if( files.size() > 1 ) {
		return( MappedFileLoader<WigSet>( files,
		 WigSet_reader, isGui ) );
	} else {
		return( QList<WigSet>() );
	}
}

BedSetList      QtC_BedSetList_reader( const QStringList& files,
                 const bool& isGui )
{
        if( files.size() == 1 ) {
                return( FileLoader<BedSetList>( files[ 0 ],
                  BedSetList_reader, isGui ) );
                //return( BedSetList_reader( files[ 0 ] ) );
        } else if( files.size() > 1 ) {
                return( MappedFileLoader<BedSet>( files,
                 BedSet_reader, isGui ) );
        } else {
                return( QList<BedSet>() );
        }
}
*************************************************************************/
void	MapSmooth( QList<WigSet>& data, const QMap<QString,QVariant>& params,
	 const bool& isGui )
{
	for( int i = 0; i < data.size(); ++i ) {
		data[ i ].setParams( params );
	}
	RunFuture<void>(
	 QtConcurrent::map( data, &WigSet::smooth ), isGui );
	return;
}
WigSet	RunSmooth( const WigSet& data,
	 const QMap<QString,QVariant>& params,
	 const bool& isGui )
{
	QFuture<WigSet> f = RunFuture<WigSet>(
	 QtConcurrent::run( WigSet_Smooth, data, params ), isGui );
	return( f.result() );

	//return( ProgressRun<WigSet>( data, params, WigSet_Smooth, showDialog ) );
}
WigList	RunBounds( const WigSetList& input, const BedSetList& features,
	 const int& dws, const int& sws, const bool& isGui )
{
	WigList			rv;

	QFuture<WigList> f = RunFuture<WigList>(
	 QtConcurrent::run( Bounds, input, features, dws, sws ),
		 isGui );

	for( int i = 0; i < f.resultCount(); ++i ) {
		rv += f.resultAt( i );
	}
	return( rv );
}
/*
WigList	Bounds_wrapper( const WigSetList& input, const BedSetList& features,
	 const int& dws, const int& sws, const bool& showDialog )
{
	WigList			rv;

	QFuture<WigList>	f;
	QFutureWatcher<WigList>	fw;
	FutureDialog		*dialog = 0;

	if( showDialog ) {
		dialog = new FutureDialog();
		dialog->setLabelText( "Running Bounds" );
		dialog->attach( &fw );
	}
	f = QtConcurrent::run( Bounds, input, features, dws, sws );
	fw.setFuture( f );
	if( showDialog ) {
		dialog->exec();
	}
	fw.waitForFinished();
	for( int i = 0; i < f.resultCount(); ++i ) {
		rv += f.resultAt( i );
	}
	if( dialog ) {
		delete dialog;
	}
	return( rv );
}
*/

}	//	GH namespace
