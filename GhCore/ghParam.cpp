#include "ghParam.h"

namespace	GH
{
	Param::Param()
{
	_name = QString();
	_defaultValue = QVariant();
	_type = UndefParam;
	_displayName = _name;
	_enabled = true;
}
	Param::Param( const QString& name, const QVariant& value,
		 const Param::ParamType& type,
		 const QString& displayName  )
	: QVariant( value )
{
	_name = name;
	_defaultValue = value;
	_type = type;
	_displayName = displayName;
	_enabled = true;
	if( _displayName.isEmpty() ) {
		_displayName = _name;
	}	
}
bool	Param::isEnabled() const
{
	return( _enabled );
}
void	Param::setEnabled( const bool& tf )
{
	_enabled = tf;
}
void	Param::setName( const QString& name )
{
	_name = name;
}
void	Param::setDisplayName( const QString& displayName )
{
	_displayName = displayName;
}
void	Param::setDefaultValue( const QVariant& value )
{
	_defaultValue = value;
}
//
//	QVariant::setValue( const T& value ) in parent class
//
Param::ParamType	Param::type() const
{
	return( _type );
}
QString		Param::name() const
{
	return( _name );
}
QString		Param::displayName() const
{
	return( _displayName );
}
QVariant	Param::defaultValue() const
{
	return( _defaultValue );
}
//
//	T QVariant::value() const in parent class
//
}	//	GH namespace
