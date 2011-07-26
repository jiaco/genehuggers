#include "window.h"
#include "ghPui2.h"

namespace	GH
{
	Window::Window( MyApp* app )
	: WindowApp( app )
{
	QWidget	*centralWidget = new QWidget( this );
	
	this->app = app;
	setWindowTitle( "" );
	loadStyle( "default" );

	centralWidget->setLayout( PUI2( this, app,
	 QStringList() << "QtOnly" << "ActionParams" << "ActionParam2s" ) );
	setCentralWidget( centralWidget );
	syncWithApp( app );

	ActionParam2::AddListener( this, "QtOnly",
		this, SLOT( on_qtonly() ) );
	ActionParam2::AddListener( this, "ActionParams",
		this, SLOT( on_actionparams() ) );
	ActionParam2::AddListener( this, "ActionParam2s",
		this, SLOT( on_actionparam2s() ) );
/*
	QToolBar	*toolBar = new QToolBar( this );
	toolBar->setObjectName( "toolbar" );
	toolBar->addAction( ActionParam::Action( this, "QtOnly" ) );
	toolBar->addAction( ActionParam::Action( this, "ActionParams" ) );
	addToolBar( toolBar );
*/
}
void	Window::on_qtonly()
{
	QWidget *popup = new QWidget();
	QGridLayout *layout = new QGridLayout;
	popup->setLayout( layout );
	popup->setWindowTitle( "Qt-only" );

	for( int r = 0; r < 5; ++r ) {
		for( int c = 0; c < 5; ++c ) {
			QString s = QString( "Btn%1-%2" )
					.arg( r + 1 ).arg( c + 1 );
			QPushButton *pb = new QPushButton( s, this );
			layout->addWidget( pb, r, c );
		}
	}


	popup->show();
}
void	Window::on_actionparams()
{
	QWidget *popup = new QWidget();
	QGridLayout *layout = new QGridLayout;
	popup->setLayout( layout );
	popup->setWindowTitle( "ActionParams" );

	for( int r = 0; r < 5; ++r ) {
		for( int c = 0; c < 5; ++c ) {
			QString s = QString( "Btn%1-%2" )
					.arg( r + 1 ).arg( c + 1 );
			layout->addWidget( 
			 new ActionParam( this, app->pparam( s ) ), r, c );
		}
	}
	popup->show();
}
void	Window::on_actionparam2s()
{
	QWidget *popup = new QWidget();
	QGridLayout *layout = new QGridLayout;
	popup->setLayout( layout );
	popup->setWindowTitle( "ActionParams" );

	for( int r = 0; r < 5; ++r ) {
		for( int c = 0; c < 5; ++c ) {
			QString s = QString( "Btn%1-%2" )
					.arg( r + 1 ).arg( c + 1 );
			app->addParam2( Param2( s, "", Param2::ActionParam) );
			layout->addLayout( 
			 new ActionParam2( this, app->pparam2( s ) ), r, c );
		}
	}
	popup->show();
}
}	//	GH namespace
