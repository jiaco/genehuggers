#include "ghFileView.h"

namespace	GH
{
	FileView::FileView( ParamModel *model, QWidget *parent )
	: ParamView( model, parent )
{
	_widgetName = nameObject( "fileViewWidget" );
	_browseBtnName = nameObject( "browseButton" );
	_checkBoxName = nameObject( "checkBox" );

	//_browseAction = new QAction( "Browse", _parent );
	_browseAction = new QAction( _model->displayName(), _parent );
	_menu = new QMenu( _parent );
	_menu->addAction( _browseAction );

	connect( _browseAction, SIGNAL( triggered() ),
	 this, SLOT( doBrowse() ) );

	configure();
}
bool	FileView::SetCheckable( QObject* parent, const QString& name,
	 const bool& tf )
{
	FileView *p;

	if( ( p = GetParam<FileView>( parent, name, "GH::FileView" ) ) == 0 ) {
		return( false );
	}
	p->setCheckable( tf );
	return( true );
}
/*
bool	FileView::SetChecked( QObject* parent, const QString& name,
	 const bool& tf )
{
	FileView *p;

	if( ( p = GetParam<FileView>( parent, name, "GH::FileView" ) ) == 0 ) {
		return( false );
	}
	p->setChecked( tf );
	return( true );
}
*/
/*
soiv	FileView::setCheckable( const bool& tf )
{
	_checkable = tf;
}
*/
void	FileView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan,
	 Qt::Alignment alignment )
{
	int	atCol = col;

		QCheckBox *checkBox = new QCheckBox( _parent );
		checkBox->setObjectName( _checkBoxName );
		connect( checkBox, SIGNAL( toggled(bool) ),
		 this, SLOT( setChecked(bool) ) );
		connect( checkBox, SIGNAL( toggled(bool) ),
		 this, SIGNAL( checked(bool) ) );
		layout->addWidget( checkBox, row, atCol );
		++atCol;

	checkBox->setVisible( isCheckable() );
/*
	if( isCheckable() ) {
	}
*/
	QToolButton	*toolButton = new QToolButton( _parent );
	//toolButton->setArrowType( Qt::DownArrow );
	toolButton->setPopupMode( QToolButton::MenuButtonPopup );
	toolButton->setToolButtonStyle( Qt::ToolButtonIconOnly );
	toolButton->setMenu( _menu );
	toolButton->setText( _model->displayName() );
	toolButton->setObjectName( _browseBtnName );
	toolButton->setDefaultAction( _browseAction );

	QLineEdit *edit = new QLineEdit( V2S( _model->value() ), _parent );
	edit->setObjectName( _widgetName );
	connect( edit, SIGNAL( textEdited(QString) ),
	 this, SLOT( textEdited(QString) ) );

	layout->addWidget( toolButton, row, atCol );
	++atCol;
	layout->addWidget( edit, row, atCol, rowSpan, colSpan, alignment );


	//EditView::addToGrid( layout, row, useCol, rowSpan, colSpan, alignment );
	//useCol += colSpan;

	_startingDir = SL( _model->value() ).at( 0 );

/*
	QPushButton	*btn = new QPushButton( "...", _parent );
	btn->setObjectName( _browseBtnName );
	connect( btn, SIGNAL( pressed() ), _browseAction, SLOT( trigger() ) );
*/

	//
	//	if colSpan is sent in as -1 this will be wrong!
	//
	//layout->addWidget( btn, row, col + 1 + colSpan );

	updateEnabled();
}
/*
void	FileView::setChecked( const bool& value )
{
	SetChecked<QCheckBox>( _parent, _checkBoxName, value );
}
*/
/*
bool	FileView::Checked( QObject *parent, const QString& name )
{
	
}
bool	FileView::checked() const
{
	return( _model->isEnabled() );
}
void	FileView::toggle( const bool& value )
{
	_model->setEnabled( value );
	updateEnabled();
}
*/
void	FileView::textEdited( const QString& value )
{
	setValue( V( value ) );
}
void	FileView::updateValue()
{
	QList<QLineEdit *> edtab =
	 GetChildren<QLineEdit>( _parent, _widgetName );

	for( int i = 0; i < edtab.size(); ++i ) {
		if( !edtab[ i ]->hasFocus() ) {
			edtab[ i ]->setText( V2S( _model->value() ) );
		}
	}
}
void	FileView::updateChecked()
{
	SetChecked<QCheckBox>( _parent, _checkBoxName, _model->isChecked() );
}
void	FileView::updateEnabled()
{
	//EditView::updateEnabled();

/*
	QList<QPushButton *>	mytab =
	 GetChildren<QPushButton>( _parent, _browseBtnName );
*/
	//SetEnabled<QCheckBox>( _parent, _checkBoxName, _model->isEnabled() );
	SetEnabled<QCheckBox>( _parent, _checkBoxName, _model->isEnabled() );
	SetEnabled<QToolButton>( _parent, _browseBtnName, _model->isEnabled() );
	SetEnabled<QLineEdit>( _parent, _widgetName, _model->isEnabled() );
/*
	QList<QToolButton *>	mytab =
	 GetChildren<QToolButton>( _parent, _browseBtnName );

	for( int i = 0; i < mytab.size(); ++i ) {
		mytab[ i ]->setEnabled( _model->isEnabled() );
	}

	QList<QLineEdit *> edtab =
	 GetChildren<QLineEdit>( _parent, _widgetName );
	for( int i = 0; i < edtab.size(); ++i ) {
		edtab[ i ]->setEnabled( _model->isEnabled() );
	}
*/
}
void	FileView::configure( const DialogType& type,
	 const QString& caption, const QString& filter )
{
	_type = type;
	if( caption.isEmpty() ) {
		switch( _type ) {
			case	Dir:
				_caption = CAPTION_DEFAULT_DIR;
				break;
			case	SaveDir:
				_caption = CAPTION_DEFAULT_SAVEDIR;
				break;
			case	Open:
				_caption = CAPTION_DEFAULT_OPEN;
				break;
			case	OpenMulti:
				_caption = CAPTION_DEFAULT_OPENMULTI;
				break;
			case	Save:
				_caption = CAPTION_DEFAULT_SAVE;
				break;
		}
	} else {
		_caption = caption;
	}
	_filter = filter;
}
bool	FileView::Configure( QObject *parent, const QString& name,
	 const DialogType& type, const QString& caption, const QString& filter )
{
	FileView *p;

	if( ( p = GetParam<FileView>( parent, name, "GH::FileView" ) ) == 0 ) {
		return( false );
	}
	p->configure( type, caption, filter );
	return( true );
}
void	FileView::addAction( QAction* action )
{
	_menu->addAction( action );
}
bool	FileView::AddAction( QObject* parent, const QString& name,
	 QAction *action )
{
	FileView *p;

	if( ( p = GetParam<FileView>( parent, name, "GH::FileView" ) ) == 0 ) {
		return( false );
	}
	p->addAction( action );
	return( true );
}
void	FileView::setSinglePath( const QString& path )
{
	_startingDir = path;
	setValue( V( path ) );
}
void	FileView::setMultiplePath( const QStringList& path )
{
	if( path.size() > 0 ) {
		_startingDir = path.at( 0 );
	}
	setValue( QVariant( path ) );
}
void	FileView::doBrowse()
{
	QString		path;
	QStringList	pathList;

	LoseFocus<QLineEdit>( this, _widgetName );
	switch( _type ) {
		case	Dir:
		case	SaveDir:
			path = QFileDialog::getExistingDirectory(
			 _parent, _caption, _startingDir );
			if( path.isNull() ) {
				return;
			}
			setSinglePath( path );
			break;
		case	Open:
			path = QFileDialog::getOpenFileName(
			 _parent, _caption, _startingDir, _filter );
			if( path.isNull() ) {
				return;
			}
			setSinglePath( path );
			break;
		case	OpenMulti:
			pathList = QFileDialog::getOpenFileNames(
			 _parent, _caption, _startingDir, _filter );
			if( pathList.isEmpty() ) {
				return;
			}
			setMultiplePath( pathList );
			break;
		case	Save:
			path = QFileDialog::getSaveFileName(
			 _parent, _caption, _startingDir, _filter );
			if( path.isNull() ) {
				return;
			}
			setSinglePath( path );
			break;
	}
}
}	//	GH namespace
