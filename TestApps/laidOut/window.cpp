#include "window.h"

namespace	GH
{
	Window::Window( MyApp* app )
	: WindowApp( app )
{
	this->app = app;
	setWindowTitle( "" );
	loadStyle( "default" );

	QWidget	*_centralWidget = new QWidget( this );
	_centralWidget->setLayout( PUI( this, app, QStringList()
		<< "QtOnly" << "ActionParams" ) );
	setCentralWidget( _centralWidget );
	syncWithApp( app );

	ActionParam::AddListener( this, "QtOnly",
		this, SLOT( on_qtonly() ) );
	ActionParam::AddListener( this, "ActionParams",
		this, SLOT( on_actionparams() ) );
	QToolBar	*toolBar = new QToolBar( this );
	toolBar->setObjectName( "toolbar" );
	toolBar->addAction( ActionParam::Action( this, "QtOnly" ) );
	toolBar->addAction( ActionParam::Action( this, "ActionParams" ) );
	addToolBar( toolBar );
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
			app->addParam( s, "", Param::ActionParam );
			layout->addLayout( 
			 new ActionParam( this, app->pparam( s ) ), r, c );
		}
	}
	popup->show();
}
}	//	GH namespace
