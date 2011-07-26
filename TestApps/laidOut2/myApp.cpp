#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam2( Param2( "QtOnly", "", Param2::ActionParam ) );
	addParam2( Param2( "ActionParams", "", Param2::ActionParam ) );
	addParam2( Param2( "ActionParam2s", "", Param2::ActionParam ) );

	//_isGui = !checkArgs();
	_isGui = true;

	makeBtnParams();
}
bool	MyApp::addParam2( Param2 param )
{
	if( _param2.contains( param.name() ) ) {
		return( false );
	}
	_param2.insert( param.name(), param );
	_param2Names << param.name();
	return( true );
}
Param2*	MyApp::pparam2( const QString& name )
{
	if( _param2.contains( name ) ) {
		return( &_param2[ name ] );
	}
	return( 0 );
}
QStringList	MyApp::param2Names() const
{
	return( _param2Names );
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
