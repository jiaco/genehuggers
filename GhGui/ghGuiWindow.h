#ifndef	GH_GUIWINDOW_H
#define	GH_GUIWINDOW_H
#include <QtGui>
#include <GhCore.h>
#include "ghBaseParam.h"
#include "ghObjApp.h"
#include "ghGuiBase.h"

namespace	GH
{
const	QString	_GEOM = "GW_GEOMETRY";
const	QString	_STATE = "GW_STATE";

class	GH_DLL_EXPORT	GuiWindow	:	public QMainWindow, public GuiBase
{
	Q_OBJECT

public:
	GuiWindow( ObjApp* app = 0 );
	~GuiWindow();

protected:
	void	closeEvent( QCloseEvent* event );
	void	useLastGeometry();
};
}	//	GH namespace
#endif	//	GH_GUIWINDOW_H
