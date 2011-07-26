#include "ghHeatMap.h"

namespace	GH
{
	ColorMap::ColorMap()
{
	
}
void	ColorMap::clear()
{
	_step.clear();
	_color.clear();
}
void	ColorMap::addStep( const double& val, const QColor color )
{
	_step << val;
	_color << color;
}
QColor	ColorMap::get( const double& val )
{
	for( int i = 0; i < _step.size(); ++i ) {
		if( val < _step.at( i ) ) {
			return( _color.at( i ) );
		}
	}
	return( _color.at( _color.size() - 1 ) );
}
	HeatMap::HeatMap( QWidget* parent, ColorMap* cmap )
	: QWidget( parent, Qt::Window )
{
	_cmap = cmap;
	_step = 0;

	_i0 = 1;

	init();
}
	HeatMap::HeatMap( QWidget* parent, const int& step )
	: QWidget( parent, Qt::Window )
{
	_cmap = 0;
	_step = step;

	_i0 = 1;

	init();
}
void	HeatMap::setVerticalHeader( const QStringList& hdr )
{
	_vhdr = hdr;
	_i0 = 0;
}
void	HeatMap::init()
{
	_font = QFont( "Helvetica", 12 );
	// _space = 20;
	_space = 0;
	// _startX = 20, _startY = 20; // Will set rectangle items starting pos
	// _height = 20, _width = 20; // Will set rectangle items size

	_scene = new QGraphicsScene( this );
	_viewGrid = new QGraphicsView( _scene, this );
	_viewRows = new QGraphicsView( _scene, this );
	_viewCols = new QGraphicsView( _scene, this );
	
	QGridLayout *layout = new QGridLayout;
	layout->addWidget( _viewCols, 0, 1 );
	layout->addWidget( _viewRows, 1, 0 );
	layout->addWidget( _viewGrid, 1, 1 );
	layout->addWidget( _viewGrid->verticalScrollBar(), 1, 2 );
	layout->addWidget( _viewGrid->horizontalScrollBar(), 2, 1 );
	layout->setColumnStretch(1, 10);
	layout->setRowStretch(1, 10);
	
	// layout->addWidget( _viewCols, 0, 1, 1, 1 );
	// layout->addWidget( _viewRows, 1, 0, 1, 1 );
	// layout->addWidget( _viewGrid, 1, 1, 1, 1 );
	// layout->addWidget( _viewGrid->verticalScrollBar(), 1, 2, 1, 1 );
	// layout->addWidget( _viewGrid->horizontalScrollBar(), 2, 1, 1, 1 );
	
	QWidget	*centralWidget = new QWidget;
	centralWidget->setLayout( layout );
	
	QVBoxLayout	*myLayout = new QVBoxLayout;
	myLayout->addWidget( centralWidget );
	setLayout( myLayout );
	
	_viewGrid->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	_viewGrid->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	_viewRows->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	_viewRows->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	_viewCols->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	_viewCols->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

	connect( _viewRows->verticalScrollBar(), SIGNAL( valueChanged(int) ),
	 _viewGrid->verticalScrollBar(), SLOT( setValue(int) ) );
	connect( _viewGrid->verticalScrollBar(), SIGNAL( valueChanged(int) ),
	 _viewRows->verticalScrollBar(), SLOT( setValue(int) ) );

	connect( _viewCols->horizontalScrollBar(), SIGNAL( valueChanged(int) ),
	 _viewGrid->horizontalScrollBar(), SLOT( setValue(int) ) );
	connect( _viewGrid->horizontalScrollBar(), SIGNAL( valueChanged(int) ),
	 _viewCols->horizontalScrollBar(), SLOT( setValue(int) ) );

}
	HeatMap::~HeatMap()
{
	delete	_viewGrid;
	delete	_viewRows;
	delete	_viewCols;
	delete	_scene;
}
void	HeatMap::Display( const QStringList& hdr, const QList<Row>& data,
	 const QString& title, QWidget* parent, ColorMap* cmap )
{
	HeatMap	*hm = new HeatMap( parent, cmap );
	hm->setData( hdr, data );
	hm->setWindowTitle( title );
	hm->show();

	hm->update();
	hm->setGeometry( 50, 50, 600, 600 );
	//hm->updateViewSize();
}
void	HeatMap::Display( const QStringList& hdr,
	 const QStringList& vhdr,  const QList<Row>& data,
	 const QString& title, QWidget* parent, ColorMap* cmap )
{
	HeatMap	*hm = new HeatMap( parent, cmap );
	hm->setVerticalHeader( vhdr );
	hm->setData( hdr, data );
	hm->setWindowTitle( title );
	hm->show();

	hm->update();
	hm->setGeometry( 50, 50, 600, 600 );
	//hm->updateViewSize();
}
void	HeatMap::setData( const QStringList& hdr, const QList<Row>& data )
{
	_hdr = hdr;
	_data = data;
	drawHeaders();
	drawGrid();

	_gridWidth = (_hdr.size() - _i0 ) * _boxWidth;
	_gridHeight = _data.size() * _boxWidth;

	_scene->setSceneRect( 0, 0, _topX + _gridWidth + _space, _topY + _gridHeight + _space);

	_viewGrid->setSceneRect( _topX + _space, _topY + _space, _gridWidth, _gridHeight );
	// _viewGrid->setFixedWidth( _gridWidth );
	// _viewGrid->setFixedHeight( _gridHeight );

	_viewRows->setSceneRect( 0, _topY, _topX + _space, _gridHeight );
	_viewRows->setFixedWidth( _topX + _space );
	// _viewRows->setFixedHeight( _gridHeight );
	//_viewRows->setFixedWidth( _topX + _viewRows->verticalScrollBar()->width() );
	//_viewRows->setFixedHeight( _viewGrid->height() - _viewGrid->horizontalScrollBar()->height() );

	_viewCols->setSceneRect( _topX, 0, _gridWidth, _topY + _space );
	_viewCols->setFixedHeight( _topY  + _space );
	// _viewCols->setFixedWidth( _gridWidth );
	//_viewCols->setFixedHeight( _topY + _viewCols->horizontalScrollBar()->height() );
	//_viewCols->setFixedWidth( _viewGrid->width() - _viewGrid->verticalScrollBar()->width() );

}
void	HeatMap::updateViewSize()
{
	_viewRows->setFixedWidth( _topX + _viewRows->verticalScrollBar()->width() );
	_viewRows->setFixedHeight( _viewGrid->height() - _viewGrid->horizontalScrollBar()->height() );
	_viewCols->setFixedHeight( _topY + _viewCols->horizontalScrollBar()->height() );
	_viewCols->setFixedHeight( _topY + _viewCols->horizontalScrollBar()->height() );

	_viewCols->setFixedWidth( _viewGrid->width() - _viewGrid->verticalScrollBar()->width() );
}
/*
 * 	PRIVATE METHODS
 */

void	HeatMap::drawHeaders()
{
	QFontMetrics	fm( _font, this );
	_topX = 0;
	_topY = 0;

	int	swidth = 0;
	//int	sheight = 0;
	// height gave 16 and lineSpacing only 15...?

	_boxWidth = fm.height();

	for( int i = _i0; i < _hdr.size(); ++i ) {
		QString s = _hdr.at( i );
		QGraphicsTextItem *it = _scene->addText( s, _font );
		
		// swidth = fm.width( s );
		swidth = it->document()->size().width();
		//qDebug() << fm.width( s ) << it->document()->size().width();
		if( swidth > _topY ) {
			_topY = swidth;
		}
		
		_horHeader << it;
	}
	if( _vhdr.size() == 0 ) {
		for( int i = 0; i < _data.size(); ++i ) {
			QString s = S( _data.at( i )[ _hdr.at( 0 ) ] );
			QGraphicsTextItem *it = _scene->addText( s, _font );
			
			// swidth = fm.width( s );
			swidth = it->document()->size().width();
			//qDebug() << fm.width( s ) << it->document()->size().width();
			if( swidth > _topX ) {
				_topX = swidth;
			}
			
			_verHeader << it;
		}
	} else {
		for( int i = 0; i < _vhdr.size(); ++i ) {
			QString s = _vhdr.at( i );
			QGraphicsTextItem *it = _scene->addText( s, _font );
			
			// swidth = fm.width( s );
			swidth = it->document()->size().width();
			//qDebug() << fm.width( s ) << it->document()->size().width();
			if( swidth > _topX ) {
				_topX = swidth;
			}
			
			_verHeader << it;
		}
	}
		
	for( int i = 0; i < _horHeader.size(); ++i ) {
		_horHeader[ i ]->setPos( _topX + i * _boxWidth, _topY );
		_horHeader[ i ]->setRotation( -90 );
	}
	for( int i = 0; i < _verHeader.size(); ++i ) {
		_verHeader[ i ]->setPos( 0, _topY + i * _boxWidth + _space );
	}
}
/*
	QGraphicsTextItem*	item;

	_horHeader.clear();
	QStringList hdr = _data.at(0).hdr();
	for ( int i = 1; i < hdr.size(); i++ ) {
		item = new QGraphicsTextItem( hdr.at(i) );
		item->setRotation( -90 );
		_scene->addItem( item );
		_horHeader << item;
	}
}
void	HeatMap::setVerticalHeader()
{
	QGraphicsTextItem*	item;

	_verHeader.clear();
	foreach ( Row row, _data ) {
		QString h = S( row[_data.at(0).hdr().at(0)] );
		item = new QGraphicsTextItem( h );
		_scene->addItem( item );
		_verHeader << item;
	}
}
*/
/* GRID
 */
void	HeatMap::drawGrid()
{
	int	gx, gy;
	double	val;
	bool ok;
	QColor	blackColor, color;

	blackColor = QColor( 0, 0, 0 );
	if( _cmap == 0 ) {
		//updateGradientColor();
		defaultColorMap();
	}
	gy = _topY + _space;
	for( int i = 0; i < _data.size(); ++i ) {
		gx = _topX + _space;
		for( int j = _i0; j < _hdr.size(); ++j ) {
			val = _data.at( i )[ _hdr.at( j ) ].toDouble( &ok );
			if( !ok ) {
				val = 0;
				color = blackColor;
			} else {
				color = _cmap->get( val );
				//color = getColor( val );
			}
			//val = D( _data.at( i )[ _hdr.at( j ) ] );
			_scene->addRect( gx, gy, _boxWidth, _boxWidth, QPen(), color );
			gx += _boxWidth;
		} 
		gy += _boxWidth;
	}
}
/*
void	HeatMap::setContent()
{
	int x, y;

	y = _startY;
	foreach( Row row, _data ) {
		x = _startX;
		for( int i = 1; i < row.hdr().size(); i++ ) {
			double val = D( row[row.hdr().at(i)] );
			_scene->addRect( x, y,
			 _width, _height, QPen(), getColor(val) );
			x += _width;
		}
		y += _height;
	}
}
*/
void	HeatMap::updateGradientColor()
{
	_gradient.clear();
	bool	ok = false;
	double	val = 0;
	
	for( int i = 0; i < _data.size(); ++i ) {
		for( int j = _i0; j < _hdr.size(); ++j ) {
			val = _data.at( i )[ _hdr.at( j ) ].toDouble( &ok );
			if( !ok ) {
				val = 0;
			}
			if( !_gradient.contains( val ) ) {
				_gradient << val;
			}
		}
	}

/*

	foreach ( Row row, _data ) {
		for ( int i = 1; i < hdr.size(); i++ ) {
			bool ok = false;
			double val = row[ hdr.at( i ) ].toDouble( &ok );
			if( !ok ) {
				val = 0;
			} else {
qDebug() << hdr.at( i ) << " " << row[ hdr.at( i ) ] << " gives " << val;
			}
			if( !_gradient.contains( val ) ) {
				_gradient << val;
			}
*
			double val = D(row[hdr.at(i)]);
			if ( !_gradient.contains(val) ) {
				_gradient << val;
			}
*
		}
	}
*/
	qSort( _gradient );
}

QColor	HeatMap::getColor( const double& val )
{
	int r, g, b; // 0 to 255
	
	// For a single gradient, say white -> red
	r = 255;
	b = g = 255 - ( 255.0 / double(_gradient.size()) )
	 * _gradient.indexOf(val);

	if( b < 0 ) {
		return( QColor( 0, 0, 0 ) );
	}
	
	return QColor(r, g, b);
}
void	HeatMap::defaultColorMap()
{
	QList<double>	temp;
	bool	ok;
	double	val;
	int	nPerStep;

	for( int i = 0; i < _data.size(); ++i ) {
		for( int j = _i0; j < _hdr.size(); ++j ) {
			val = _data.at( i )[ _hdr.at( j ) ].toDouble( &ok );
			if( ok && !temp.contains( val ) ) {
				temp << val;
			}
		}
	}
	qSort( temp.begin(), temp.end() );
	{
		double	top = temp.size();

		top /= ( _step * 1.0 );

		nPerStep = top;
	}
	
	if( _cmap != 0 ) {
		delete _cmap;
		_cmap = new ColorMap();
	}
	_cmap->clear();
	for( int i = 0; i < _step; ++i ) {
		int	r = 255;
		int 	b = 0;
		int	g = 0;
		b = g = 255 * ( ( i + 1 ) / _step );
		_cmap->addStep( temp.at( i + nPerStep ), QColor( r, g, b ) );
	}
}

// 
// 1- Set _startX/Y that define where to start the actual heatmap (ie. rectangles)
// 2- Set _width/_height that define the width/height of the rectangles
// 3- Once _width and _height are calculated, reset headers position
// 4- Set scene rectangle that houses headers + content
/*
void	HeatMap::updateDimensions()
{
	int h, w;
	_startX = 20, _startY = 20;
	_height = 20, _width = 20; // Default rectangle size
	
	// Calculate _startX/Y, _width and _height
	foreach ( QGraphicsTextItem* item, _horHeader ) {
		h = item->document()->size().height();
		w = item->document()->size().width();
		if ( h > _width ) {
			_width = h;
		}
		if ( w > _startY ) {
			_startY = w;
		}
	}
	foreach ( QGraphicsTextItem* item, _verHeader ) {
		h = item->document()->size().height();
		w = item->document()->size().width();
		if ( h > _height ) {
			_height = h;
		}
		if ( w > _startX ) {
			_startX = w;
		}
	}
	
	// Set headers position now that _width and _height are known
	for ( int i = 0; i < _verHeader.size(); i++ ) {
		_verHeader[i]->setPos( 0, _startY + i * _height );
	}

	for ( int i = 0; i < _horHeader.size(); i++ ) {
		_horHeader[i]->setPos( _startX + i * _width, _startY );
	}
}
*/	
}	//	GH namespace
