#include "ghCliApp.h"

namespace	GH
{
QMap<QString,QString>	MapStringArgs( const int& argc, char** argv )
{
	QMap<QString,QString>	rv;

	for( int ac = 1; ac < argc; ++ac ) {
		QString name = argv[ ac ];
		QString	value = "true";
		if( ac + 1 < argc ) {
			value = argv[ ac + 1 ];
			if( value.startsWith( '-' ) ) {
				value = "true";
			} else {
				++ac;
			}
		}
		if( name.startsWith( '-' ) ) {
			rv.insert( name.mid( 1 ), value );
		}
	}
	return( rv );
}
	CliApp::CliApp( int argc, char** argv )
{
	_isGui = true;
	_arg0 = argv[ 0 ];
	_arg = MapStringArgs( argc, argv );

	addParam( StyleParam, "default", ParamModel::Choice, "Style" );
}
void	CliApp::addParam( const QString& name, ParamModel* param )
{
	_param.insert( name, param );
	_paramNames << name;
}
void	CliApp::addParam( const QString& name, const QVariant& value,
	 const ParamModel::ParamType& type, const QString& displayName )
{
	addParam( name, new ParamModel( name, value, type, displayName ) );
}
QStringList	CliApp::paramNames( const QString& subgroup ) const
{
	if( subgroup.isEmpty() ) {
		return( _paramNames );
	} else {
		QStringList	rv;
		foreach( QString pname, _paramNames ) {
			QStringList tok = pname.split( '/' );
			if( tok.size() > 0 && tok.at( 0 ) == subgroup ) {
				rv << pname;
			}
		}
		return( rv );
	}
}
ParamModel*	CliApp::param( const QString& name ) const
{
	return( _param[ name ] );
}
ParamModel*	CliApp::param( const QString& name )
{
	return( _param[ name ] );
}
ParamModel::ParamType	CliApp::paramType( const QString& name )
{
	return( _param[ name ]->type() );
}
void	CliApp::setParamValue( const QString& name, const QVariant& value )
{
	_param[ name ]->setValue( value );
}
void	CliApp::resetDefaults()
{
	foreach( QString name, paramNames() ) {
		_param[ name ]->resetDefault();
	}
}
	//
	//	OLD INTERFACE
	//
bool	CliApp::isGui() const
{
	return( _isGui );
}
QString CliApp::arg0() const
{
	return( _arg0 );
}
void	CliApp::showError() const
{
	if( isGui() ) {
		emit( emitError( error() ) );
	}
}
QString	CliApp::arg( const QString& name ) const
{
	if( _arg.contains( name ) ) {
		return( _arg[ name ] );
	}
	return( QString() );
}
bool	CliApp::checkArgs()
{
	QStringList	checkList, setList;

	if( _requiredParamNames.isEmpty() ) {
		checkList = paramNames();
	} else {
		checkList = _requiredParamNames;
	}
	foreach( QString name, _arg.keys() ) {
		if( _paramNames.contains( name ) ) {
			setParamValue( name, _arg[ name ] );
			setList << name;
		}
	}
	foreach( QString name, checkList ) {
		if( !setList.contains( name ) ) {
			return( false );
		}
	}
	return( true );
}
void	CliApp::setRequiredParamNames( const QStringList& names )
{
	_requiredParamNames = names;
}
}	//	GH namespace
