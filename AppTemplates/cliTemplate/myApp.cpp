#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	 : CliApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "i", ParamModel::File );

	checkArgs();
}
bool	MyApp::doit()
{
	Ifp	ifp;
	QString	inpath;
	QString	line;

	inpath = S( param( "i" )->value() );

	if( !ifp.open( inpath ) ) {
		cerr << "Usage here" << endl;
		return( false );
	}
	while( nextLine( ifp, line ) ) {
		cout << "DEBUG " << line << endl;
	}
	ifp.close();
	return( true );
}

}	//	GH namespace
