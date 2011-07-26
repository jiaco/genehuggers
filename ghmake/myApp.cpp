#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
{
	arg0 = argv[ 0 ];
	USAGE = "%1: [ command ] [ option(s) ]\n"
		"command = { app }\n"
		"   app option(s) = type app-name\n"
		"    - window creates a window template app in pwd\n"
		"    -db creates a dbase template app in pwd\n"
		"    - wizard creates a wizard template app in pwd\n"
		"    - cli creates a cli template app in pwd\n"
		"   nsi [ no-options ]\n"
		"    - nsi creates a Windows nsi installer for app in pwd\n"
		"";

	cout.open( STDOUT );
	cerr.open( STDERR );

	cmd = Fail;
	if( argc == 2 ) {
		cmd = Nsi;
	} else if( argc == 4 ) {
		cmd = App;
		appType = argv[ 2 ];
		appType = appType.toLower();
		if( appType == "window" ) {
			opt = Window;
		} else if( appType.startsWith( "db" ) ) {
			appType = "dbase";
			opt = DBase;
		} else if( appType == "wizard" ) {
			opt = Wizard;
		} else if( appType == "cli" ) {
			opt = Cli;
		}
		appName = argv[ 3 ];
	}
}
bool	MyApp::usage()
{
	cerr << QString( USAGE ).arg( arg0 ) << endl;
	return( false );
}
bool	MyApp::doit()
{
	bool	rv;

	ghHome = QDir( HOME() );

	switch( cmd ) {
		case	Nsi:
			rv = doNsi();
			break;
		case	App:
			rv = doApp();
			break;
		default:
			rv = usage();
			break;
	}
	return( rv );
}
bool	MyApp::copyFile( const QString& oldPath, const QString& newPath )
{
	Ifp	ifp;
	Ofp	ofp;
	QString	line;

	if( !ifp.open( oldPath ) ) {
		return( false );
	}
	if( !ofp.open( newPath ) ) {
		return( false );
	}
	while( nextLine( ifp, line ) ) {
		ofp << line << endl;
	}
	ifp.close();
	ofp.close();
	return( true );
}
bool	MyApp::copyProFile( const QString& oldPath, const QString& newPath )
{
	Ifp	ifp;
	Ofp	ofp;
	QString	line;
	Token	tok( Lexer::WhiteSpace );

	if( !ifp.open( oldPath ) ) {
		return( false );
	}
	if( !ofp.open( newPath ) ) {
		return( false );
	}
	while( nextLine( ifp, line ) ) {
		if( line.startsWith( "TGT" ) ) {
			ofp << "TGT=\"" << appName << "\"" << endl;
		} else { 
			ofp << line << endl;
		}
		tok.split( line );
		foreach( QString s, tok ) {
			if( s.endsWith( ".h" ) || s.endsWith( ".cpp" ) ) {
				srcFiles << s;
			}
		}
	}
	ifp.close();
	ofp.close();
	return( true );
}
bool	MyApp::doApp()
{
	bool	rv = false;

	QString appPath, templatePath, proFile;

	appPath = QString( "%1/%2" ).arg( pwd.absolutePath() )
		.arg( appName );
	templatePath = QString( "%1/AppTemplates/%2Template" )
		.arg( ghHome.absolutePath() ).arg( appType );
	proFile = QString( "%1/%2Template.pro" ).arg( templatePath )
		.arg( appType );

	pwd.mkpath( appPath );
	copyProFile( proFile,
	 QString( "%1/%2.pro" ).arg( appPath ).arg( appName ) );

	foreach( QString srcFile, srcFiles ) {
		copyFile( QString( "%1/%2" ).arg( templatePath ).arg( srcFile ),
			 QString( "%1/%2" ).arg( appPath ).arg( srcFile ) );
	}

	
	return( rv );
}
bool	MyApp::doNsi()
{
	Ifp	ifp;
	Ofp	ofp;
	QString	line;
	QString	ghPath;

	char *env;
	if( ( env = getenv( "GH_PATH" ) ) == 0 ) {
		ghPath = "C:\\gh";
	} else {
		ghPath = QString( env );
	}
	

	appName = FileName( pwd.absolutePath() );
	QMap<QString,QString>	far;	// find and replace

	far.insert( "APP", appName );
	far.insert( "PATH", ghPath );

	if( !ifp.open( ":/installer/default.nsi" ) ) {
		cerr << "NOPE" << endl;
		return( false );
	}
	if( !ofp.open( QString( "%1.nsi" ).arg( appName ) ) ) {
		return( false );
	}
	QString pat = "GH_(\\S*)_VAR";
	QRegExp	exp = QRegExp( pat );
	while( nextLine( ifp, line ) ) {
		if( ( exp.indexIn( line ) ) != -1 ) {
			line.replace( exp, far[ exp.cap( 1 ) ] );
		}
		ofp << line << endl;
	}
	
	ofp.close();
	ifp.close();
	return( true );
}

}	//	GH namespace
