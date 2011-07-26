#ifndef	GH_CONCURRENT_BASE
#define	GH_CONCURRENT_BASE	1
#include "ghDef.h"
#include "ghVariant.h"

namespace	GH
{
class	GH_DLL_EXPORT	ConcurrentBase
{
public:
	ConcurrentBase();
	void	init();
	void	setParams( const QMap<QString,QVariant>& params );
	void	injectParams( const QMap<QString,QVariant>& params );
	const QMap<QString,QVariant>& params() const;
protected:
	QMap<QString,QVariant>	_params;
};
}	//	GH namespace
#endif	//	GH_CONCURRENT_BASE
