#include "ghBooleanView.h"

namespace	GH
{
	BooleanView::BooleanView( ParamModel *model, QWidget *parent )
	: ParamView( model, parent )
{
	_checkBoxName = nameObject( "checkbox" );
	_groupBoxName = nameObject( "groupbox" );
	_radioButtonName = nameObject( "radiobutton" );

	_action = new QAction( _model->displayName(), _parent );
	_action->setCheckable( true );
	setHelpTexts( _action );

	// here the action toggles the other widgets
	// but the other widgets are not toggling the action...
	// so I put the _action->setChecked() in toggle, but wonder
	// why that is not setting up infinite signals/slots
	//
	connect( _action, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );
}
void	BooleanView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{

	if( ( _model->type() & ParamModel::Radio ) != 0 ) {
		addAsRadioButton( layout, row, col, rowSpan, colSpan, alignment );
	} else {
		addAsCheckBox( layout, row, col, rowSpan, colSpan, alignment );
	}
	updateEnabled();
}
QAction*	BooleanView::action() const
{
	return( _action );
}
QAction*	BooleanView::Action( QObject *parent, const QString& name )
{
	BooleanView	*p;

	if( ( p =
	 GetParam<BooleanView>( parent, name, className() ) ) == 0 ) {
		return( 0 );
	}
	return( p->action() );
}
void	BooleanView::addAsGroupBox( QGridLayout* layout,
	 const int& row, const int& col, QGridLayout* innerLayout,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	QGroupBox	*groupBox;

	if( _model->displayName().isEmpty() ) {
		groupBox = new QGroupBox( _parent );
		groupBox->setFlat( true );
	} else {
		groupBox = new QGroupBox( _model->displayName(), _parent );
	}
	groupBox->setObjectName( _groupBoxName );
	groupBox->setCheckable( true );
	groupBox->setChecked( B( _model->value() ) );
	groupBox->setLayout( innerLayout );
	connect( groupBox, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );

	setHelpTexts( groupBox );
	layout->addWidget( groupBox, row, col, rowSpan, colSpan, alignment );

	updateEnabled();
}
bool	BooleanView::AddAsGroupBox( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col,
	  QGridLayout *innerLayout,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	BooleanView *p;

	if( ( p =
	 GetParam<BooleanView>( parent, name, className() ) ) == 0 ) {
		return( false );
	}
	p->addAsGroupBox( layout, row, col, innerLayout, rowSpan, colSpan, alignment );
	return( true );
}
void	BooleanView::addAsGroupBox( QGridLayout* layout,
	 const int& row, const int& col, QGridLayout* innerLayout )
{
	QGroupBox	*groupBox;

	if( _model->displayName().isEmpty() ) {
		groupBox = new QGroupBox( _parent );
	} else {
		groupBox = new QGroupBox( _model->displayName(), _parent );
	}

	groupBox->setObjectName( _groupBoxName );
	groupBox->setCheckable( true );
	groupBox->setChecked( B( _model->value() ) );
	groupBox->setLayout( innerLayout );
	connect( groupBox, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );

	layout->addWidget( groupBox, row, col );

	setHelpTexts( groupBox );
	updateEnabled();
}
bool	BooleanView::AddAsGroupBox( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col,
	  QGridLayout *innerLayout )
{
	BooleanView *p;

	if( ( p =
	 GetParam<BooleanView>( parent, name, className() ) ) == 0 ) {
		return( false );
	}
	p->addAsGroupBox( layout, row, col, innerLayout );
	return( true );
}
QCheckBox*	BooleanView::newCheckBox()
{
	QCheckBox	*checkBox =
	 new QCheckBox( _model->displayName(), _parent );

	checkBox->setObjectName( _checkBoxName );
	checkBox->setChecked( B( _model->value() ) );
	connect( checkBox, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );

	setHelpTexts( checkBox );
	
	return( checkBox );
}
void	BooleanView::addAsCheckBox( QGridLayout* layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	int	useCol = col;
	++useCol;		// column zero reserved for checkable-box
	layout->addWidget( newCheckBox(), row, useCol++,
	 rowSpan, colSpan, alignment );

	updateEnabled();
}
bool	BooleanView::AddAsCheckBox( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col )
{
	BooleanView *p;

	if( ( p =
	 GetParam<BooleanView>( parent, name, className() ) ) == 0 ) {
		return( false );
	}
	p->addAsCheckBox( layout, row, col );
	return( true );
}
QRadioButton*	BooleanView::newRadioButton()
{
	QRadioButton	*radioButton =
	 new QRadioButton( _model->displayName(), _parent );

	radioButton->setObjectName( _radioButtonName );
	radioButton->setChecked( B( _model->value() ) );
	connect( radioButton, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );

	setHelpTexts( radioButton );

	return( radioButton );
}
void	BooleanView::addAsRadioButton( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	int useCol = col;
	++useCol;	//	reserved for checkable-box

	layout->addWidget( newRadioButton(), row, useCol++,
	 rowSpan, colSpan, alignment );

	updateEnabled();
}
bool	BooleanView::AddAsRadioButton( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col )
{
	BooleanView *p;

	if( ( p =
	 GetParam<BooleanView>( parent, name, className() ) ) == 0 ) {
		return( false );
	}
	p->addAsRadioButton( layout, row, col );
	return( true );
}
void	BooleanView::toggle( const bool& value )
{
	// when the GUI wants to be changed, ParamView::setValue( QVariant )
	// is the method to call. It will do 3 things:
	//	1) emit( changed( value ) )
	//		--->>> connected to ParamModel::guiSetValue
	//	2) call virtual updateValue()
	//		subclasses must update widgets
	//	3) emit( setSetting(QString,QVariant) )
	//		--->>> connected to MainWindow

	setValue( V( value ) );
	_action->setChecked( B( _model->value() ) );
}
void	BooleanView::updateValue()
{
	SetChecked<QCheckBox>( _parent, _checkBoxName, B( _model->value() ) );
	SetChecked<QGroupBox>( _parent, _groupBoxName, B( _model->value() ) );
	SetChecked<QRadioButton>( _parent, _radioButtonName, B( _model->value() ) );
}
void	BooleanView::updateEnabled()
{
	SetEnabled<QCheckBox>( _parent, _checkBoxName, _model->isEnabled() );
	SetEnabled<QGroupBox>( _parent, _groupBoxName, _model->isEnabled() );
	SetEnabled<QRadioButton>( _parent, _radioButtonName, _model->isEnabled() );
}
}	//	GH namespace
