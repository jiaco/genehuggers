#include "ghAxes.h"

namespace GH
{
	Axes::Axes()
{
	initX = initY = false;
}
void	Axes::xinit( const WigList& data )
{
	if( initX == false ) {
		xmin = data[ 0 ].beg;
		xmax = data[ data.size() - 1 ].beg;
	} else {
		if( data[ 0 ].beg < xmin ) {
			xmin = data[ 0 ].beg;
		}
		if( data[ data.size() - 1 ].beg > xmax ) {
			xmax = data[ data.size() - 1 ].beg;
		}
	}
	xrange = xmax - xmin;
	initX = true;
	return;
}
void	Axes::xinit( const int& lo, const int& hi )
{
	initX = true;
	xmin = lo;
	xmax = hi;
	xrange = xmax - xmin;
}
void	Axes::yinit( const WigList& data )
{
	if( initY == false ) {
		ymin = data[ 0 ].val;
		ymax = ymin;
	}
	for( int i = 0; i < data.size(); ++i ) {
		if( data[ i ].val < ymin ) {
			ymin = data[ i ].val;
		}
		if( data[ i ].val > ymax ) {
			ymax = data[ i ].val;
		}
	}
	yrange = ymax - ymin;
	initY = true;
	return;
}
void	Axes::ycalc( const WigList& data,
	const double& dlo, const double& dhi )
{
	initY = true;
	double	dataMin, dataMax;

	dataMin = data[ 0 ].val;
	dataMax = data[ 0 ].val;
	for( int i = 0; i < data.size(); ++i ) {
		if( data[ i ].val < dataMin ) {
			dataMin = data[ i ].val;
		}
		if( data[ i ].val > dataMax ) {
			dataMax = data[ i ].val;
		}
	}
	ymin = dataMin - dlo;
	ymax = ymin + dhi;
	if( dataMax > ymax ) {
		ymax = dataMax;
	}
	yrange = ymax - ymin;
	initY = true;
	return;
}
void	Axes::yinit( const double& lo, const double& hi )
{
	initY = true;
	ymin = lo;
	ymax = hi;
	yrange = ymax - ymin;
}
qreal	Axes::getX( const int& pos ) const
{
	return( ( pos - xmin ) / xrange );
}
qreal	Axes::getY( const double& val, bool* ok ) const
{
	qreal rv = ( val - ymin ) / yrange;
	if( ok ) {
		*ok = true;
		if( val < ymin ) {
qDebug() << "BAD " << val << " < " << ymin;
			*ok = false;
		}
	}
	return( rv );
/*
	if( ok && val < ymin ) {
		ok = false;
	}
	if( ok && rv > ymax ) {
		ok = false;
	}
	return( ( val - ymin ) / yrange );
*/
}
}	// GH namespace
