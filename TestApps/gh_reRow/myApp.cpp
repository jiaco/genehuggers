#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Input", "", Param::FileParam, "Input File" );
// should use a ListParam for this but it is broken
//	QStringList selectFields;
// put this in doit() for now
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
	QStringList	selectFields;
	QStringList	outHdr;

	selectFields << "Gene Symbol"
		 << "Context Sequence";

	outHdr << "mir" << "seq";

	
	Ifp	ifp;
	Row	row;

	if( !ifp.open( ipath ) ) {
		cerr << "error reading " << ipath << endl;
		return;
	}

	//QStringList	hdr = ifp.next().split( "\t" );

	row.attachHeader( ifp.next().split( "\t" ) );
	cout << outHdr.join( "\t" ) << endl;

	while( row.next( ifp ) ) {
		QStringList	outRow;
		foreach( QString s, selectFields ) {
			outRow << S( row[ s ] );
		}
		cout << outRow.join( "\t" ) << endl;
	}
	

	ifp.close();
	return;
}
}	//	GH namespace
