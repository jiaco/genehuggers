#include "ghChoiceView.h"

namespace	GH
{
	ChoiceView::ChoiceView( ParamModel *model, QWidget *parent )
	: ParamView( model, parent )
{
	_comboName = nameObject( "choiceComboBox" );
	_selBtnName = nameObject( "choiceSelectorButton" );
	_selEditName = nameObject( "choiceSelectorEdit" );

	_previousSelection = V2S( _model->value() );
}
void	ChoiceView::setChoices( const QStringList& choices )
{
	QList<QComboBox *>	mytab =
		 GetChildren<QComboBox>( _parent, _comboName );

	_choices = choices;

	blockSignals( true );
	for( int i = 0; i < mytab.size(); ++i ) {
		mytab[ i ]->clear();
		mytab[ i ]->addItems( _choices );
	}
	blockSignals( false );
	setValue( _model->value() );
}
bool	ChoiceView::SetChoices( QObject *parent, const QString& name,
		const QStringList& choices )
{
	ChoiceView	*p;

	if( ( p =
	 GetParam<ChoiceView>( parent, name, "GH::ChoiceView" ) ) == 0 ) {
		return( false );
	}
	p->setChoices( choices );
	return( true );
}
void	ChoiceView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	Q_UNUSED( rowSpan );
	Q_UNUSED( colSpan );
	Q_UNUSED( alignment );

	if( ( _model->type() ^ ParamModel::Choice ) == ParamModel::Selector ) {
		addAsSelector( layout, row, col );
	} else if( ( _model->type() ^ ParamModel::Choice ) == ParamModel::RadioGroup ) {
		addAsRadioGroup( layout, row, col );
	} else if( ( _model->type() ^ ParamModel::Choice ) == ParamModel::CheckGroup ) {
	} else {
		addAsCombo( layout, row, col );
	}
	updateEnabled();
}
void	ChoiceView::addAsCombo( QGridLayout *layout,
	 const int& row, const int& col )
{
	QLabel *label = new QLabel( _model->displayName(), _parent );
	QComboBox	*comboBox = new QComboBox( _parent );

	comboBox->setObjectName( _comboName );
	connect( comboBox, SIGNAL( currentIndexChanged(QString) ),
	 this, SLOT( currentIndexChanged(QString) ) );
	connect( comboBox, SIGNAL( activated(int) ),
	 this, SIGNAL( activated(int) ) );
	connect( comboBox, SIGNAL( activated(QString) ),
	 this, SIGNAL( activated(QString) ) );

	int	useCol = col;
	++useCol;
	layout->addWidget( label, row, useCol );
	++useCol;
	layout->addWidget( comboBox, row, useCol );

	if( !_choices.isEmpty() ) {
		setChoices( _choices );
	}
	updateEnabled();
}
bool	ChoiceView::AddAsCombo( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col )
{
	ChoiceView *p;

	if( ( p =
	 GetParam<ChoiceView>( parent, name, "GH::ChoiceView" ) ) == 0 ) {
		return( false );
	}
	p->addAsCombo( layout, row, col );
	return( true );
}
void	ChoiceView::addAsSelector( QGridLayout *layout,
	 const int& row, const int& col )
{
	QLabel *label = new QLabel( _model->displayName(), _parent );
	QLineEdit *edit = new QLineEdit( V2S( _model->value() ), _parent );
	edit->setEnabled( false );
	edit->setObjectName( _selEditName );
	QPushButton	*button = new QPushButton( tr( "Browse" ), _parent );
	button->setObjectName( _selBtnName );
	
	connect( button, SIGNAL( clicked() ),
	 this, SLOT( showSelector() ) );

	layout->addWidget( label, row, col );
	layout->addWidget( edit, row, col + 1 );
	layout->addWidget( button, row, col + 2 );
}
bool	ChoiceView::AddAsSelector( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col )
{
	ChoiceView *p;

	if( ( p =
	 GetParam<ChoiceView>( parent, name, "GH::ChoiceView" ) ) == 0 ) {
		return( false );
	}
	p->addAsSelector( layout, row, col );
	return( true );
}
void	ChoiceView::addAsRadioGroup( QGridLayout *layout,
	 const int& row, const int& col )
{
	QButtonGroup	*btnGroup = new QButtonGroup( _parent );
	QGroupBox	*btnGroupBox =
	 new QGroupBox( _model->displayName(), _parent );
	QGridLayout	*lay = new QGridLayout;
	btnGroupBox->setLayout( lay );
	btnGroup->setExclusive( true );

	int	grow = 0;
	for( int i = 0; i < _choices.size(); ++i ) {
		QRadioButton *b =
		 new QRadioButton( _choices.at( i ), _parent );
		b->setObjectName( _selBtnName );
		if( _choices.at( i ) == _previousSelection ) {
			b->setChecked( true );
		}
		btnGroup->addButton( b, i );
		if( i % 2 == 0 ) {
			lay->addWidget( b, grow, 0 );
		} else {
			lay->addWidget( b, grow, 1 );
			++grow;
		}
	}
	
	connect( btnGroup, SIGNAL( buttonClicked(int) ),
		this, SLOT( currentButtonChanged(int) ) );

	layout->addWidget( btnGroupBox, row, col, 1, -1 );
}
bool	ChoiceView::AddAsRadioGroup( QObject *parent, const QString& name,
	 QGridLayout *layout, const int& row, const int& col )
{
	ChoiceView *p;

	if( ( p =
	 GetParam<ChoiceView>( parent, name, "GH::ChoiceView" ) ) == 0 ) {
		return( false );
	}
	p->addAsRadioGroup( layout, row, col );
	return( true );
}
void	ChoiceView::currentIndexChanged( const QString& value )
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
}
void	ChoiceView::currentButtonChanged( const int& id )
{
	// for now this is only used by RadioGroup and that is hardwired to be exclusive
	setValue( V( _choices.at( id ) ) );
	_previousSelection = V2S( _model->value() );
/*
	QString	btnChoice = _choices.at( id );
	QStringList	plist = _model->value().toStringList();

	if( plist.contains( btnChoice ) ) {
		plist.removeAt( plist.indexOf( btnChoice ) );
	} else {
		plist << btnChoice;
	}
	setValue( V( plist ) );
	_previousSelection = V2S( _model->value() );
*/
}
void	ChoiceView::showSelector()
{
	setValue( V( StringListDialog::selectStringList( 0,
		_choices, _previousSelection.split( "," ) ) ) );

	_previousSelection = V2S( _model->value() );
}
void	ChoiceView::updateValue()
{
	updateCombo();
	updateSelEdit();
}
void	ChoiceView::updateCombo()
{
	QList<QComboBox *>	mytab =
		 GetChildren<QComboBox>( _parent, _comboName );

	for( int i = 0; i < mytab.size(); ++i ) {
		mytab[ i ]->setCurrentIndex(
		 _choices.indexOf( S( _model->value() ) ) );
	}
}
void	ChoiceView::updateSelEdit()
{
	QList<QLineEdit *>	mytab =
		GetChildren<QLineEdit>( _parent, _selEditName );

	for( int i = 0; i < mytab.size(); ++i ) {
		mytab[ i ]->setText( V2S( _model->value() ) );
	}
}
void	ChoiceView::updateEnabled()
{
	SetEnabled<QComboBox>( _parent, _comboName, _model->isEnabled() );
	SetEnabled<QPushButton>( _parent, _selBtnName, _model->isEnabled() );
	SetEnabled<QRadioButton>( _parent, _selBtnName, _model->isEnabled() );
}
}	//	GH namespace
