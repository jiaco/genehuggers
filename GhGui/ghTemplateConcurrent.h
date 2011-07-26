#ifndef	GH_TEMPLATECONCURRENT_H
#define	GH_TEMPLATECONCURRENT_H	1
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



/*******************************************************************
 **	HUGE COMMENT BLOCK CONTAINS DEPRECATED FUNCTIONS
 **
template<class T>
void	vRUN( QFuture<T> future,
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
T	RUN( QFuture<T> future,
		const bool& isGui = true,
		const QString& cancelButtonText = QString() )
{
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
	return( future.result() );
}
template<class T>
void	MAP( QFuture<T> future, const bool& isGui = true )
{
	RUN<T>( future, isGui, "Cancel" );
}
template<class T>
QList<T>	MAPPED( QFuture<T> future, const bool& isGui = true )
{
	vRUN<T>( future, isGui, "Cancel" );
	return( future.results() );
}
template<class T>
T	RUNLOAD( const QString& file, T (*reader)( const QString& ),
	 const bool& isGui )
{
	return( RUN<T>( QtConcurrent::run( reader, file ), isGui ) );
}
template<class T>
QList<T>	MAPPEDLOAD( const QStringList& files,
		 T (*reader)( const QString&), const bool& isGui )
{
	return( MAPPED<T>( QtConcurrent::mapped( files, reader ), isGui ) );
}
*
	was toying with this, leave it here for reference

template<class T,typename Class>
void    RUN2( Class* object, T (Class::*fx)() )
{
        QFuture<T>              f;
        QFutureWatcher<T>       fw;
        FutureDialog            *d = new FutureDialog();
        d->setLabelText( "RUN version 2" );
        d->attach( &fw );
        f = QtConcurrent::run<T>( object, fx );
        fw.setFuture( f );
        d->exec();
        fw.waitForFinished();
        delete d;
}
 *

*
 *	create one blocking and one non-blocking function for this
 *	 as most things (except file writing) can be done in parallel
 *
template<class T>
void    BlockingMapProcess( QList<T>& data, void (*mapFunction)( T& ),
         const bool& showDialog )
{
        QFuture<void>           f;
        QFutureWatcher<void>    fw;
        FutureDialog            *dialog = 0;

        if( showDialog ) {
                dialog = new FutureDialog();
                dialog->setLabelText( "Multi-threaded job in progress" );
                dialog->attach( &fw );
        }
        QtConcurrent::blockingMap( data, mapFunction );
        fw.setFuture( f );
        if( showDialog ) {
                dialog->exec();
        }
        fw.waitForFinished();
        if( dialog ) {
                delete dialog;
        }
        return;
}
*
template<typename Class>
void    Run( Class* object, void (Class::*runFunction)(),
        f = QtConcurrent::run( object, runFunction );
*
template<typename Class>
void    MapMemberProcess( QList<Class>& data, void (Class::*mapFunction)(),
         const bool& showDialog )
{
        QFuture<void>           f;
        QFutureWatcher<void>    fw;
        FutureDialog            *dialog = 0;

        if( showDialog ) {
                dialog = new FutureDialog();
                dialog->setLabelText( "Multi-threaded job in progress" );
                dialog->attach( &fw );
        }
        f = QtConcurrent::map( data, mapFunction );
        //QtConcurrent::blockingMap( data, mapFunction );
        fw.setFuture( f );
        if( showDialog ) {
                dialog->exec();
        }
        fw.waitForFinished();
        if( dialog ) {
                delete dialog;
        }
        return;
}
template<class T>
void    VolatileMapProcess( QList<T>& data, T (*mapFunction)( T& ),
         const bool& showDialog )
{
        QFuture<void>           f;
        QFutureWatcher<void>    fw;
        FutureDialog            *dialog = 0;

        if( showDialog ) {
                dialog = new FutureDialog();
                dialog->setLabelText( "Multi-threaded job in progress" );
                dialog->attach( &fw );
        }
        f = QtConcurrent::map( data, mapFunction );
        //QtConcurrent::blockingMap( data, mapFunction );
        fw.setFuture( f );
        if( showDialog ) {
                dialog->exec();
        }
        fw.waitForFinished();
        if( dialog ) {
                delete dialog;
        }
        return;
}
template<class T>
void    MapProcess( const QList<T>& data, T (*mapFunction)( const T& ),
         const bool& showDialog )
{
        QFuture<void>           f;
        QFutureWatcher<void>    fw;
        FutureDialog            *dialog = 0;

        if( showDialog ) {
                dialog = new FutureDialog();
                dialog->setLabelText( "Multi-threaded job in progress" );
                dialog->attach( &fw );
        }
        f = QtConcurrent::map( data, mapFunction );
        //QtConcurrent::blockingMap( data, mapFunction );
        fw.setFuture( f );
        if( showDialog ) {
                dialog->exec();
        }
        fw.waitForFinished();
        if( dialog ) {
                delete dialog;
        }
        return;
}
template<class T>
void    RunProcess( QList<T>& data, void (*runFunction)( QList<T>& ),
         const bool& showDialog )
{
        QFuture<void>           f;
        QFutureWatcher<void>    fw;
        FutureDialog            *dialog = 0;

        if( showDialog ) {
                dialog = new FutureDialog();
                dialog->setLabelText( "Multi-threaded job in progress" );
                dialog->attach( &fw );
        }
        f = QtConcurrent::run( runFunction, data );
        fw.setFuture( f );
        if( showDialog ) {
                dialog->exec();
        }
        fw.waitForFinished();
        if( dialog ) {
                delete dialog;
        }
        return;
}
template<typename Class>
void    Run( Class* object, void (Class::*runFunction)(),
         const bool& showDialog )
{
        QFuture<void>           f;
        QFutureWatcher<void>    fw;
        FutureDialog            *dialog = 0;

        if( showDialog ) {
                dialog = new FutureDialog();
                dialog->setLabelText( "Multi-threaded job in progress" );
                dialog->attach( &fw );
        }
        f = QtConcurrent::run( object, runFunction );
        fw.setFuture( f );
        if( showDialog ) {
                dialog->exec();
        }
        fw.waitForFinished();
        if( dialog ) {
                delete dialog;
        }
        return;
}


*
template<class T>
void	MapProcess( QList<T>& data, void (*mapFunction)(),
	 const bool& showDialog )
{
	QFuture<void>		f;
	QFutureWatcher<void>	fw;
	FutureDialog		*dialog = 0;

	if( showDialog ) {
		dialog = new FutureDialog();
		dialog->setLabelText( "Multi-threaded job in progress" );
		dialog->attach( &fw );
	}
	f = QtConcurrent::map( data, mapFunction );
	fw.setFuture( f );
	if( showDialog ) {
		dialog->exec();
	}
	fw.waitForFinished();
	if( dialog ) {
		delete dialog;
	}
	return;
}
*
template<class T>
QList<T>	MappedFileLoader( const QStringList& files,
		 T (*reader)(const QString&),
		 const bool& showDialog )
{
	QFuture<T>		f;
	QFutureWatcher<T>	fw;
	FutureDialog		*dialog = 0;

	if( showDialog ) {
		dialog = new FutureDialog();
		dialog->setLabelText( "Multi-threaded job in progress" );
		dialog->attach( &fw );
	}
	f = QtConcurrent::mapped( files, reader );
	fw.setFuture( f );
	if( showDialog ) {
		dialog->exec();
	}
	fw.waitForFinished();
	if( dialog ) {
		delete dialog;
	}
	return( f.results() );
}

template<class T>
T	FileLoader( const QString& file,
		 T (*reader)(const QString&),
		 const bool& showDialog )
{
	QFuture<T>		f;
	QFutureWatcher<T>	fw;
	FutureDialog		*dialog = 0;

	if( showDialog ) {
		dialog = new FutureDialog();
		dialog->setLabelText( "Single-threaded job in progress" );
		dialog->attach( &fw );
	}
	f = QtConcurrent::run( reader, file );
	fw.setFuture( f );
	if( showDialog ) {
		dialog->exec();
	}
	fw.waitForFinished();
	if( dialog ) {
		delete dialog;
	}
	return( f.result() );
}

template<class T>
void	FileWriter( const QString& file, const T& data,
	 bool (*writer)( const QString&, const T& ),
	 const bool& showDialog )
{
	QFuture<void>	f;
	QFutureWatcher<void>	fw;
	FutureDialog	*dialog = 0;

	if( showDialog ) {
		dialog = new FutureDialog();
		dialog->setLabelText( "single-threaded job in progress" );
		dialog->attach( &fw );
	}
	f = QtConcurrent::run( writer, file, data );
	fw.setFuture( f );
	if( showDialog ) {
		dialog->exec();
	}
	fw.waitForFinished();
	if( dialog ) {
		delete dialog;
	}
	return;
}

template<class T>
void	FileWriterMulti(
	 const QString& path,
	 const QString& label,
	 const T& data,
	 bool (*writer)( const QString&, const QString&, const T& ),
	 const bool& showDialog )
{
	QFuture<void>	f;
	QFutureWatcher<void>	fw;
	FutureDialog	*dialog = 0;

	if( showDialog ) {
		dialog = new FutureDialog();
		dialog->setLabelText( "single-threaded job in progress" );
		dialog->attach( &fw );
	}
	f = QtConcurrent::run( writer, path, label, data );
	fw.setFuture( f );
	if( showDialog ) {
		dialog->exec();
	}
	fw.waitForFinished();
	if( dialog ) {
		delete dialog;
	}
	return;
}

template<class T>
T	ProgressRun( const T& data, const QMap<QString,QVariant>& params,
	 T (*runFunction)( const T&,  const QMap<QString,QVariant>& ),
	 const bool& showDialog )
{
	QFuture<T>	f;
	QFutureWatcher<T>	fw;
	FutureDialog	*dialog = 0;

	if( showDialog ) {
		dialog = new FutureDialog();
		dialog->setLabelText( "Single-threaded run" );
		dialog->attach( &fw );
	}
	f = QtConcurrent::run( runFunction, data, params );
	fw.setFuture( f );
	if( showDialog ) {
		dialog->exec();
	}
	fw.waitForFinished();
	if( dialog ) {
		delete dialog;
	}
	return( f.result() );
}

*********************************************************************/
