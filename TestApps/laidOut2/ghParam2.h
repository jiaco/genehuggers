#ifndef	GH_PARAM2_H
#define	GH_PARAM2_H	1
#include <QtCore>
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT Param2	:	public	QVariant
{
public:
	enum	ParamType {
			UndefParam,
			CheckParam,
			ComboParam,
			EditParam,
			FileParam,
			GroupBoxParam,
			RadioParam,
			ListParam,
			StringListParam,
			ActionParam,
			ButtonParam
	};
	Param2();
	Param2( const QString& name, const QVariant& value,
		 const ParamType& type = UndefParam,
		 const QString& displayName = QString() );

	//	QVariant::setValue( T& value );
	//
	void	setName( const QString& name );
	void	setDisplayName( const QString& displayName );
	void	setDefaultValue( const QVariant& value );

	bool	isEnabled() const;
	void	setEnabled( const bool& tf = true );

	//	T QVariant::value() const
	//
	ParamType	type() const;
	QString		name() const;
	QString		displayName() const;
	QVariant	defaultValue() const;
private:
	QString		_name;
	QString		_displayName;
	QVariant	_defaultValue;
	ParamType	_type;
	bool		_enabled;
};
}	//	GH namespace
#endif	//	GH_PARAM_H
