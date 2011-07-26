#include "ghParam2.h"

namespace	GH
{
	Param2::Param2()
{
	_name = QString();
	_defaultValue = QVariant();
	_type = UndefParam;
	_displayName = _name;
	_enabled = true;
}
	Param2::Param2( const QString& name, const QVariant& value,
		 const Param2::ParamType& type,
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
bool	Param2::isEnabled() const
{
	qDebug() << "PARAM ISENABLED CALLED " << _name << " " << _enabled;
	return( _enabled );
}
void	Param2::setEnabled( const bool& tf )
{
	_enabled = tf;
}
void	Param2::setName( const QString& name )
{
	_name = name;
}
void	Param2::setDisplayName( const QString& displayName )
{
	_displayName = displayName;
}
void	Param2::setDefaultValue( const QVariant& value )
{
	_defaultValue = value;
}
//
//	QVariant::setValue( const T& value ) in parent class
//
Param2::ParamType	Param2::type() const
{
	return( _type );
}
QString		Param2::name() const
{
	return( _name );
}
QString		Param2::displayName() const
{
	return( _displayName );
}
QVariant	Param2::defaultValue() const
{
	return( _defaultValue );
}
//
//	T QVariant::value() const in parent class
//
}	//	GH namespace
