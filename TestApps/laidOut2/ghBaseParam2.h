#ifndef	GH_BASEPARAM2_H
#define	GH_BASEPARAM2_H	1
#include <QtGui>
#include <GhCore.h>
#include "ghObjApp.h"
#include "ghParam2.h"

namespace	GH
{
class	GH_DLL_EXPORT	BaseParam2	: public	QGridLayout
{
	Q_OBJECT

public:
	BaseParam2( QWidget* parent, Param2* param );

	//
	//	IMPORTANT, QSettings goes into subclasses
	//		through the update( value ) method
	//
	virtual void		update( const QVariant& value,
				 const bool& silent = false ) = 0;
	Param2::ParamType	paramType() const;

	QLayout* asLayout();

	void	setLastValue( const QVariant& value );
	//void	leaveEvent( QEvent* event );
	void	refreshEnabled();
public slots:
	void	update();
signals:
	void	changed( const QString& name, const QVariant& value );
protected:
	QVariant	_lastValue;
	Param2		*_param;
};

}	//	GH namespace
#endif	//	GH_BASEPARAM_H
