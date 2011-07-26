#ifndef	GH_MATH_H
#define	GH_MATH_H	1
#include "ghDef.h"

namespace	GH
{
inline	double	lFactorial( const int& n )
{
	double	rv = 0;
	if( n <= 0 ) {
		return( rv );
	}
	for( int i = 2; i <=n; ++i ) {
		rv += log( i );
	}
	return( rv );
}
inline double	ttPvalue( const int& a, const int& b,
		 const int& c, const int& d )
{
	double	rv = 0;
	double	base = 2;

	rv = lFactorial( a + b ) + lFactorial( c + d ) +
		lFactorial( a + c ) + lFactorial( b + d );
	rv -= ( lFactorial( a + b + c + d ) +
		lFactorial( a ) + lFactorial( b ) +
		lFactorial( c ) + lFactorial( d ) );

	return( pow( base, rv ) );
}
	//
	//	Below are many template functions
	//
template<typename T>
T	Sum( const QList<T>& data )
{
	T rv = 0;

	foreach( T t, data ) {
		rv += t;
	}
	return( rv );
}
template<typename T>
T	Sum( const QVector<T>& data )
{
	T rv = 0;

	foreach( T t, data ) {
		rv += t;
	}
	return( rv );
}
template<typename T>
T	SumSquares( const QList<T>& data )
{
	T rv = 0;

	foreach( T t, data ) {
		rv += ( t * t );
	}
	return( rv );
}
template<typename T>
T	Mean( const QList<T>& data, const T& divisor = 0 )
{
	T rv = 0;
	T divBy = divisor == 0 ? data.size() : divisor;

	if( divBy != 0 ) {
		rv = Sum<T>( data ) / divBy;
	}
	return( rv );
}
template<typename T>
T	Mean( const QVector<T>& data, const T& divisor = 0 )
{
	T rv = 0;
	T divBy = divisor == 0 ? data.size() : divisor;

	if( divBy != 0 ) {
		rv = Sum<T>( data ) / divBy;
	}
	return( rv );
}
template<typename T>
T	Median( const QList<T>& data )
{
	T   rv = 0;
	int size = data.size();
	int midAt;

	if( size == 0 ) {
		return( rv );
	}
	if( size % 2 ) {
		midAt = size / 2;
	} else {
		midAt = ( size + 1 ) / 2;
	}
	rv = data.at( midAt );
	return( rv );
}
template<typename T>
T	VarianceB( const QList<T>& data )
{
	int	n = 0;
	T	s = 0;
	T	ss = 0;

	foreach( T t, data ) {
		s += t;
		ss += ( t * t );
		++n;
	}
	if( n > 0 ) {
		s /= n;
		ss /= n;
	}
	return( ss - ( s * s ) );
}
template<typename T>
T	Variance( const QList<T>& data )
{
	T	sum, mean;
	double	cnt = 0;
	sum = mean = 0;
	foreach( T t, data ) {
		sum += t;
		++cnt;
	}
	if( cnt == 0 ) {
		return( 0 );
	}
	mean = sum / cnt;
	sum = 0;
	foreach( T t, data ) {
		sum += ( t - mean ) * ( t - mean );
	}
	return( sum / ( cnt - 1 ) );
}
template<typename T>
T	SD( const QList<T>& data )
{
	return( sqrt( Variance<T>( data ) ) );
}
}	//	GH namespace
#endif	//	GH_MATH_H
