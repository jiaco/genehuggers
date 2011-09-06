#include "ghView.h"

namespace	GH
{
View::View( QGraphicsScene* scene, QWidget* parent )
	: QGraphicsView( scene, parent )
{
	// need to decide if this is rightmousebutton only or not
	// this ScrollHandDrag builds in the leftmousebutton dragging
	setDragMode( QGraphicsView::ScrollHandDrag );
	scaleOp = SCALE_XY;
	moveOp = MOVE_XY;
	_handScrolling = false;
}
View::View( QWidget* parent )
	: QGraphicsView( parent )
{
}
void	View::mouseReleaseEvent( QMouseEvent* event )
{
	if( event->button() == Qt::RightButton ) {
		_handScrolling = false;
		event->accept();
		return;
	}
	QGraphicsView::mouseReleaseEvent( event );
}
void	View::mousePressEvent( QMouseEvent *event )
{
	if( event->button() == Qt::LeftButton ) {
		QList<QGraphicsItem*> itab;
		bool handled = false;
	
		itab = items( event->pos() );
		for( int i = 0; i < itab.size(); ++i ) {
			if( !itab[ i ]->data( 0 ).toString().isEmpty() ) {
				handled = true;
				emit( itemClicked( itab[ i ] ) );
			}
		}
		if( !handled ) {
			event->ignore();
			//QGraphicsView::mousePressEvent( event );
		}
	} else if( event->button() == Qt::RightButton ) {
		_lastDragPos = event->pos();
		_handScrolling = true;
		event->accept();
		return;
	} else {
		QGraphicsView::mousePressEvent( event );
	}
/*
	for( int i = 0; i < itab.size(); ++i ) {
		if( !itab[ i ]->data( 0 ).toString().isEmpty() ) {
			int	idx = itab[ i ]->data( 0 ).toInt();
			emit( clickedZone( idx ) );
// should just be able to break out here...
	QGraphicsView::mousePressEvent( event );
			return;
		}
	}
	QGraphicsView::mousePressEvent( event );
*/
}
void	View::mouseMoveEvent( QMouseEvent *event )
{
	if( dragMode() == QGraphicsView::ScrollHandDrag &&
	 moveOp == MOVE_X_ONLY && _handScrolling ) {
		QScrollBar *hBar = horizontalScrollBar();
		//QScrollBar *vBar = verticalScrollBar();
		QPoint delta = event->pos() - _lastDragPos;
		hBar->setValue( hBar->value() + 
		 ( qApp->isRightToLeft() ? delta.x() : -delta.x() ) );
		event->accept();
		return;
	//	mouseMoveEventHandler( event );
	} else {
		QGraphicsView::mouseMoveEvent( event );
	}
}
void	View::wheelEvent( QWheelEvent *event )
{
	// removed the - on -event to try to fix the wheel
	//
	scaleView( pow( (double)2, event->delta() / 240.0 ) );
}
void	View::setScaleOp( const int& op )
{
	scaleOp = op;
}
void	View::setMoveOp( const int& op )
{
	moveOp = op;
}
void	View::scaleView( qreal factor )
{
	if( factor < 0.07 || factor > 100 ) {
		return;
	}
	qreal	sx, sy;
	sx = sy = factor;
	if( scaleOp == SCALE_X_ONLY ) {
		sy = 1.0;
	}
	if( scaleOp == SCALE_Y_ONLY ) {
		sx = 1.0;
	}
	
	scale( sx, sy );
}
}	//	GH namespace
