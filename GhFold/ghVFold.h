#ifndef	GH_VFOLD_H
#define	GH_VFOLD_H	1

extern "C"	{
#include <fold.h>
#include <fold_vars.h>
}
#include "GhCore.h"

namespace	GH
{
class	Fold
{
public:
	Fold();
	void	init();
	float	minFold( const QString& seq, QString& structure );
private:
	bool	paramsUpdated;
};
}	//	GH namespace
#endif
