#include "ghParamWidget.h"

namespace	GH
{
ParamWidget::ParamWidget( QWidget* parent )
	: QWidget( parent )
{
	lay = new QFormLayout;
}
void	ParamWidget::setParams( Param& param )
{
	foreach( QString name, param.keys() ) {
		QLineEdit *le = new QLineEdit( param[ name ], this );
		lay->addRow( name, le );
	}
	setLayout( lay );
}
void	ParamWidget::refresh( Param& param )
{
	delete lay;
	lay = 0;
	lay = new QFormLayout;
	setParams( param );
}
void	ParamWidget::updateParams( Param& param )
{
	for( int i = 0; i < lay->rowCount(); ++i ) {
		QLabel	*lbl;
		QLineEdit	*le;
	lbl = ( QLabel* )lay->itemAt( i, QFormLayout::LabelRole )->widget();
	le = ( QLineEdit* )lay->itemAt( i, QFormLayout::FieldRole )->widget();

		param[ lbl->text() ] = le->text();
	}
}
}	//	GH namespace
