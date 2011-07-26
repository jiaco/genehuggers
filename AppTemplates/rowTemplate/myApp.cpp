#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Input", "", Param::FileParam, "Input File" );
/*
	addParam( "Param1", "Default",
	 Param::EditParam, "Enter Parameter please" );
	addParam( "Output", "", Param::FileParam, "Output File" );
*/

	_isGui = !checkArgs();
}
void	MyApp::doit()
{
	QString ipath = S( paramValue( "Input" ) );
	Ifp	ifp;
	Row	row;

	if( !ifp.open( ipath ) ) {
		cerr << "error reading " << ipath << endl;
		return;
	}

	//QStringList	hdr = ifp.next().split( "\t" );
	row.attachHeader( ifp.next().split( "\t" ) );
	while( row.next( ifp ) ) {
		for( int i = 0; i < row.size(); ++i ) {
			cout << row.hdrAt( i ) << "\t" 
			 << S( row[ row.hdrAt( i ) ] ) << endl;
			 //<< S( row.value( row.hdrAt( i ) ) ) << endl;
		}
		//cout << "\t" << row.join( " " ) << endl;
	}
	

	ifp.close();
	return;
}
}	//	GH namespace
