#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );
}
void	MyApp::doit()
{
	cerr << "doit!" << endl;
}
}	//	GH namespace
