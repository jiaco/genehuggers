#ifndef	GH_GUIBASE_H
#define	GH_GUIBASE_H	1
#include <QtGui>
#include <GhCore.h>
#include "ghBaseParam.h"
#include "ghObjApp.h"

/*
 *	GuiBase -> ghWidget -> ghWindow
 *			  -> ghWizard
 */

namespace	GH
{

class	GH_DLL_EXPORT	GuiBase	:	public	QWidget
{
	Q_OBJECT

public:
	GuiBase( ObjApp* app = 0 );
	~GuiBase();

	QVariant	getSetting( const QString& name );
public slots:
	void	updateSetting( const QString& name, const QVariant& value );
	void	showMessage( const QString& text, const int& type );
protected:
	QSettings	*_settings;
};

}	//	GH namespace
#endif
