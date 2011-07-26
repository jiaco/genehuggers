#ifndef	GH_ACTIONPARAM2_H
#define	GH_ACTIONPARAM2_H	1
#include "ghBaseParam2.h"
#include "ghGuiCommon.h"

namespace	GH
{
class	GH_DLL_EXPORT	ActionParam2	:	public	BaseParam2
{
	Q_OBJECT
public:
	ActionParam2( QWidget* parent, Param2* param );
	QAction*	action();
	static QAction*	Action( QWidget *w, const QString& name );
	QPushButton*	button();
	void		addListener( QObject* obj, const char* slot );
	static bool	AddListener( QWidget* w, const QString& name,
			 QObject* obj, const char* slot );

	void	update( const QVariant& value,
		 const bool& silent = false );
signals:
	void	clicked( bool checked = false );
	void	pressed();
	void	released();
	void	toggled( bool checked );
protected:
	QAction		*_action;
	QPushButton	*_button;
};
}	//	GH namespace
#endif	//	GH_ACTIONPARAM_H
