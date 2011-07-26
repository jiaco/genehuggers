#ifndef	WIZARD_H
#define	WIZARD_H	1
#include <GhCore.h>
#include <GhGui.h>
#include "myApp.h"

namespace	GH
{

class	Wizard	:	public	WizardApp
{
	Q_OBJECT

public:
	Wizard( MyApp* app = 0 );

private:
	EditParam	*p1, *p2;
	MyApp	*app;
};

}	//	GH namespace
#endif	//	WIZARD_H
