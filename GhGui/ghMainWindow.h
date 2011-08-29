#ifndef	GH_MAINWINDOW_H
#define	GH_MAINWINDOW_H	1
#include "ghCliApp.h"
#include "ghErrorprone.h"
#include "ghParamView.h"
#include "ghEditView.h"
#include "ghFileView.h"
#include "ghColorView.h"
#include "ghFontView.h"
#include "ghActionView.h"
#include "ghBooleanView.h"
#include "ghChoiceView.h"

#include "ghLogWidget.h"
#include <QtGui>

namespace	GH
{
const	QString	MW_GEOM = "MW_GEOMETRY";
const	QString	MW_STATE = "MW_STATE";

const	QStringList	StyleChoices = QStringList() << "default" << "coffee" << "pagefold" << "mpjfc";

inline QWidget	*NonModalWindow( QWidget* parent = 0, Qt::WindowFlags f = Qt::Window ) 
{
	QWidget *rv;
#ifdef	Q_OS_WIN32
	rv = new QWidget( 0, f );
#else
	rv = new QWidget( parent, f );
#endif
	rv->setWindowModality( Qt::NonModal );

	return( rv );
}
void	MessageBox( const QString& text, const int& type = Error::Warn );

class	GH_DLL_EXPORT	MainWindow	:	public QMainWindow
{
	Q_OBJECT

public:
	MainWindow( CliApp *app );
	~MainWindow();
	void	initLogWidget( Qt::WindowFlags f = 0 );
	LogWidget*	logWidget();
	void	createParamView();
	int	pui( QGridLayout *layout, const int& startingRow = 0 );
	int	pui( QGridLayout *layout,
		 const QStringList& paramNames, const int& startingRow = 0 );

	void	setHelpText( const QString& text );
	void	setAboutText( const QString& text );
	QMenu*	appendHelpMenu( const QString& text = tr( "Help" ) );
public slots:
	void	loadStyle( const QString& sheetName = QString() );
	void	setSetting( const QString& name, const QVariant& value );
	void	showMessage( const QString& text, const int& type );
	void	showMessage( const Error& error );
	void	showMessage( Error* error );

	void	help();
	void	whatsThis();
	void	about();
signals:
	void	resetDefaults();
protected:
	ParamModel	*param( const QString& name ) const;
	ParamView	*paramView( const QString& name );
	QSettings	*_settings;
	QAction		*helpAction();
	QAction		*whatsThisAction();
	QAction		*aboutAction();
	QAction		*resetDefaultsAction();

	void	closeEvent( QCloseEvent *event );
	void	useLastGeometry();
private:
	QMap<QString,ParamView*>	_paramView;
	CliApp				*_app;
	LogWidget			*_logWidget;
	// These actions are default GUI functions that typically
	// go in the help menu and do not require persistence:
	//	Help		showHelp()
	//	WhatsThis	toggleWhatsThis() checkable
	//	Reset Defaults	resetDefaults()
	//	About		showAbout()
	//
	//	QString	_help, _about
	//	SLOT(CliApp)	resetDefaults
	//	ParamModel	needs tooltip/whatsthis support
	//	ParamView	handles events/signals during widget creation
	QAction		*_helpAction;
	QAction		*_aboutAction;
	QAction		*_whatsThisAction;
	QAction		*_resetDefaultsAction;
	QWidget		*_helpWidget;
	QWidget		*_aboutWidget;
	QString		_helpText;
	QString		_aboutText;
};
}	//	GH namespace
#endif	//	GH_MAINWINDOW_H
