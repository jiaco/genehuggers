#include "ghBooleanView.h"

namespace	GH
{
	BooleanView::BooleanView( ParamModel *model, QWidget *parent )
	: ParamView( model, parent )
{
	_widgetName = nameObject( "checkBox" );
	_action = new QAction( _model->displayName(), _parent );
	_action->setCheckable( true );

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
	//	addAsRadioButton( layout, row, col );
	} else {
		addAsCheckBox( layout, row, col, rowSpan, colSpan, alignment );
	//	addAsCheckBox( layout, row, col );
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
	 GetParam<BooleanView>( parent, name, "GH::BooleanView" ) ) == 0 ) {
		return( 0 );
	}
	return( p->action() );
}
/*
void	BooleanView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col, const WidgetType& widgetType )
{
	switch( widgetType ) {
		case	RadioButton:
			return( addAsRadioButton( layout, row, col ) );
			break;
		default:
			return( addAsCheckBox( layout, row, col ) );
			break;
	}
}
*
bool	BooleanView::AddChild( QObject* parent, const QString& name,
	 const int& row, const int& col )
{
	QList<QGroupBox*>	tab = GetChildren<QGroupBox>( parent,
	 _widgetName );
	for( int i = 0; i < tab.size(); ++i ) {
		QWidget *w = 
		tab->layout()->addWidget( w );
	}
}
*/
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
/*
	QGroupBox	*groupBox =
	 new QGroupBox( _model->displayName(), _parent );
*/

	groupBox->setCheckable( true );
	groupBox->setChecked( B( _model->value() ) );

	groupBox->setObjectName( _widgetName );
	groupBox->setLayout( innerLayout );
	connect( groupBox, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );

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
	 GetParam<BooleanView>( parent, name, "GH::BooleanView" ) ) == 0 ) {
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
/*
	QGroupBox	*groupBox =
	 new QGroupBox( _model->displayName(), _parent );
*/

	groupBox->setCheckable( true );
	groupBox->setChecked( B( _model->value() ) );

	groupBox->setObjectName( _widgetName );
	groupBox->setLayout( innerLayout );
	connect( groupBox, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );

	layout->addWidget( groupBox, row, col );

	updateEnabled();
}
bool	BooleanView::AddAsGroupBox( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col,
	  QGridLayout *innerLayout )
{
	BooleanView *p;

	if( ( p =
	 GetParam<BooleanView>( parent, name, "GH::BooleanView" ) ) == 0 ) {
		return( false );
	}
	p->addAsGroupBox( layout, row, col, innerLayout );
	return( true );
}
void	BooleanView::addAsCheckBox( QGridLayout* layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	QCheckBox	*checkBox =
	 new QCheckBox( _model->displayName(), _parent );

	checkBox->setChecked( B( _model->value() ) );

	checkBox->setObjectName( _widgetName );
	connect( checkBox, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );

	int	useCol = col;
	++useCol;
	layout->addWidget( checkBox, row, useCol, rowSpan, colSpan, alignment );

	updateEnabled();
}
bool	BooleanView::AddAsCheckBox( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col )
{
	BooleanView *p;

	if( ( p =
	 GetParam<BooleanView>( parent, name, "GH::BooleanView" ) ) == 0 ) {
		return( false );
	}
	p->addAsCheckBox( layout, row, col );
	return( true );
}
void	BooleanView::addAsRadioButton( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	QRadioButton	*w =
	 new QRadioButton( _model->displayName(), _parent );

	w->setChecked( B( _model->value() ) );

	w->setObjectName( _widgetName );
	connect( w, SIGNAL( toggled(bool) ),
	 this, SLOT( toggle(bool) ) );

	layout->addWidget( w, row, col, rowSpan, colSpan, alignment );

	updateEnabled();
}
bool	BooleanView::AddAsRadioButton( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col )
{
	BooleanView *p;

	if( ( p =
	 GetParam<BooleanView>( parent, name, "GH::BooleanView" ) ) == 0 ) {
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
	SetChecked<QCheckBox>( _parent, _widgetName, B( _model->value() ) );
	SetChecked<QGroupBox>( _parent, _widgetName, B( _model->value() ) );
	SetChecked<QRadioButton>( _parent, _widgetName, B( _model->value() ) );
}
void	BooleanView::updateEnabled()
{
	SetEnabled<QCheckBox>( _parent, _widgetName, _model->isEnabled() );
	SetEnabled<QGroupBox>( _parent, _widgetName, _model->isEnabled() );
	SetEnabled<QRadioButton>( _parent, _widgetName, _model->isEnabled() );
}
}	//	GH namespace
