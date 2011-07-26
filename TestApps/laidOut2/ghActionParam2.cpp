#include "ghActionParam2.h"

namespace	GH
{
	ActionParam2::ActionParam2( QWidget* parent, Param2* param )
	: BaseParam2( parent, param )
{
	_action = new QAction( _param->displayName(), parent );
	_button = new QPushButton( _param->displayName(), parent );
	addWidget( _button );

	connect( _button, SIGNAL( clicked(bool) ),
	 this, SIGNAL( clicked(bool) ) );
	connect( _button, SIGNAL( clicked(bool) ),
	 _action, SIGNAL( triggered(bool) ) );
	connect( _button, SIGNAL( toggled(bool) ),
	 this, SIGNAL( toggled(bool) ) );
	connect( _button, SIGNAL( pressed() ), this, SIGNAL( pressed() ) );
	//connect( _button, SIGNAL( pressed() ), _action, SLOT( trigger() ) );
	connect( _button, SIGNAL( released() ), this, SIGNAL( released() ) );
	
	//resize( _button->size() );
}
void	ActionParam2::update( const QVariant& value, const bool& silent )
{
	Q_UNUSED( value );
	Q_UNUSED( silent );
}
QAction*	ActionParam2::action()
{
	return( _action );
}
QAction*	ActionParam2::Action( QWidget *w, const QString& name )
{
	ActionParam2 *p;
	if( !( p = w->findChild<ActionParam2 *>( name ) ) ) {
		return( 0 );
	}
	if( !p->inherits( "GH::ActionParam2" ) ) {
		return( 0 );
	}
	return( p->action() );
}
QPushButton*	ActionParam2::button()
{
	return( _button );
}
void	ActionParam2::addListener( QObject *obj, const char* slot )
{
	connect( _action, SIGNAL( triggered() ), obj, slot );
}
bool	ActionParam2::AddListener( QWidget *w, const QString& name,
	 QObject *obj, const char* slot )
{
	ActionParam2 *p;
	if( !( p = w->findChild<ActionParam2 *>( name ) ) ) {
		return( false );
	}
	if( !p->inherits( "GH::ActionParam2" ) ) {
		return( false );
	}
	p->addListener( obj, slot );
	return( true );
}

}	//	GH namespace
