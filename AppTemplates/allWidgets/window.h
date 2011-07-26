#ifndef	WINDOW_H
#define	WINDOW_H	1
#include <GhCore.h>
#include <GhGui.h>
#include "myApp.h"

namespace	GH
{

class	Window	:	public	MainWindow
{
	Q_OBJECT

public:
	Window( MyApp* app = 0 );

private:
	QGridLayout	*_centralLayout;
	QWidget		*_centralWidget;
	MyApp	*_myApp;
};

}	//	GH namespace
#endif	//	WINDOW_H
