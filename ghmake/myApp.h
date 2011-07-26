#ifndef	MY_APP_H
#define	MY_APP_H	1
#include <GhCore.h>

namespace	GH
{
class	MyApp
{
public:
	MyApp( int argc, char** argv );

	bool	doit();

	enum Command {
		App, Nsi, Fail
	};
	enum Option {
		Window, DBase, Wizard, Cli
	};
	bool	usage();
private:
	QMap<QString,QString>	args;
	Ofp	cout, cerr;

	QString	USAGE, arg0;
	Command cmd;
	Option	opt;

	QString	appName, appType;;
	QDir	pwd, ghHome;

	bool	doApp();
	bool	doNsi();

	bool	copyFile( const QString& oldPath, const QString& newPath );
	bool	copyProFile( const QString& oldPath, const QString& newPath );
	QStringList	srcFiles;
};

}	//	GH namespace
#endif	//	MY_APP_H
