#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Query", "", Param::FileParam, "Input File" );
	addParam( "Input", "", Param::FileParam, "Input File" );

	_isGui = !checkArgs();
}
void	MyApp::doit()
{
	QString qpath = S( paramValue( "Query" ) );
	QString ipath = S( paramValue( "Input" ) );

	QString	qId = "mir";
	
	QStringList	qids, fids;

	Ifp	ifp;
	Row	row;

	if( !ifp.open( qpath ) ) {
		cerr << "error reading " << qpath << endl;
		return;
	}
	row.attachHeader( ifp.next().split( "\t" ) );
	while( row.next( ifp ) ) {
		qids << S( row[ qId ] );
	}
	ifp.close();

	foreach( QString s, qids ) {
		cerr << "DEBUG looking for: " << s << endl;
	}

	if( !ifp.open( ipath ) ) {
		cerr << "error reading " << ipath << endl;
		return;
	}
	row.attachHeader( ifp.next().split( "\t" ) );
	while( row.next( ifp ) ) {
		if( qids.contains( S( row[ qId ] ) ) ) {
			cout << row.line() << endl;
			cerr << "\tDEBUG found " << S( row[ qId ] ) << endl;
		}
	}
	

	ifp.close();
	return;
}
}	//	GH namespace
