#include "ghCliApp.h"

namespace	GH
{
CliApp  *CliApp::self = 0;

QVariant	APP_V( const QString& name )
{
/*
	Q_ASSERT_X( ghApp->hasParam( name ), "CliApp",
	 QString::fromLatin1( "Attempt to access undefined parameter '%1'" )
	 .arg( name ).toLocal8Bit().data() );
*/
	if( !ghApp->hasParam( name ) ) {
		qDebug() << ghApp->arg0() << " does not have parameter " << name;
		qFatal( "---die a violent death" );
	}
	return( ghApp->param( name )->value() );
}
bool	APP_B( const QString& name )
{
	return( B( APP_V( name ) ) );
}
int	APP_I( const QString& name )
{
	return( I( APP_V( name ) ) );
}
double	APP_D( const QString& name )
{
	return( D( APP_V( name ) ) );
}
QString	APP_S( const QString& name )
{
	return( S( APP_V( name ) ) );
}
QStringList	APP_SL( const QString& name )
{
	return( SL( APP_V( name ) ) );
}
QColor		APP_CLR( const QString& name )
{
	return( CLR( APP_V( name ) ) );
}
QFont		APP_FONT( const QString& name )
{
	return( FONT( APP_V( name ) ) );
}
QString		APP_FONTSTRING( const QString& name )
{
	return( FONTSTRING( APP_FONT( name ) ) );
}
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
	Q_ASSERT_X( !self, "CliApp", "there should be only one CliApp object" );
	CliApp::self = this;
	_isGui = true;
	_arg0 = argv[ 0 ];
	_arg = MapStringArgs( argc, argv );

	addParam( StyleParam, "default", ParamModel::Choice, "Style" );
}
	CliApp::~CliApp()
{
	CliApp::self = 0;
}
void	CliApp::addParam( const QString& name, ParamModel* param )
{
	if( hasParam( name ) ) {
		// TODO really need to make Errorprone more usable
		return;
	}
	_param.insert( name, param );
	_paramNames << name;
}
void	CliApp::addParam( const QString& name, const QVariant& value,
	 const ParamModel::ParamType& type, const QString& displayName,
	 const QString& configText )
{
	addParam( name,
	 new ParamModel( name, value, type, displayName, configText, this ) );
}
void	CliApp::addParam( const QString& name,
	 const ParamModel::ParamType& type, const QString& configText )
{
	addParam( name, new ParamModel( name, type, configText, this ) );
}
void	CliApp::addParam( const QString& name, const QString& configText )
{
	addParam( name, new ParamModel( name, configText, this ) );
}

QStringList	CliApp::paramNames( const QString& subgroup ) const
{
	if( subgroup.isEmpty() ) {
		return( _paramNames );
	} else {
		QStringList	rv;
		foreach( QString pname, _paramNames ) {
			QStringList tok = pname.split( '/' );
			if( tok.size() > 1 && tok.at( 0 ) == subgroup ) {
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
	if( !_param.contains( name ) ) {
		qWarning() << "INVALID PARAM NAME " << name;
		return;
	}
	_param[ name ]->setValue( value );
}
bool	CliApp::hasParam( const QString& name ) const
{
	return( _param.contains( name ) );
}
void	CliApp::resetDefaults()
{
	foreach( QString name, paramNames() ) {
		_param[ name ]->resetDefault();
	}
}
void	CliApp::passInfo( const QString& text ) const
{
	emitError( Info( text ) );
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
