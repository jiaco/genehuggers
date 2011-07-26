#ifndef	GH_PARAMMODEL_H
#define	GH_PARAMMODEL_H	1
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT	ParamModel	:	public	QObject
{
	Q_OBJECT
public:
	static const int BaseTypeAndValue = 0x0007;

	enum Type {
		Undef = 0x0000,		// 0 0000
		Edit = 0x0001,		// 0 0001
		File = 0x0002,		// 0 0010
		Action = 0x0003,	// 0 0011
		Boolean = 0x0004,	// 0 0100
		Choice = 0x0005,	// 0 0101
		Color = 0x0006,		// 0 0110
		Font = 0x0007,		// 0 0111
					// 0 1111
	//	BaseTypeAndValue = 0x0007,
		// Boolean subsets
		// DEFAULT Check
		Radio = 0x0010,		// 0001 0000
		// Choice subsets
		// DEFAULT Combo
		Selector = 0x0100,	// 0010 0000
		CheckGroup = 0x0200,	// 0011 0000
		RadioGroup = 0x0300	// 0100 0000

	};
	Q_DECLARE_FLAGS( ParamType, Type );
	
	ParamModel( QObject *parent = 0 );
	ParamModel( const QString& name, const QVariant& value,
		const ParamType& type = Undef,
		const QString& displayName = QString(),
		QObject *parent = 0 );
/*
	void	setValue( const QVariant& value );
	void	guiSetValue( const QVariant& value );
*/

	QString		name() const;
	QString		displayName() const;
	QVariant	value() const;
	ParamType	type() const;
	ParamType	baseType() const;

	bool	isEnabled() const;
	bool	isCheckable() const;
	bool	isChecked() const;

	bool	hasSetting() const;

public slots:
	void	setValue( const QVariant& value );
	void	guiSetValue( const QVariant& value );

	void	resetDefault();
	void	setEnabled( const bool& enabled );
	void	guiSetEnabled( const bool& enabled );
	void	setCheckable( const bool& tf );
	void	guiSetCheckable( const bool& tf );
	void	setChecked( const bool& tf );
	void	guiSetChecked( const bool& tf );
signals:
	// should change this to valueChanged(value)
	// to accomodate changed() for enabled/checkable
	// and then toggled(bool) for checked
	// for now use checked(bool)
	void	changed( const QVariant& value );
	void	enabled( const bool& tf );
	//void	toggled( const bool& tf );
	void	checked( const bool& tf );

	//void	signalChanged( const QString& name, const QVariant& value );
	//void	updateView();
private:
	bool		_hasSetting;
	bool		_enabled;
	bool		_checkable;
	bool		_checked;
	QVariant	_value;
	QVariant	_defaultValue;
	QString		_name;
	QString		_displayName;
	ParamType	_type;
};
Q_DECLARE_OPERATORS_FOR_FLAGS( ParamModel::ParamType );
}	//	GH namespace
#endif	//	GH_PARAMMODEL_H