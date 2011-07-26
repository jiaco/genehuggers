#ifndef	WINDOW_H
#define	WINDOW_H	1
#include <GhCore.h>
#include <GhGui.h>
#include "myApp.h"

namespace	GH
{

class	Window	:	public	WindowApp
{
	Q_OBJECT

public:
	Window( MyApp* app = 0 );

public slots:
	void	on_qtonly();
	void	on_actionparams();
private:
	MyApp	*app;
};

}	//	GH namespace
#endif	//	WINDOW_H
