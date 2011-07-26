#include <GhCore.h>
#include "myApp.h"
#include "wizard.h"

using namespace	GH;

int	main( int argc, char** argv )
{
	QApplication	qapp( argc, argv );
	MyApp		app( argc, argv );
	Wizard		*wizard;

	//Q_INIT_RESOURCE( headers );

	if( app.isGui() ) {
		QCoreApplication::setOrganizationName( GH_ORG_NAME );
		QCoreApplication::setOrganizationDomain( GH_ORG_DOMAIN );
		QCoreApplication::setApplicationName( app.arg0() );
		wizard = new Wizard( &app );
		wizard->show();
		return( qapp.exec() );
	} else {
		app.doit();
		return( 0 );
	}
}
