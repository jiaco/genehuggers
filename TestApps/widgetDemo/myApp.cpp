#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: CliApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "action/action1", "", ParamModel::Action, "Action1" );
	addParam( "action/action2", "", ParamModel::Action, "Action2" );
	addParam( "boolean/bool1", "true", ParamModel::Boolean, "Boolean1" );
	addParam( "boolean/bool2", "false", ParamModel::Boolean, "Boolean2" );
	addParam( "edit/edit1", "", ParamModel::Edit, "Edit1" );
	addParam( "file/file1", "", ParamModel::File, "File1" );
	addParam( "font/font1", "", ParamModel::Font, "Font1" );
	addParam( "color/color1", "", ParamModel::Color, "Color1" );
}
void	MyApp::doit()
{
	cerr << "doit!" << endl;
}
}	//	GH namespace
