#include "window.h"

namespace	GH
{
QWidget	*myW( const QString& label, QWidget* parent = 0 )
{
	QWidget *rv = new QWidget( parent );
	QVBoxLayout *lay = new QVBoxLayout;

	rv->setLayout( lay );
	lay->setSpacing( 0 );
	lay->addWidget( p );

	return( rv );
}
	Window::Window( MyApp* app )
	: WindowApp( app )
{
	this->app = app;
	setWindowTitle( "" );

	QWidget *main = new QWidget( this );
	QGridLayout	*lay = new QGridLayout;
	main->setLayout( lay );
	lay->setHorizontalSpacing( 0 );
	lay->setVerticalSpacing( 0 );

	for( int r = 0; r < 5; ++r ) {
		for( int c = 0; c < 5; ++c ) {
			QWidget	*w = new QWidget( main );
			QVBoxLayout *l = new QVBoxLayout;
			//QFrame	*w = new QFrame( main );
			l->setSpacing( 0 );
			w->setLayout( l );
			w->setObjectName( "wrapper" );
			
			//QGridLayout *l = new QGridLayout;
		
			QString label = QString( "%1-%2" ).arg( r ).arg( c );
			QPushButton *p = new QPushButton( label, main );
			//lay->addWidget( p, r, c );

			//l->addWidget( p, 0, 0 );

			//lay->addLayout( l, r, c );

			l->addWidget( p );
			lay->addWidget( w, r, c );

		}
	}

	loadStyle( "default" );

	setCentralWidget( main );
}
}	//	GH namespace
