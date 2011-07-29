#include "ghActionView.h"

namespace	GH
{
	ActionView::ActionView( ParamModel *model, QWidget *parent )
	: ParamView( model, parent )
{
	_pushButtonName = nameObject( "pushbutton" );
	_action = new QAction( _model->displayName(), _parent );
}
QPushButton*	ActionView::newPushButton()
{
	QPushButton *pushButton =
	 new QPushButton( _model->displayName(), _parent );

	pushButton->setObjectName( _pushButtonName );
	connect( pushButton, SIGNAL( pressed() ), _action, SLOT( trigger() ) );
	return( pushButton );
}
void	ActionView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	int	useCol = col;

	++useCol;	// Skip column zero (reserved for checkable-box)

	layout->addWidget( newPushButton(), row, useCol++,
	 rowSpan, colSpan, alignment );

	updateEnabled();
}
void	ActionView::updateValue()
{
	// nothing to do?
}
void	ActionView::updateEnabled()
{
	_action->setEnabled( _model->isEnabled() );
	SetEnabled<QPushButton>( _parent, _pushButtonName, _model->isEnabled() );
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
	 GetParam<ActionView>( parent, name, className() ) ) == 0 ) {
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
	 GetParam<ActionView>( parent, name, className() ) ) == 0 ) {
		return( 0 );
	}
	return( p->action() );
}
}	//	GH namespace
