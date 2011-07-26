#ifndef	GH_HEATMAP_H
#define	GH_HEATMAP_H	1
#include <GhCore.h>
#include "GhGui.h"

namespace	GH
{
class	GH_DLL_EXPORT ColorMap
{
public:
	ColorMap();

	void	clear();
	void	addStep( const double& val, const QColor color );
	QColor		get( const double& val );

	QList<double>	_step;
	QList<QColor>	_color;
};
	// TODO
	//	color map assumes sorted input but should enure it too
	//		still need to add _step into HeatMap
	//
class	GH_DLL_EXPORT HeatMap : public QWidget
{
	Q_OBJECT

public:
	//HeatMap( const QList<Row>& data, QObject* parent = 0 );
	HeatMap( QWidget* parent = 0, ColorMap* cmap = 0 );
	HeatMap( QWidget* parent = 0, const int& step = 10 );

	void	setVerticalHeader( const QStringList& hdr );

	void	init();

	~HeatMap();

	void	setData( const QStringList& hdr, const QList<Row>& data );
	static void Display( const QStringList& hdr, const QList<Row>& data,
		const QString& title, QWidget* parent = 0, ColorMap* cmap = 0 );
	static void Display( const QStringList& hdr,
		const QStringList& vhdr, const QList<Row>& data,
		const QString& title, QWidget* parent = 0, ColorMap* cmap = 0 );
	
private:
	void	drawHeaders();
	void	drawGrid();
	void	updateGradientColor();
	QColor	getColor( const double& val );

	void	updateViewSize();
/*
	void	setHorizontalHeader();
	void	setVerticalHeader();
	void	setContent();
	void	updateDimensions();
*/
	
	QList<QGraphicsTextItem*>	_horHeader, _verHeader;
	
	QStringList	_hdr;
	int		_i0;	// 0 for vhdr and 1 for no-vhdr
	QStringList	_vhdr;
	QList<Row>	_data;
	QList<double>	_gradient;
	int	_startX, _startY;
	int	_width, _height;

	QGraphicsScene	*_scene;
	QGraphicsView	*_viewGrid, *_viewRows, *_viewCols;

	QFont	_font;

	qreal	_topX, _topY, _boxWidth;
	qreal	_gridWidth, _gridHeight;

	void	defaultColorMap();

	qreal	_space;
	int	_step;
	ColorMap	*_cmap;
};
}	// GH namespace
#endif	// GH_HEATMAP_H
