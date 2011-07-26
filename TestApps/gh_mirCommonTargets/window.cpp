#include "window.h"

namespace	GH
{
	Window::Window( MyApp* app )
	: WindowApp( app )
{
	this->app = app;
	setWindowTitle( "" );

	setCentralWidget( PUI( this, app ) );

	FileParam::Configure( this, "Input",
	 FileParam::Open, "Select an Input", "*.txt" );

/*
	Paster *inputPaster = new Paster( this, "Input",
	 this, SLOT( pasteInput() ) );
*/
/*
 * problem with all this is that app cannot control it
 * the window needs to spawn a widget get the text on close
 * and then send that text into the app
	QAction	*act_pasteInput = new QAction( this );
	connect( act_pasteInput, SIGNAL( triggered() ),
	 app, SLOT( pasteInput() ) );
	FileParam::AddButton( this, "Input", "Paste", act_pasteInput );

	FileParam::AddButton( this, "Input",
	 "Paste", app, SLOT( pasteInput(QString&) ) );
 */

	syncWithApp( app );
}
}	//	GH namespace
