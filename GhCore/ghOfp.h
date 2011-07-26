#ifndef	GH_OFP_H
#define	GH_OFP_H	1
#include "ghDef.h"
#include "ghErrorprone.h"

namespace	GH	{

const	QString	STDOUT = "stdout";
const	QString	STDERR = "stderr";

class	GH_DLL_EXPORT	Ofp	: public QTextStream, public GH::Errorprone
{
public:
	bool	open( const QString& usePath );
	bool	append( const QString& usePath );
	QString	getPath() const;
	void	close();
	qint64	fpos() const;
	void	ml( const QString& buf );
private:
	QString	path;
	QFile	file;
	//qint64	byteCount;
};

}	//	GH namespace
#endif	//	GH_OFP_H
