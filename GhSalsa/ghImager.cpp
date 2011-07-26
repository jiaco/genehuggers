#include "ghImager.h"

namespace GH
{
	Imager::Imager( const qreal& w, const qreal& h,
		 const qreal& tm, const qreal& bm,
		 const qreal& lm, const qreal& rm )
{
	width = w;
	height = h;
	tmargin = tm;
	bmargin = bm;
	lmargin = lm;
	rmargin = rm;
	xspace = width - ( lmargin + rmargin );
	yspace = height - ( tmargin + bmargin );

	sceneRect = QRectF( 0, 0, width, height );

	lineColor = QColor( "black" );
	rightColor = QColor( "lightgrey" );

	image = 0;
	painter = 0;

	//
	//	make sure we have some defaults
	//
	setGraphicFont( FONT_AXIS );
	setGraphicFont( FONT_TITLE );
}
	Imager::~Imager()
{
	if( painter ) {
		delete painter;
		painter = 0;
	}
	if( image ) {
		delete image;
		image = 0;
	}
}
void	Imager::setGraphicFont( const int& which, const QString& name,
	 const int& size, const QString& hexColor )
{
	switch( which ) {
		case	FONT_AXIS:
			axisFont.name = name;
			axisFont.hexColor = hexColor;
			axisFont.size = size;
			break;
		case	FONT_TITLE:
			titleFont.name = name;
			titleFont.hexColor = hexColor;
			titleFont.size = size;
			break;
	}
}
qreal	Imager::X( const qreal& x )
{
	qreal  rv = lmargin;
	rv += ( xspace * x );
	return( rv );
}
qreal	Imager::Y( const qreal& y )
{
	qreal rv = tmargin;
	rv += ( yspace - ( y * yspace ) );
	return( rv );
}
void	Imager::initScene( QGraphicsScene* scene )
{
	scene->clear();
	scene->setSceneRect( sceneRect );
	scene->setBackgroundBrush( Qt::white );
	scene->addRect(
	 QRectF( lmargin, tmargin, xspace, yspace ),
	 QPen( lineColor ) );
/*
	 QRectF( rmargin, tmargin, rmargin + xspace, tmargin + yspace ),
	scene->addRect(
	 QRectF( rmargin, tmargin,
	 width - ( rmargin + lmargin ), height - ( tmargin + bmargin ) ),
	 QPen( lineColor ) );
*/
}
bool	Imager::saveScene( QGraphicsScene* scene, const QString& path )
{
	if( painter ) {
		delete painter;
		painter = 0;
	}
	if( image ) {
		delete image;
		image = 0;
	}
	image = new QImage( width, height, QImage::Format_ARGB32 );
	painter = new QPainter( image );
	scene->render( painter );
	image->save( path, "PNG" );

	return( true );
}
void	Imager::colorRightSide( QGraphicsScene* scene, const QString& color )
{
	QGraphicsRectItem	*right;
	QColor	clr( color );
	QPen	pen( clr );
	QBrush	brush( clr );

/*
	right = scene->addRect(
		 QRect( xspace / 2 + rmargin, tmargin,
		 xspace / 2, height - ( tmargin + bmargin ) ),
		 pen, brush );
*/
	right = scene->addRect(
		 QRect( xspace / 2 + lmargin, tmargin,
		 xspace / 2, yspace ),
		 pen, brush );
	right->setZValue( -1 );
}
bool	Imager::plotData( QGraphicsScene* scene, const Axes& axes,
	 const WigList& data, const QString& color  )
{
	qreal	x, y, lastx, lasty;
	QColor	clr( color );
	QPen	pen( clr );
	bool	inRange = false;
	int	i = 0;

	while( !inRange && i < data.size() ) {
		x = lmargin + data[ i ].beg;
		y = tmargin + Y( axes.getY( data[ i ].val, &inRange ) );
		if( inRange ) {
			scene->addLine( x, y, x + 1, y + 1, pen );
			lastx = x;
			lasty = y;
qDebug() << "plot " << x << " " << y;
		} else {
qDebug() << "no plot " << x << " " << y;
		}
		++i;
	}
	while( i < data.size() ) {
		x = lmargin + data[ i ].beg;
		y = Y( axes.getY( data[ i ].val, &inRange ) );
		if( inRange ) {
			scene->addLine( lastx, lasty, x, y, pen );
			lastx = x;
			lasty = y;
		}
		++i;
	}
/*
	x = lmargin + data[ 0 ].beg;
	y = tmargin + Y( axes.getY( data[ 0 ].val, &inRange ) );
	scene->addLine( x, y, x + 1, y + 1, pen );
	lastx = x;
	lasty = y;
	for( int i = 1; i < data.size(); ++i ) {
		x = lmargin + data[ i ].beg;
		y = Y( axes.getY( data[ i ].val ) );
		scene->addLine( lastx, lasty, x, y, pen );
		lastx = x;
		lasty = y;
	}
*/
	return( true );
}
void	Imager::legendY( QGraphicsScene* scene, const Axes& axes,
	 const qreal& x, const qreal& xlen, const QList<qreal>& ytab )
{
	QGraphicsSimpleTextItem	*stip;
	QPen	pen( axisFont.hexColor );
	QBrush	brush( QColor( axisFont.hexColor ) );
	QFont	font( axisFont.name, axisFont.size );
	QFontMetricsF fm( font );
	qreal	xleft;


	for( int i = 0; i < ytab.size(); ++i ) {
		qreal	py = Y( axes.getY( ytab[ i ] ) );

		QString	lbl = QString( "%1" ).arg( ytab[ i ], 3, 'f', 2 );
		qreal	diff = fm.height() / 2;
		
		stip = scene->addSimpleText( lbl, font );
		xleft = lmargin - xlen;
		stip->setPos( xleft - fm.width( lbl ) - 1, py - diff );
		stip->setPen( pen );
		stip->setBrush( brush );
		scene->addLine( xleft, py, lmargin, py, pen );
	}
}
void	Imager::legendX( QGraphicsScene* scene, const Axes& axes,
	 const qreal& y, const qreal& ylen, const QList<qreal>& xtab )
{
	QGraphicsSimpleTextItem	*stip;
	QPen	pen( axisFont.hexColor );
	QBrush	brush( QColor( axisFont.hexColor ) );
	QFont	font( axisFont.name, axisFont.size );
	QFontMetricsF fm( font );

	qreal	ytop;

	for( int i = 0; i < xtab.size(); ++i ) {
		qreal	px = X( axes.getX( xtab[ i ] ) );
		QString lbl = QString( "%1" ).arg( xtab[ i ] );
		qreal	diff = fm.width( lbl ) / 2;
		stip = scene->addSimpleText( lbl, font );
		ytop = tmargin - ylen;
		stip->setPos( px - diff, ytop - ( ylen + 1 ) );
		stip->setPen( pen );
		stip->setBrush( brush );
		scene->addLine( px, ytop, px, ytop + ylen, pen );
	}
}
void	Imager::setTitle( QGraphicsScene* scene, const QString& title )
{
	QGraphicsSimpleTextItem	*stip;
	QPen	pen( titleFont.hexColor );
	QBrush	brush( QColor( titleFont.hexColor ) );
	QFont	font( titleFont.name, titleFont.size );
	QFontMetricsF fm( font );
	qreal	xpos, ypos;

	xpos = ( width / 2 ) - ( fm.width( title ) / 2 );
	ypos = 1;
	stip = scene->addSimpleText( title, font );
	stip->setPos( xpos, ypos );
	stip->setPen( pen );
	stip->setBrush( brush );
}
/*
bool	Imager::plotPVset( const QString& path, const QList<PV>& data,
	 const int& smoothWindowSize )
{
	QList<PV>	ptab;

	// assume the data is sorted by x

	setXrange( data );

	int	xincr;
	if( ( xincr = xrange / xspace ) <= 0 ) {
		xincr = 1;
	}
	SmoothPVData( data, ptab, xspace, smoothWindowSize, xmin, xincr );

	int	smoothWindowIncr = xincr;
	int	smoothWindowSize = 1000;

	int	lastIdx = 0;
	for( int i = 0; i < xspace; ++i ) {
		int lo = xmin + ( i * smoothWindowIncr );
		int hi = lo + smoothWindowSize;
		qreal	val = 0;
		int	cnt = 0;
		for( int idx = lastIdx; idx < data.size(); ++idx ) {
			if( data[ idx ].pos < lo ) {
				lastIdx = idx;
				continue;
			}
			if( data[ idx ].pos > hi ) {
				break;
			}
			val += data[ idx ].val;
			++cnt;
		}
		val /= cnt;
		Point p;
		p.x = i;
		p.y = val;
		ptab << p;
	}
	ymin = ymax = 0;
	for( int i = 0; i < ptab.size(); ++i ) {
		if( ptab[ i ].val < ymin ) {
			ymin = ptab[ i ].val;
		}
		if( ptab[ i ].val > ymax ) {
			ymax = ptab[ i ].val;
		}
	}
	setYrange( ptab );

	scene->setBackgroundBrush( Qt::white );
	scene->addRect(
	 QRectF( margin, margin,
	 width - ( 2 * margin ), height - ( 2 * margin ) ),
	 QPen( Qt::blue ) );

//	yrange = ymax - ymin;
//	yspace = height - ( 2 * margin );
	qreal	x = margin + ptab[ 0 ].pos;
	qreal	y = margin + Y( ptab[ 0 ].val );
	scene->addLine( x, y, x + 1, y + 1, QPen( Qt::red ) );
	qreal	lastx = x;
	qreal	lasty = y;
	for( int i = 1; i < ptab.size(); ++i ) {
		x = margin + ptab[ i ].pos;
		y = Y( ptab[ i ].val );
		scene->addLine( lastx, lasty, x, y, QPen( Qt::red ) );
		lastx = x;
		lasty = y;
	}
	QGraphicsRectItem *rp = scene->addRect(
	 QRectF( width/2, margin, width/2 - (margin), height - (2*margin) ),
	 QPen( Qt::yellow ),
	 QBrush( Qt::yellow ) );
	rp->setZValue( -1 );

{	// y-axis legend

	QGraphicsSimpleTextItem* ilp;
	qreal	x0 = 30;
	qreal yinc = yrange / 10;
	for( int i = 0; i < 10; ++i ) {
		qreal y0 = ymin + i * yinc;
		qreal py = Y( y0 ) - margin;
		QString	lbl = QString( "%1" ).arg( y0, 3, 'f', 2 );
		
		ilp = scene->addSimpleText( lbl, QFont( "Arial", 12 ) );
		ilp->setPos( x0, py );
		scene->addLine( margin, py, margin + 10, py, QPen( Qt::blue ) );
	}
} // end Y-axis legend
{	// x-axis legend
	QGraphicsSimpleTextItem* ilp;
	qreal xinc = xrange / 10;
	for( int i = 0; i < 10; ++i ) {
		qreal x0 = xmin + ( i * xinc );
		qreal px = X( x0 );
		QString	lbl = QString( "%1" ).arg( x0 );

		ilp = scene->addSimpleText( lbl, QFont( "Arial", 10 ) );
		ilp->setPos( px, height - margin );
		scene->addLine(
		 px, height - margin, px, height - margin * 2,
		 QPen( Qt::blue ) );
	}
} // end X-axis legend


	scene->render( painter );
	image->save( path, "PNG" );
	
	return( true );
}
*/
/*
bool	Imager::plotDomDrive(
	 Ofp& fp,
	 const QString& path, QMap<int,QList<double> >& dmap )
{
	QMap<int,double>		pmap;

	int	margins = margin * 2;

	int	xincr = 1;

	xmin = xmax = 0;
	foreach( int i, dmap.keys() ) {
		if( i < xmin ) {
			xmin = i;
		}
		if( i > xmax ) {
			xmax = i;
		}
	}
	xrange = xmax - xmin;
	xspace = width - margins;
	if( ( xincr = xrange / xspace ) <= 0 ) {
		xincr = 1;
	}
	for( int i = xmin; i < xmax; i += xincr ) {
		double	v = 0.0;
		int	cnt = 0;
		for( int j = 0; j < xincr; ++j ) {
			foreach( double d, dmap[ i ] ) {
				v += d;
				++cnt;
			}
		}
		if( cnt > 0 ) {
			v /= cnt;
			pmap.insert( i, v );
			fp << i << "\t" << v << endl;
		}
	}
	

*
	xrange = 2 * domainWindowSize;

	xspace = width - margins;
	if( ( xincr = xrange / xspace ) <= 0 ) {
		xincr = 1;
	}
	
	double	ymin, ymax;
	ymin = ymax = 0;
	for( int i = begIdx; i < endIdx;  i += xincr ) {
		int	cnt = 0;
		double	val = 0.0;
		for( int j = 0; j < xincr; ++j ) {
			foreach( double d, dmap[ i + j ] ) {
				val += d;
				++cnt;
			}
		}
		val /= cnt;
		if( val < ymin ) {
			ymin = val;
		}
		if( val > ymax ) {
			ymax = val;
		}
		pmap.insert( i, val );
		fp << i << "\t" << val << endl;
	}
	yrange = ymax - ymin;
	yspace = height - margins;

	scene->setBackgroundBrush( Qt::white );
	scene->addRect(
	 QRectF( margin, margin, width - margins, height - margins ),
	 QPen( Qt::blue ) );
	scene->render( painter );
	image->save( path, "PNG" );

	foreach( int i, pmap.keys() ) {
		qreal x, y;
		x = xspace * ( i / xrange );
		y = yspace * ( pmap[ i ] / yrange );

		x += margin;
		y += margin;
		scene->addLine( x, y, x + 1, y + 1, QPen( Qt::red ) );
	}


	for( int i = 0; i < data.size(); i += xincr ) {
		double v = PVAverage( data, i, i + xincr );

fp << data[ i ].pos << "\t" << v << endl;

		qreal x, y;
		x = xspace * ( data[ i ].pos / xrange );
		y = yspace * ( v / yrange );
		x += margin;
		y += margin;
		scene->addLine( x, y, x + 1, y + 1, QPen( Qt::red ) );
	}


	scene->render( painter );
	image->save( path, "PNG" );
	return( true );
}
*/
/*
bool	Imager::plotPVData( const QString& path, const QList<PV>& data,
	 const int& bidx, const int& eidx )
{
	int	margins = margin * 2;

	int	nProbes = eidx - bidx;

	qreal	y0;
	int	xincr = 1;

	double	val = 0.0;

	xspace = width - margins;
	if( ( xincr = nProbes / xspace ) <= 0 ) {
		xincr = 1;
	}
	xrange = data[ eidx ].pos - data[ bidx ].pos;

	scene->setBackgroundBrush( Qt::white );
	scene->addRect(
	 QRectF( margin, margin, width - margins, height - margins ),
	 QPen( Qt::blue ) );

	QGraphicsSimpleTextItem* ilp;

	ilp = scene->addSimpleText(
	 QString( "from %1 to %2" ).
		arg( bidx ).arg( eidx ),
	 QFont( "Arial", 14 ) );

	ilp->setPos( 40, 40 );

	yspace = height - margins;
	ymin = ymax = 0;
	for( int i = bidx; i < eidx; i += xincr ) {
		val = PVAverage( data, i, i + xincr );
		if( ymin == 0 || val < ymin ) {
			ymin = val;
		}
		if( ymax == 0 || val > ymax ) {
			ymax = val;
		}
	}
	yrange = ymax - ymin;
	y0 = yspace * ( ( ymax ) / yrange );

	for( int i = bidx; i < eidx; i += xincr ) {
		val = PVAverage( data, i, i + xincr );
		
		qreal	x, y, w, h;
		qint64	xv = data[ i ].pos - data[ bidx ].pos;
		double	yv = qAbs( val );
		h = yspace * ( yv / yrange );
		w = 1;
		x = xspace * ( xv / xrange );
		x += margin;
	
		if( val > 0 ) {
			y = y0 - h;
			y += margin;
			scene->addRect(
			 QRectF( x, y, w, h ),
			 QPen( Qt::red ) );
		} else {
			y = y0;
			y += margin;
			scene->addRect(
			 QRectF( x, y, w, h ),
			 QPen( Qt::green ) );
		}
	}
	// want to place 10 x labels
	// xspace / 10 gives the spacing
	// place at margin, margin + xspacing * 1 ...

	int	xspacing = xspace / 10;

	for( int i = 0; i < 10; ++i ) {
		qreal x = margin + xspacing * i;
		qreal y;
		if( ( i % 2 ) == 0 ) {
			y = height - margins;
		} else {
			y = height -  2 * margins;
		}
		int	xbase = data[ bidx ].pos + ( xrange / 10 ) * i;
		ilp = scene->addSimpleText(
	 	QString( "%1" ).arg( xbase ),
	 	QFont( "Arial", 10 ) );
	
		ilp->setPos( x, y );
	}

	scene->render( painter );
	image->save( path, "PNG" );
	return( true );
}
*/
}	// GH namespace
