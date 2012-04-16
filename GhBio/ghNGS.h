#ifndef	GH_NGS_H
#define	GH_NGS_H	1
#include <GhCore.h>
#include "ghFast.h"

namespace	GH
{
class	GH_DLL_EXPORT	Sff
{
public:
	QString	sid;
	QString	seq;
	QList<int>	qual;
	QList<double>	flow;
	int		endFlow;
};
class	GH_DLL_EXPORT	SffLoader
{
public:
	SffLoader();
	~SffLoader();
	bool	setBasePath( const QString& basePath );
	bool	next( Sff& sff );
private:
	QString	_basePath;
	QString	_pathFast, _pathQual, _pathFlow;
	QString	_suffixFast, _suffixQual, _suffixFlow;
	Ifp	_fpFast, _fpQual, _fpFlow;
};
}	//	GH namespace
#endif	//	GH_NGS_H
