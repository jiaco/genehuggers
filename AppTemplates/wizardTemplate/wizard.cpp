#include "wizard.h"

namespace	GH
{
	Wizard::Wizard( MyApp* app )
	: WizardApp( app )
{
	this->app = app;
	setWindowTitle( "wizard template" );
}
}	//	GH namespace
