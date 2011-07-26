#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "QtOnly", "", Param::ActionParam );
	addParam( "ActionParams", "", Param::ActionParam );

	_isGui = !checkArgs();

//	makeBtnParams();
}
void	MyApp::makeBtnParams()
{
	for( int r = 0; r < 5; ++r ) {
		for( int c = 0; c < 5; ++c ) {
			QString s = QString( "Btn%1-%2" )
			 .arg( r + 1 ).arg( c + 1 );
			addParam( s, "", Param::ActionParam );
		}
	}
}
void	MyApp::doit()
{
	cerr << "doit!" << endl;
}
}	//	GH namespace
