#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: CliApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

/*
	addParam( "Input", "", ParamModel::File );
	addParam( "Output", "", ParamModel::File );
	addParam( "Threshold", "2.0", ParamModel::Edit );
	addParam( "Rev", "false", ParamModel::Boolean );
*/
	addParam( "Doit", "", ParamModel::Action );

}
void	MyApp::doit()
{
	cerr << "doit!" << endl;
}
}	//	GH namespace
