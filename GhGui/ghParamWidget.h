#ifndef	GH_PARAM_WIDGET_H
#define	GH_PARAM_WIDGET_H	1
#include <QtGui>
#include "ghDef.h"
#include "ghParam.h"

namespace	GH
{
class	GH_DLL_EXPORT ParamWidget	:	public	QWidget
{
	Q_OBJECT

public:
	ParamWidget( QWidget* parent = 0 );

	void	setParams( Param& param );
	void	refresh( Param& param );
	void	updateParams( Param& param );
private:	
	QFormLayout	*lay;
};
}	//	GH namespace
#endif	//	GH_PARAM_WIDGET_H
