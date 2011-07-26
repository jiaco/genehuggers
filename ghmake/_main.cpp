#include <GhCore.h>
#include "myApp.h"

using namespace	GH;

int	main( int argc, char** argv )
{
	Q_INIT_RESOURCE( installer );

	QApplication	qapp( argc, argv, CLI );
	MyApp		app( argc, argv );

	app.doit();
	return( 0 );
}
