#include "window.h"

namespace	GH
{
	Window::Window( MyApp* app )
	: MainWindow( app )
{
	_myApp = app;
	setWindowTitle( "Demo" );
	//createParamView();
	//loadStyle( S( param( "Style" )->value() ) );
	//loadStyle( "mpjfc" );

	_centralLayout = new QGridLayout;
	_centralWidget = new QWidget( this );
	_centralWidget->setLayout( _centralLayout );
	FileView::SetCheckable( this, "FileCheck", true );
	pui( _centralLayout, 0 );
	setCentralWidget( _centralWidget );

	ChoiceView::SetChoices( this, "Combo", QStringList() << "one" << "two" << "5" );
}
}	//	GH namespace
