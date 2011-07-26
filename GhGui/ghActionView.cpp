#include "ghActionView.h"

namespace	GH
{
	ActionView::ActionView( ParamModel *model, QWidget *parent )
	: ParamView( model, parent )
{
	_action = new QAction( _model->displayName(), _parent );

	_btnName = nameObject( "button" );
}
/*
void	ActionView::addToGrid( QGridLayout *layout, const int& row, const int& col )
{
	QPushButton *btn = new QPushButton( _model->displayName(), _parent );

	btn->setObjectName( _btnName );

	connect( btn, SIGNAL( pressed() ), _action, SLOT( trigger() ) );
	layout->addWidget( btn, row, col );

	updateEnabled();
}
*/
void	ActionView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	QPushButton *btn = new QPushButton( _model->displayName(), _parent );

	btn->setObjectName( _btnName );

	connect( btn, SIGNAL( pressed() ), _action, SLOT( trigger() ) );
	int	useCol = col;
	++useCol;
	layout->addWidget( btn, row, useCol, rowSpan, colSpan, alignment );

	updateEnabled();
}
void	ActionView::updateValue()
{
	// nothing to do?
}
void	ActionView::updateEnabled()
{
	_action->setEnabled( _model->isEnabled() );
	SetEnabled<QPushButton>( _parent, _btnName, _model->isEnabled() );
}

void	ActionView::addListener( QObject *obj, const char *slot )
{
	connect( _action, SIGNAL( triggered() ), obj, slot );
}
bool	ActionView::AddListener( QObject *parent, const QString& name,
		QObject *obj, const char *slot )
{
	ActionView *p;

	if( ( p =
	 GetParam<ActionView>( parent, name, "GH::ActionView" ) ) == 0 ) {
		return( false );
	}
	p->addListener( obj, slot );
	return( true );
}
QAction*	ActionView::action() const
{
	return( _action );
}
QAction*	ActionView::Action( QObject *parent, const QString& name )
{
	ActionView	*p;
	if( ( p =
	 GetParam<ActionView>( parent, name, "GH::ActionView" ) ) == 0 ) {
		return( 0 );
	}
	return( p->action() );
}
}	//	GH namespace
