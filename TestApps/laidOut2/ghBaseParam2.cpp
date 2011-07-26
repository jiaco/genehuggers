#include "ghBaseParam2.h"

namespace	GH
{
	BaseParam2::BaseParam2( QWidget* parent, Param2* param )
	 : QGridLayout()
{
	_param = param;
	setObjectName( _param->name() );
}
QLayout* BaseParam2::asLayout()
{
	return( this );
}
Param2::ParamType	BaseParam2::paramType() const
{
	return( _param->type() );
}
void	BaseParam2::setLastValue( const QVariant& value )
{
	_lastValue = value;
qDebug() << "BaseParam2::setLastValue " << _param->name() << " = " << value;
}
void	BaseParam2::refreshEnabled()
{
qDebug() << "BASE PARAM REFRESH ENABLED CALLED";
	setEnabled( _param->isEnabled() );
}
void	BaseParam2::update()
{
	//QWidget::update();
/*
	foreach( QWidget *w, this->findChildren<QWidget *>() ) {
		w->update();
	}
*/
}

}	//	GH namespace
