#include "window.h"

namespace	GH
{
	Window::Window( MyApp* app )
	: MainWindow( app )
{
	_myApp = app;
	setWindowTitle( "myProgram" );

	_centralLayout = new QGridLayout;
	_centralWidget = new QWidget( this );

	_centralWidget->setLayout( _centralLayout );

	FileView::SetCheckable( this, "file/file1", true );
	pui( _centralLayout, 0 );
	ActionView::AddListener( this, "Doit", _myApp, SLOT( doit() ) );
	setCentralWidget( _centralWidget );

	appendHelpMenu( "?" );
}
}	//	GH namespace
