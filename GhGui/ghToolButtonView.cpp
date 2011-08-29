#include "ghToolButtonView.h"

namespace	GH
{
	ToolButtonView::ToolButtonView( ParamModel *model, QWidget *parent )
	: EditView( model, parent )
{
	// checkBox and lineEdit are named in EditView
	_toolButtonName = nameObject( "toolButtonToolButton" );

	_defaultAction = new QAction( _model->displayName(), _parent );
	_menu = new QMenu( _parent );
	_menu->addAction( _defaultAction );
	connect( _defaultAction, SIGNAL( triggered() ),
		this, SIGNAL( defaultAction() ) );
}
QToolButton*	ToolButtonView::newToolButton()
{
	QToolButton	*toolButton = new QToolButton( _parent );

	toolButton->setObjectName( _toolButtonName );
	toolButton->setPopupMode( QToolButton::MenuButtonPopup );
	toolButton->setToolButtonStyle( Qt::ToolButtonIconOnly );
	toolButton->setMenu( _menu );
	toolButton->setText( _model->displayName() );
	toolButton->setDefaultAction( _defaultAction );

	setHelpTexts( toolButton );

	return( toolButton );
}
void	ToolButtonView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan,
	 Qt::Alignment alignment )
{
	int	useCol = col;

	layout->addWidget( newCheckBox(), row, useCol++ );
	layout->addWidget( newToolButton(), row, useCol++ );
	layout->addWidget( newLineEdit(), row, useCol++,
	 rowSpan, colSpan, alignment );

	updateEnabled();
}
void	ToolButtonView::addAction( QAction *action )
{
	_menu->addAction( action );
}
bool	ToolButtonView::AddAction( QObject *parent, const QString& name,
	 QAction *action )
{
	ToolButtonView *p;

	if( ( p =
	 GetParam<ToolButtonView>( parent, name, className() ) ) == 0 ) {
		return( false );
	}
	p->addAction( action );
	return( true );
}
void	ToolButtonView::updateEnabled()
{
	// checkbox and lineedit handled by EditView
	EditView::updateEnabled();
	SetEnabled<QToolButton>( _parent, _toolButtonName, _model->isEnabled() );
}
void	ToolButtonView::updateChecked()
{
	SetChecked<QCheckBox>( _parent, _checkBoxName, _model->isChecked() );
}
}	//	GH namespace
