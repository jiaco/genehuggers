#include "ghFontView.h"

namespace	GH
{
	FontView::FontView( ParamModel *model, QWidget *parent )
	 : ToolButtonView( model, parent )
{
	connect( this, SIGNAL( defaultAction() ),
	 this, SLOT( doBrowse() ) );
}
void	FontView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	ToolButtonView::addToGrid( layout, row, col,
	 rowSpan, colSpan, alignment );
}
void	FontView::doBrowse()
{
	bool ok;

	QFont font = QFontDialog::getFont( &ok, FONT( _model->value() ),
		_parent, "Pick a Font"  );

	setValue( QVariant( font ) );
}
}	//	GH namespace
