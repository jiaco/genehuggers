#include "ghParamModel.h"

namespace	GH
{
	ParamModel::ParamModel( QObject *parent )
	: QObject( parent )
{
}
	ParamModel::ParamModel( const QString& name, const QVariant& value,
	 const ParamType& type, const QString& displayName, QObject *parent )
	: QObject( parent )
{
	_name = name;
	_defaultValue = value;
	_value = value;
	_type = type;

	_displayName = displayName;
	// TODO should make empty do one thing and USTR do the other
/*
	if( displayName.isEmpty() ) {
		_displayName = _name;
	} else {
		_displayName = displayName;
	}
*/
	setObjectName( _name );
	_enabled = true;
	_checkable = false;
	_checked = false;
	_hasSetting = true;
	// can expand here for checkable/checked based on type
	if( _type == Action ) {
		_hasSetting = false;
	}
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
