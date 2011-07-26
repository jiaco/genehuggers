#include "ghStat.h"

namespace	GH
{
double	lFactorial( const int& n )
{
	double rv = 0;
	if( n <= 0 ) {
		return( rv );
	}
	for( int i = 2; i <= n; ++i ) {
		rv += log( i );
	}
	return( rv );
}
	TTGrid::TTGrid()
{
	a = b = c = d = 0;
}
double	TTGrid::pvalue()
{
	return( _pvalue );
}
double	TTGrid::calcPValue()
{
	_lpvalue	= 0;
	double	base = 2;

	_lpvalue = lFactorial( a + b ) + lFactorial( c + d ) +
		lFactorial( a + c ) + lFactorial( b + d );
	_lpvalue -= ( lFactorial( a + b + c + d ) +
		lFactorial( a ) + lFactorial( b ) +
		lFactorial( c ) + lFactorial( d ) );

	//return( rv );
	_pvalue = pow( base,_lpvalue );
	//return( pow( base, rv ) );
	return( _pvalue );
}

}	//	GH namespace
