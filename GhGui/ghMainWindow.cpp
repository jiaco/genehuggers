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

	_resetDefaults = new QAction( tr( "Reset Defaults" ), this );
	connect( _resetDefaults, SIGNAL( triggered() ),
	 this, SIGNAL( resetDefaults() ) );
	connect( this, SIGNAL( resetDefaults() ),
	 _app, SLOT( resetDefaults() ) );

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
QAction*	MainWindow::resetDefaultsAction()
{
	return( _resetDefaults );
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
			// qWarn?
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

