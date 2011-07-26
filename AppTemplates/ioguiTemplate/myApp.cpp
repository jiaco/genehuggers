#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Input", "", Param::FileParam, "Input File" );
	addParam( "Param1", "Default",
	 Param::EditParam, "Enter Parameter please" );
	addParam( "Output", "", Param::FileParam, "Output File" );

	_isGui = true;
//addParam( "Param1", "Default", Param::EditParam, "Enter Parameter please" );
}
void	MyApp::pasteInput( const QString& s )
{
	cout << "got " << s << endl;
}
void	MyApp::doit()
{
	cerr << "doit!" << endl;
}
}	//	GH namespace
