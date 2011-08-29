#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: CliApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "action/action1", "", ParamModel::Action, "Action1",
	"tooltip=Here is a tooltip for ya;"
	"whatsthis=Anybody able to read this?" );
	addParam( "action/action2", "", ParamModel::Action, "Action2" );
	addParam( "boolean/bool1", "true", ParamModel::Boolean, "Boolean1" );
	addParam( "boolean/bool2", "false", ParamModel::Boolean, "Boolean2" );
	addParam( "edit/edit1", "", ParamModel::Edit, "Edit1" );
	addParam( "file/file1", "", ParamModel::File, "File1" );
	addParam( "file/file2", "", ParamModel::File, "File2",
	 "checkable=true;"
	 "tooltip=get a real job;"
	 "dialogtype=save;"
	 "whatsthis=<font color=#00ff00>wow green</font>in a sea of color"
	 );
	addParam( "file/file3", "", ParamModel::File, "File3",
	 "caption=hehehee;"
	 "filter=*.png"
	);
	addParam( "font/font1", "", ParamModel::Font, "Font1" );
	addParam( "color/color1", "", ParamModel::Color, "Color1" );

}
void	MyApp::doit()
{
	cerr << "doit!" << endl;
}
}	//	GH namespace
