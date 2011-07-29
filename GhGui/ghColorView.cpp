#include "ghColorView.h"

namespace	GH
{
	ColorView::ColorView( ParamModel *model, QWidget *parent )
	 : ToolButtonView( model, parent )
{
	connect( this, SIGNAL( defaultAction() ),
	 this, SLOT( doBrowse() ) );
}
void	ColorView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	ToolButtonView::addToGrid( layout, row, col,
	 rowSpan, colSpan, alignment );
	updateValue();
}
void	ColorView::doBrowse()
{
	QColor color = QColorDialog::getColor( CLR( _model->value() ),
		_parent, "Pick a Color"  );

	setValue( QVariant( color ) );
}
void	ColorView::updateValue()
{
	QList<QLineEdit *> mytab =
		GetChildren<QLineEdit>( _parent, _lineEditName );
	QPalette	palette;

	for( int i = 0; i < mytab.size(); ++i ) {
		if( i == 0 ) {
			palette = mytab[ i ]->palette();
			palette.setColor( QPalette::Text, CLR( _model->value() ) );
		}
		if( !mytab[ i ]->hasFocus() ) {
			mytab[ i ]->setText( V2S( _model->value() ) );
		}
		mytab[ i ]->setPalette( palette );
		
	}
}
}	//	GH namespace
