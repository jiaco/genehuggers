#include "ghMainWindow.h"

namespace	GH
{

void	MessageBox( const QString& text, const int& type )
{
	QMessageBox     msg;
	QMessageBox::Icon icon;
	switch( type ) {
	case    Error::Info:
		icon = static_cast<QMessageBox::Icon>(
		 QMessageBox::Information );
		break;
	case    Error::Warn:
		icon = static_cast<QMessageBox::Icon>(
		 QMessageBox::Warning );
		break;
	case    Error::Crit:
		icon = static_cast<QMessageBox::Icon>(
		 QMessageBox::Critical );
		break;
	default:
		icon = static_cast<QMessageBox::Icon>( type );
		break;
	}
	msg.setIcon( icon );
	msg.setText( text );
	msg.exec();
}




	MainWindow::MainWindow( CliApp *app )
	: QMainWindow()
{
	_app = app;
	_logWidget = 0;

	_helpText = _aboutText = QString();
	_helpWidget = _aboutWidget = 0;
	_helpAction = _aboutAction = 0;
	_whatsThisAction = _resetDefaultsAction = 0;

	// added June 14, 2011 due to issues with different versions
	// possibly creating a fiasco on windows where one release
	// would crash due to ?things in the registry?
	//
	//QSettings::setDefaultFormat( QSettings::IniFormat );
	_settings = new QSettings();

	connect( _app, SIGNAL( emitError(Error) ),
	 this, SLOT( showMessage(Error) ) );

	connect( _app, SIGNAL( emitError(Error*) ),
	 this, SLOT( showMessage(Error*) ) );

	_helpAction = new QAction( tr( "Help" ), this );
	connect( _helpAction, SIGNAL( triggered() ),
	 this, SLOT( help() ) );

	_aboutAction = new QAction( tr( "About" ), this );
	connect( _aboutAction, SIGNAL( triggered() ),
	 this, SLOT( about() ) );

	_whatsThisAction = QWhatsThis::createAction( this );
	_whatsThisAction->setCheckable( true );
	connect( _whatsThisAction, SIGNAL( triggered() ),
	 this, SLOT( whatsThis() ) );
	
	_resetDefaultsAction = new QAction( tr( "Reset Defaults" ), this );
	connect( _resetDefaultsAction, SIGNAL( triggered() ),
	 this, SIGNAL( resetDefaults() ) );
	connect( this, SIGNAL( resetDefaults() ),
	 _app, SLOT( resetDefaults() ) );
// evidently tooltips on actions in menus do not show up..
	_resetDefaultsAction->setToolTip(
	 "Reset all parameters to default values"
	 );
	_resetDefaultsAction->setWhatsThis(
	 "<font color=#000000>"
	 "If you find that you have completely messed up"
	 " parameter values (which are now saved in your application settings)"
	 " this action will reset them all to their initial, default values"
	 "</font>" 
	);
	useLastGeometry();

	createParamView();

	ChoiceView::SetChoices( this, StyleParam, StyleChoices );
	ConnectToSlot<ChoiceView>( this, StyleParam,
	 SIGNAL( activated(QString) ), this, SLOT( loadStyle(QString) ) );


	loadStyle( S( param( StyleParam )->value() ) );

	if( !_app->arg( "STYLE" ).isEmpty() ) {
		loadStyle( _app->arg( "STYLE" ) );
	}
}
	MainWindow::~MainWindow()
{
	delete	_settings;
}
void	MainWindow::setHelpText( const QString& text )
{
	_helpText = text;
}
void	MainWindow::setAboutText( const QString& text )
{
	_aboutText = text;
}
QAction*	MainWindow::helpAction()
{
	return( _helpAction );
}
QAction*	MainWindow::aboutAction()
{
	return( _aboutAction );
}
QAction*	MainWindow::whatsThisAction()
{
	return( _whatsThisAction );
}
QAction*	MainWindow::resetDefaultsAction()
{
	return( _resetDefaultsAction );
}
QMenu*	MainWindow::appendHelpMenu( const QString& text )
{
	QMenu *rv = menuBar()->addMenu( text );
	rv->addAction( helpAction() );
	rv->addAction( whatsThisAction() );
	rv->addSeparator();
	rv->addAction( resetDefaultsAction() );
	rv->addAction( aboutAction() );
	
	return( rv );
}
void	MainWindow::help()
{
	if( _helpWidget == 0 ) {
		if( _helpText.isEmpty() ) {
			return;
		}
		if( _helpText.startsWith( ':' ) ) {
			_helpText = Ifp::load( _helpText );
		}
		if( _helpText.isEmpty() ) {
			_helpText = "Sorry but no help is available";
		}
		QGridLayout	*lay = new QGridLayout;
		QScrollArea	*sa = new QScrollArea;
		QLabel		*lbl = new QLabel;
		_helpWidget = NonModalWindow( this );
		QPushButton	*closeButton =
		 new QPushButton( "Close", _helpWidget );

		_helpWidget->setLayout( lay );
		_helpWidget->setObjectName( "HelpWidget" );

		lbl->setText( _helpText );
		lbl->setWordWrap( true );
		sa->setWidget( lbl );
		lay->addWidget( sa, 0, 0, 1, 4 );
		lay->addWidget( closeButton, 1, 3, 1, 1 );

		connect( closeButton, SIGNAL( pressed() ),
		 this, SLOT( help() ) );
	}
	if( _helpWidget->isVisible() ) {
		_helpWidget->hide();
	} else {
		_helpWidget->show();
	}
}
void	MainWindow::about()
{
	if( _aboutWidget == 0 ) {
		if( _aboutText.isEmpty() ) {
			return;
		}
		if( _aboutText.startsWith( ':' ) ) {
			_aboutText = Ifp::load( _aboutText );
		}
		if( _aboutText.isEmpty() ) {
			_aboutText = "Created with GeneHuggers.\n"
			 " No other information is available...";
		}
		QGridLayout	*lay = new QGridLayout;
		QLabel		*lbl = new QLabel;
		_aboutWidget = NonModalWindow( this );
		QPushButton	*closeButton =
		 new QPushButton( "Close", _aboutWidget );
		_aboutWidget->setLayout( lay );
		_aboutWidget->setObjectName( "AboutWidget" );
		lbl->setText( _aboutText );
		lbl->setWordWrap( true );
		lay->addWidget( lbl, 0, 0, 1, 4 );
		lay->addWidget( closeButton, 1, 3, 1, 1 );

		connect( closeButton, SIGNAL( pressed() ),
		 this, SLOT( about() ) );
	}
	if( _aboutWidget->isVisible() ) {
		_aboutWidget->hide();
	} else {
		_aboutWidget->show();
	}
	return;
}
void	MainWindow::whatsThis()
{
	// apparently whatsThis is a quick toggle that goes
	// off as soon as it is used. the whole checkable
	// concept is a waste.
	//
	// bugger about the QPalette on GNOME (ubuntu)
	// the tooltip is white on back but the whatsthis
	// is white on yellow which is invisible...
	//	 current workaround is in ParamModel
	// 	where the returned string is wrapped in font=red
	_whatsThisAction->setChecked( true );
	QWhatsThis::enterWhatsThisMode();
/*
	if( QWhatsThis::inWhatsThisMode() ) {
qDebug() << "DEBUG LEAVE WHATS THIS MODE";
		_whatsThisAction->setChecked( false );
		QWhatsThis::leaveWhatsThisMode();
	} else {
qDebug() << "DEBUG ENTER WHATS THIS MODE";
		_whatsThisAction->setChecked( true );
		QWhatsThis::enterWhatsThisMode();
	}
*/
}
void	MainWindow::initLogWidget( Qt::WindowFlags f )
{
	_logWidget = new LogWidget( this, f );
}
LogWidget*	MainWindow::logWidget()
{
	return( _logWidget );
}
void	MainWindow::loadStyle( const QString& sheetName )
{
	QFile	file;
	QString	sname, fname;

	sname = "default";
	if( !sheetName.isEmpty() ) {
		sname = sheetName;
	}
	if( sname.endsWith( "qss" ) ) {
		fname = sname;
	} else {
		fname = QString( ":style/%1.qss" ).arg( sname );
	}
	file.setFileName( fname );
	if( file.open( QFile::ReadOnly ) ) {
		qApp->setStyleSheet( QLatin1String( file.readAll() ) );
		file.close();
	}
}
void	MainWindow::closeEvent( QCloseEvent *event )
{
	_settings->setValue( MW_GEOM, saveGeometry() );
	_settings->setValue( MW_STATE, saveState() );
	QMainWindow::closeEvent( event );
}
ParamModel*	MainWindow::param( const QString& name ) const
{
	return( _paramView[ name ]->model() );
}
ParamView*	MainWindow::paramView( const QString& name )
{
	return( _paramView[ name ] );
}
int	MainWindow::pui( QGridLayout *layout,
	 const QStringList& paramNames, const int& startingRow )
{
	int	row = startingRow;
	foreach( QString pname, paramNames ) {
		_paramView[ pname ]->addToGrid( layout, row, 0 );
		++row;
	}
	return( row );
}
int	MainWindow::pui( QGridLayout *layout, const int& startingRow )
{
	return( pui( layout, _app->paramNames(), startingRow ) );
}
void	MainWindow::showMessage( const Error& error )
{
	showMessage( error.message(), error.type() );
}
void	MainWindow::showMessage( Error* error )
{
	showMessage( error->message(), error->type() );
}
void	MainWindow::showMessage( const QString& text, const int& type )
{
	if( type == Error::Info && _logWidget != 0 ) {
		_logWidget->log( text );
		return;
	}
	MessageBox( text, type );
}	
void	MainWindow::useLastGeometry()
{
	if( _settings->contains( MW_GEOM ) ) {
		restoreGeometry( _settings->value( MW_GEOM ).toByteArray() );
	}
	if( _settings->contains( MW_STATE ) ) {
		restoreState( _settings->value( MW_STATE ).toByteArray() );
	}
}
/*
void	MainWindow::paramChanged( const QString& name, const QVariant& value )
{
	_settings->setValue( name, value );
}
*/
void	MainWindow::createParamView()
{
	foreach( QString pname, _app->paramNames() ) {
		if( _paramView.contains( pname ) ) {
/* whoops
			qFatal(
			 QString( "Bad param name used %1" )
			 .arg( pname ).toLocal8Bit().data() 
			);
*/
			continue;
		}
		if( _settings->contains( pname ) ) {
			_app->setParamValue( pname, _settings->value( pname ) );
		}
		switch( _app->param( pname )->baseType() ) {
			case	ParamModel::Edit:
				_paramView.insert( pname,
				  new EditView( _app->param( pname ), this ) );
				break;
			case	ParamModel::File:
				_paramView.insert( pname,
				  new FileView( _app->param( pname ), this ) );
				break;
			case	ParamModel::Color:
				_paramView.insert( pname,
				  new ColorView( _app->param( pname ), this ) );
				break;
			case	ParamModel::Font:
				_paramView.insert( pname,
				  new FontView( _app->param( pname ), this ) );
				break;
			case	ParamModel::Action:
				_paramView.insert( pname,
				 new ActionView( _app->param( pname ), this ) );
				break;
			case	ParamModel::Boolean:
				_paramView.insert( pname,
				 new BooleanView( _app->param( pname ), this ) );
				break;
			case	ParamModel::Choice:
				_paramView.insert( pname,
				 new ChoiceView( _app->param( pname ), this ) );
				break;
			default:
// TODO : once found that using an stupid/invalid ParamModel constructor
// triggered this here and then qWarn()-ed about the connect below
// so this would be a good place to count and then qFatal() at the end
// to make sure that all is correct
				break;
		}
		// ALL SIGNALS are connected in the ParamView constructor
		// but this one that handles the settings
		connect( _paramView[ pname ],
		 SIGNAL( setSetting(QString,QVariant) ),
		 this, SLOT( setSetting(QString,QVariant) ) );
	}
}
void	MainWindow::setSetting( const QString& name, const QVariant& value )
{
	if( _app->param( name )->hasSetting() ) {
		_settings->setValue( name, value );
	}
}
}	//	GH namespace

