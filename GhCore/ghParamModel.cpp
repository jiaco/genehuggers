#include "ghParamModel.h"

namespace	GH
{
ParamModel::ParamType	ParamModel::StringToParamType( const QString& text )
{
	QString s = text.toLower();
	QString	s2;
	ParamType rv = ParamModel::Undef;

	if( s.contains( '|' ) ) {
		int x = s.indexOf( '|' );
		s = s.mid( 0, x );
		s2 = s.mid( x + 1 );
	}
	if( s == "edit" ) {
		rv = ParamModel::Edit;
	} else if( s == "file" ) {
		rv = ParamModel::File;
	} else if( s == "action" ) {
		rv = ParamModel::Action;
	} else if( s == "boolean" || s == "bool" ) {
		rv = ParamModel::Boolean;
	} else if( s == "choice" ) {
		rv = ParamModel::Choice;
	} else if( s == "color" ) {
		rv = ParamModel::Color;
	} else if( s == "font" ) {
		rv = ParamModel::Font;
	} else if( s == "toolbutton" ) {
		rv = ParamModel::ToolButton;
	}
	if( rv == ParamModel::Boolean && s2 == "radio" ) {
		rv |= ParamModel::Radio;
	} else if( rv == ParamModel::Choice ) {
		if( s2 == "selector" ) {
			rv |= ParamModel::Selector;
		} else if( s2 == "checkgroup" ) {
			rv |= ParamModel::CheckGroup;
		} else if( s2 == "radiogroup" ) {
			rv |= ParamModel::RadioGroup;
		}
	}
	return( rv );
}
	ParamModel::ParamModel( QObject *parent )
	: QObject( parent )
{
	init();
	configure();
}
	ParamModel::ParamModel( const QString& name,
	 const QString& configText, QObject* parent )
	: QObject( parent )
{
	init();
	_name = name;
	addConfig( configText );
	configure();
}
	ParamModel::ParamModel( const QString& name,
	 const ParamModel::ParamType& type,
	 const QString& configText, QObject* parent )
	: QObject( parent )
{
	init();
	_name = name;
	addConfig( configText );
	_type = type;
	configure();
}
	ParamModel::ParamModel( const QString& name, const QVariant& value,
	 const ParamType& type, const QString& displayName, QObject *parent )
	: QObject( parent )
{
	init();
	_name = name;
	_defaultValue = value;
	_value = value;
	_type = type;
	_displayName = displayName;
	configure();
}
	ParamModel::ParamModel( const QString& name, const QVariant& value,
	 const ParamType& type, const QString& displayName,
	 const QString& configText, QObject *parent )
	: QObject( parent )
{
	init();
	_name = name;
	_defaultValue = value;
	_value = value;
	_type = type;
	_displayName = displayName;

	addConfig( configText );
	configure();
}
void	ParamModel::init()
{
	_name = _displayName = "";
	_type = ParamModel::Undef;
	_value = _defaultValue = QVariant();
	
	_hasSetting = true;
	_enabled = true;
	_checkable = _checked = false;

	_config.clear();
}
void	ParamModel::configure()
{	
	// name is the only thing not possibly in the config
	if( hasConfig( "type" ) ) {
		_type = StringToParamType( configString( "type" ) );
	}
	if( hasConfig( "value" ) ) {
		_value = config( "value" );
	}
	if( hasConfig( "defaultValue" ) ) {
		_defaultValue = config( "defaultValue" );
		_value = _defaultValue;
	}
	if( hasConfig( "displayName" ) ) {
		_displayName = configString( "displayName" );
	}
	if( hasConfig( "hasSetting" ) &&
	 B( config( "hasSetting" ) ) == false ) {
		_hasSetting = false;
	}
	if( hasConfig( "enabled" ) &&
	 B( config( "enabled" ) ) == false ) {
		_enabled = false;
	}
	if( hasConfig( "checkable" ) &&
	 B( config( "checkable" ) ) == true ) {
		_checkable = true;
	}
	if( hasConfig( "checked" ) &&
	 B( config( "checked" ) ) == true ) {
		_checked = true;
	}
	//
	// TODO what is more logical? USTR = name and "" = empty right?
	//
	if( _displayName.isEmpty() ) {
		_displayName = "";
	} else if( _displayName == USTR ) {
		_displayName = _name;
	}
	setObjectName( _name );

	if( _type == Action ) {
		_hasSetting = false;
	}
}
void	ParamModel::addConfig( const QString& text )
{
	QMap<QString,QVariant>	jason = JASON( text );
	foreach( QString key, jason.keys() ) {
		if( _config.contains( key ) ) {
			_config[ key ] = jason[ key ];
		} else {
			_config.insert( key, jason[ key ] );
		}
	}
}
bool	ParamModel::hasConfig( const QString& key ) const
{
	return( _config.contains( key ) );
}
QVariant	ParamModel::config( const QString& key ) const
{
	QVariant	rv;

	if( hasConfig( key ) ) {
		rv = _config[ key ];
	}
	return( rv );
}
QString	ParamModel::configString( const QString& key ) const
{
	return( S( config( key ) ) );
}
void	ParamModel::setToolTip( const QString& text )
{
	_config[ "toolTip" ] = text;
}
void	ParamModel::setWhatsThis( const QString& text )
{
	_config[ "whatsThis" ] = text;
}
QString	ParamModel::toolTip() const
{
	QString	rv;

	if( _config.contains( "toolTip" ) ) {
		rv = S( _config[ "toolTip" ] );
	} else {
		rv = _displayName;
	}
	return( rv );
}
QString	ParamModel::whatsThis() const
{
	QString	rv;

	if( _config.contains( "whatsThis" ) ) {
		rv = S( _config[ "whatsThis" ] );
	} else {
		rv = toolTip();
	}
	// TODO attempt to fix this via StyleSheets or something
	rv = QString( "<font color=\"#0f0f0f\">%1" ).arg( rv );
	return( rv );
}
void	ParamModel::resetDefault()
{
	setValue( _defaultValue );
}
void	ParamModel::setValue( const QVariant& value )
{
	_value = value;
	emit( changed( _value ) );
}
void	ParamModel::guiSetValue( const QVariant& value )
{
	_value = value;
}
bool	ParamModel::hasSetting() const
{
	return( _hasSetting );
}

bool	ParamModel::isEnabled() const
{
	return( _enabled );
}
void	ParamModel::setEnabled( const bool& tf )
{
	_enabled = tf;
	emit( enabled( _enabled ) );

	//emit( changed() );
}
void	ParamModel::guiSetEnabled( const bool& tf )
{
	_enabled = tf;
}
bool	ParamModel::isCheckable() const
{
	return( _checkable );
}
void	ParamModel::setCheckable( const bool& tf )
{
	_checkable = tf;
	//emit( changed() );
}
void	ParamModel::guiSetCheckable( const bool& tf )
{
	_checkable = tf;
}
bool	ParamModel::isChecked() const
{
	return( _checked );
}
void	ParamModel::setChecked( const bool& tf )
{
	_checked = tf;
	emit( checked( _checked ) );
	//emit( toggled( _checked ) );
}
void	ParamModel::guiSetChecked( const bool& tf )
{
	_checked = tf;
}

QString	ParamModel::name() const
{
	return( _name );
}
QString	ParamModel::displayName() const
{
	return( _displayName );
}
QVariant	ParamModel::value() const
{
	return( _value );
}
ParamModel::ParamType	ParamModel::type() const
{
	return( _type );
}
ParamModel::ParamType	ParamModel::baseType() const
{
	return( _type & ParamModel::BaseTypeAndValue );
}
}
