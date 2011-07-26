#ifndef	GH_GUIDAC_H
#define	GH_GUIDAC_H	1
#include <GhCore.h>
#include "ghFutureDialog.h"

namespace	GH
{
//
//	Try to use a single wrapper voidRunFuture for the FutureDialog
//	 then a bunch of wrappers that handle the different uses
// 	 of the result in QFuture
//
//
//	ex:
//	 RUN<void>( QtConcurrent::run( this, &MyApp::loadFile ) );
//	 MAP<void>( QtConcurrent::map( list, func ) );
//		where list is QList<X> and
//		X func( X& x ); modifies in place
//	 MAPPED<int>( QtConcurrent::mapped( stringlist, &QString::size ) );
//		where stringlist is a QStringList
//
//
template<class T>
void	voidRunFuture( QFuture<T> future,
		 const bool& isGui = true,
		 const QString& cancelButtonText = QString() )
{
	QFutureWatcher<T>	fw;
	FutureDialog		*d = 0;

	if( isGui ) {
		//
		// no Cancel button (second param) as Run is not able
		// to be canceled
		//
		d = new FutureDialog( "Running...", cancelButtonText );
		d->attach( &fw );
	}
	fw.setFuture( future );
	if( isGui ) {
		d->exec();
	}
	fw.waitForFinished();
	if( isGui ) {
		delete d;
	}
}
template<class T>
QFuture<T>	RunFuture( QFuture<T> future,
		 const bool& isGui = true,
		 const QString& cancelButtonText = QString() )
{
	voidRunFuture( future, isGui, cancelButtonText );
	return( future );
}
}	//	GH namespace
#endif	//	GH_TEMPLATECONCURRENT_H
