#ifndef	GH_STAT_H
#define	GH_STAT_H	1

#include "ghRow.h"
#include "math.h"

namespace	GH
{
double	lFactorial( const int& n );

class	GH_DLL_EXPORT	TTGrid
{
public:
	TTGrid();
	//static	Hdr	header();

	double	pvalue();
	double	calcPValue();
	//void	show( QTextStream& fp, const QString& label );
	int	a, b, c, d;
	double	_pvalue, _lpvalue;
};
}	//	GH namespace
#endif
