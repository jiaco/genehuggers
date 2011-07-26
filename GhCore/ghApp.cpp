#include "ghApp.h"

namespace	GH
{
	App::App( int argc, char** argv )
{
	_isGui = false;
	_arg0 = QString();
	if( argc > 0 ) {
		_arg0 = argv[ 0 ];
		_args = Args( argc, argv );
	}
	USAGE = _arg0 + ":";
}
void	App::usage()
{
	Ofp	err;
	err.open( STDERR );
	err << USAGE;
	err.close();
}
QString	App::arg0() const
{
	return( _arg0 );
}
bool	App::isGui() const
{
	return( _isGui );
}
void	App::showError() const
{
	Ofp	cerr;
	if( cerr.open( STDERR ) ) {
		cerr << errorMessage() << endl;
	}
	cerr.close();
}
const	QMap<QString,QString>	App::args() const
{
	return( _args );
}
//////////////////////////////////////////////////////
//	The rest of the class uses ghParam.h
//////////////////////////////////////////////////////
bool	App::hasParam( const QString& name ) const
{
	return( _param.contains( name ) );
}
bool	App::addParam( Param param )
{
	if( _param.contains( param.name() ) ) {
		// qWarning or subclass Errorprone
		return( false );
	}
	_param.insert( param.name(), param );
	_paramNames << param.name();
	return( true );
}
bool	App::addParam( const QString& name,
		const QString& defaultValue,
		const Param::ParamType& type,
		const QString& displayName )
{
	return( addParam( Param( name, defaultValue, type, displayName ) ) );
}
Param	App::param( const QString& name )
{
	if( _param.contains( name ) ) {
		return( _param[ name ] );
	}
	return( Param() );
}
Param*	App::pparam( const QString& name )
{
	if( _param.contains( name ) ) {
		return( &_param[ name ] );
	}
	return( 0 );
}
QVariant	App::paramValue( const QString& name ) const
{
	if( _param.contains( name ) ) {
		return( _param[ name ] );
	}
	return( QVariant() );
}
QStringList	App::paramNames() const
{
	return( _paramNames );
}
void	App::setParam( const QString& name, const QVariant& value,
	 const bool& silent )
{
	// silent only get implemented in ObjApp
	// where it will emit( paramChanged() )
	Q_UNUSED( silent );

	if( !_param.contains( name ) ) {
		return;
	}
	switch( value.type() ) {
		case    QVariant::Bool:
			_param[ name ].setValue( value.toBool() );
			break;
		case    QVariant::Int:
			_param[ name ].setValue( value.toInt() );
			break;
		case    QVariant::Double:
			_param[ name ].setValue( value.toDouble() );
			break;
		case    QVariant::String:
			_param[ name ].setValue( value.toString() );
			break;
		case    QVariant::StringList:
			_param[ name ].setValue( value.toStringList() );
			break;
		case    QVariant::Rect:
			_param[ name ].setValue( value.toRect() );
			break;
		case	QVariant::List:
			_param[ name ].setValue( value.toList() );
			break;
		case	QVariant::Map:
			_param[ name ].setValue( value.toMap() );
			break;
		default:
			break;
	}
}
void	App::guiSetParam( const QString& name, const QVariant& value )
{
	return( setParam( name, value, false ) );
}
//
//	checkArgs determine whether or not app is GUI
//	 by trying to find a CLI arg for each param
//	call it with App::_isGui = App::checkArgs()
//
bool	App::checkArgs()
{
	QStringList	usedArgs;

	foreach( QString pname, _paramNames ) {
		QString lpname = pname.toLower();
		foreach( QString aname, _args.keys() ) {
			QString laname = aname.toLower();
			if( usedArgs.contains( laname ) ) {
				continue;
			}
			if( !lpname.startsWith( laname ) ) {
				continue;
			}
			_param[ pname ].setValue( V( _args[ aname ] ) ); 
			usedArgs << laname;
			break;
		}
	}
	if( _param.size() == usedArgs.size() ) {
		return( true );
	}
	return( false );
}


}	//	GH namespace
