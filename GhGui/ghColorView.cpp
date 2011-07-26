#include "ghColorView.h"

namespace	GH
{
	ColorView::ColorView( ParamModel *model, QWidget *parent )
	 : EditView( model, parent )
{
	_browseBtnName = nameObject( "browseButton" );
	_browseAction = new QAction( _model->displayName(), _parent );
	_menu = new QMenu( _parent );
	_menu->addAction( _browseAction );

	connect( _browseAction, SIGNAL( triggered() ),
	 this, SLOT( doBrowse() ) );
}
void	ColorView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	Q_UNUSED( rowSpan );
	Q_UNUSED( colSpan );
	Q_UNUSED( alignment );

	EditView::addToGrid( layout, row, col );

	QToolButton *toolButton = new QToolButton( _parent );
        toolButton->setPopupMode( QToolButton::MenuButtonPopup );
        toolButton->setToolButtonStyle( Qt::ToolButtonIconOnly );
        toolButton->setMenu( _menu );
        toolButton->setText( _model->displayName() );
        toolButton->setObjectName( _browseBtnName );
        toolButton->setDefaultAction( _browseAction );

/*
	QPushButton	*btn = new QPushButton( "Select", _parent );
	btn->setObjectName( _browseBtnName );
	connect( btn, SIGNAL( pressed() ), _browseAction, SLOT( trigger() ) );
*/


	layout->addWidget( toolButton, row, col + 2 );

	updateEnabled();
}
void	ColorView::updateEnabled()
{
	EditView::updateEnabled();

	SetEnabled<QToolButton>( this, _browseBtnName, _model->isEnabled() );
/*
	QList<QPushButton *> mytab =
		GetChildren<QPushButton>( _parent, _browseBtnName );
	for( int i = 0; i < mytab.size(); ++i ) {
		mytab[ i ]->setEnabled( _model->isEnabled() );
	}
*/
}
void	ColorView::doBrowse()
{
	QColor color = QColorDialog::getColor( CLR( _model->value() ),
		_parent, "Pick a Color"  );

	setValue( QVariant( color ) );
}
}	//	GH namespace
