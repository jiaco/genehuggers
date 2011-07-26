#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: CliApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Edit", "Default", ParamModel::Edit, "Enter Parameter please" );
	addParam( "Check", "Default", ParamModel::Boolean, "Enter Parameter please" );
	addParam( "Combo", "Default", ParamModel::Choice, "Enter Parameter please" );
	addParam( "File", "Default", ParamModel::File, "Enter File1 please" );
	addParam( "FileCheck", "Default", ParamModel::File, "Enter File2 please" );
	addParam( "Group", "Default", ParamModel::Color, "Enter Parameter please" );
	addParam( "Button", "Default", ParamModel::Action, "Enter Parameter please" );
	_isGui = true;
}
void	MyApp::doit()
{
	cerr << "doit!" << endl;
}
}	//	GH namespace
