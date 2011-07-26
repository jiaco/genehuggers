#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Edit", "Default", Param::EditParam, "Enter Parameter please" );
	addParam( "Check", "Default", Param::CheckParam, "Enter Parameter please" );
	addParam( "Combo", "Default", Param::ComboParam, "Enter Parameter please" );
	addParam( "File", "Default", Param::FileParam, "Enter Parameter please" );
	addParam( "Group", "Default", Param::GroupBoxParam, "Enter Parameter please" );
	addParam( "Radio", "Default", Param::RadioParam, "Enter Parameter please" );
	addParam( "Button", "Default", Param::ButtonParam, "Enter Parameter please" );
	_isGui = true;
}
void	MyApp::doit()
{
	cerr << "doit!" << endl;
}
}	//	GH namespace
