#ifndef	GH_IMAGER_H
#define	GH_IMAGER_H	1
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <QImage>
#include <GhCore.h>
#include <GhBio.h>
#include "ghAxes.h"

namespace	GH
{
const	int	FONT_AXIS = 0;
const	int	FONT_TITLE = 1;

const	qreal	DEFAULT_TMARGIN = 40;
const	qreal	DEFAULT_BMARGIN = 10;
const	qreal	DEFAULT_LMARGIN = 20;
const	qreal	DEFAULT_RMARGIN = 10;

//class	Axes;

class	GH_DLL_EXPORT	GraphicFont
{
public:
	QString	name, hexColor;
	int	size;
};
class	GH_DLL_EXPORT Imager
{
public:
	Imager( const qreal& w, const qreal& h,
	  const qreal& tm = DEFAULT_TMARGIN, const qreal& bm = DEFAULT_BMARGIN,
	  const qreal& lm = DEFAULT_LMARGIN, const qreal& rm = DEFAULT_RMARGIN );
	~Imager();

	void	setGraphicFont( const int& which = FONT_AXIS,
		 const QString& name = "Courier", const int& size = 12,
		 const QString& hexColor = "#000000" );
	void	setTitle( QGraphicsScene* scene, const QString& title );

	qreal	xspace, yspace;

	qreal	X( const qreal& x );
	qreal	Y( const qreal& y );

	void	initScene( QGraphicsScene* scene );
	bool	plotData( QGraphicsScene* scene, const Axes& axes,
		 const WigList& data, const QString& color );
	void	colorRightSide( QGraphicsScene* scene, const QString& color );
	bool	saveScene( QGraphicsScene* scene, const QString& path );


	void	legendX( QGraphicsScene* scene, const Axes& axes,
		 const qreal& y, const qreal& ylen, const QList<qreal>& xtab );
	void	legendY( QGraphicsScene* scene, const Axes& axes,
		 const qreal& x, const qreal& xlen, const QList<qreal>& ytab );

private:
	qreal	width, height;
	qreal	tmargin, bmargin, rmargin, lmargin;

	QColor	lineColor;
	QColor	rightColor;

	QRectF	sceneRect;
	QImage	*image;
	QPainter	*painter;

	GraphicFont	axisFont, titleFont;
};
}	//	GH namespace
#endif	//	GH_IMAGER_H
