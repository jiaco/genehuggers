#ifndef	GH_VIEW_H
#define	GH_VIEW_H	1
#include <QtGui>
#include "ghDef.h"

namespace	GH
{
const	int	SCALE_XY = 0;
const	int	SCALE_X_ONLY = 1;
const	int	SCALE_Y_ONLY = 2;

const	int	MOVE_XY = 0;
const	int	MOVE_X_ONLY = 1;
const	int	MOVE_Y_ONLY = 2;

class	GH_DLL_EXPORT View	:	public QGraphicsView
{
	Q_OBJECT

public:
	View( QWidget* parent = 0 );
	View( QGraphicsScene* scene, QWidget* parent = 0 );
	void	setScaleOp( const int& op );
	void	setMoveOp( const int& op );
signals:
	//void	clickedZone( const int& i );
	void	itemClicked( QGraphicsItem* item );
protected:
	void	mousePressEvent( QMouseEvent *event );
	void	mouseReleaseEvent( QMouseEvent *event );
	void	mouseMoveEvent( QMouseEvent *event );
	void	wheelEvent( QWheelEvent *event );
	void	scaleView( qreal factor );
	int	scaleOp;
	int	moveOp;

	bool	_handScrolling;
	QPoint	_lastDragPos;
};
}	//	GH namespace
#endif	//	GH_VIEW_H
