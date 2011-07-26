#ifndef	GH_IDF_H
#define	GH_IDF_H	1
#include "ghIfp.h"
#include "ghOfp.h"
namespace	GH
{
const	QString	GH_IDF_MARK = "GH::";

class	GH_DLL_EXPORT	IDF
{
public:
		IDF();
	bool	getPath();
	bool	set( const QString& key, const QString& value,
		 const bool& overwrite = false );
	QString	get( const QString& key );
	bool	removeKey( const QString& key );
	bool	load();
	bool	write();
	void	showAll( QTextStream& fp );
	
	static QString	lookup( const QString& key );
	static bool	install( const QString& key, const QString& value,
			 const bool& overwrite = false );
	static void	show( QTextStream& fp );
	static bool	remove( const QString& key );

private:
	QString			_env, _path;
	QMap<QString,QString>	_map;
};
}	//	GH namespace
#endif	//	GH_IDF_H
