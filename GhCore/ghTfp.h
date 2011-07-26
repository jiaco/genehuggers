#ifndef	GH_TFP_H
#define	GH_TFP_H	1
#include "ghDef.h"
#include "ghErrorprone.h"
#include <QTemporaryFile>

namespace	GH	{

class	GH_DLL_EXPORT	Tfp	: public QTextStream, public GH::Errorprone
{
public:
	bool	init();
	bool	open( const QString& fileTemplate = QString() );
	void	close();
	const QString&	getPath() const;
	void	setTmp( const QString& tpath );
	qint64	fpos() const;

	static	QString	TempPath( const QString& name );
private:
	QString	path;
	QTemporaryFile	file;
};

}	//	GH namespace
#endif	//	GH_OFP_H
