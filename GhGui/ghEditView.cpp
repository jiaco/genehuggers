#include "ghEditView.h"

namespace	GH
{
	EditView::EditView( ParamModel *model, QWidget *parent )
	: ParamView( model, parent )
{
	_checkBoxName = nameObject( "editCheckBox" );
	_lineEditName = nameObject( "editLineEdit" );
}
QCheckBox*	EditView::newCheckBox()
{
	QCheckBox	*checkBox = new QCheckBox( _parent );
	checkBox->setObjectName( _checkBoxName );
	connect( checkBox, SIGNAL( toggled(bool) ),
	 this, SLOT( setChecked(bool) ) );
	connect( checkBox, SIGNAL( toggled(bool) ),
	 this, SIGNAL( checked(bool) ) );

	checkBox->setVisible( isCheckable() );
	return( checkBox );
}
QLabel*	EditView::newLabel()
{
	QLabel	*label = new QLabel( _model->displayName(), _parent );
	return( label );
}
QLineEdit*	EditView::newLineEdit()
{
	QLineEdit *edit = new QLineEdit( V2S( _model->value() ), _parent );

	edit->setObjectName( _lineEditName );
	connect( edit, SIGNAL( textEdited(QString) ),
	 this, SLOT( textEdited(QString) ) );

	return( edit );
}
void	EditView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan, Qt::Alignment alignment )
{
	Q_UNUSED( rowSpan );
	Q_UNUSED( alignment );
	int	useCol = col;

	layout->addWidget( newCheckBox(), row, useCol++ );
	layout->addWidget( newLabel(), row, useCol++ );
	if( colSpan != 1 ) {
		layout->addWidget( newLineEdit(), row, useCol++, 1, colSpan );
	} else {
		layout->addWidget( newLineEdit(), row, useCol++ );
	}
	updateEnabled();
}
	// addCheckBox
	// addLabel
		// addToolButton (used by File/Color/Font)
	// addEdit
/*
	QCheckBox	*checkBox = new QCheckBox( _parent );
	checkBox->setObjectName( _checkBoxName );
	connect( checkBox, SIGNAL( toggled(bool) ),
	 this, SLOT( setChecked(bool) ) );
	connect( checkBox, SIGNAL( toggled(bool) ),
	 this, SIGNAL( checked(bool) ) );
	
	QLabel *label = new QLabel( _model->displayName(), _parent );
	QLineEdit *edit = new QLineEdit( V2S( _model->value() ), _parent );

	edit->setObjectName( _widgetName );
	connect( edit, SIGNAL( textEdited(QString) ),
	 this, SLOT( textEdited(QString) ) );

	layout->addWidget( checkBox, row, useCol++ );
	layout->addWidget( label, row, useCol++ );
	if( colSpan != 1 ) {
		layout->addWidget( edit, row, useCol, 1, colSpan );
	} else {
		layout->addWidget( edit, row, useCol );
	}
	checkBox->setVisible( isCheckable() );
	updateEnabled();
*/
void	EditView::updateChecked()
{
	SetChecked<QCheckBox>( _parent, _checkBoxName, _model->isChecked() );
}
void	EditView::updateEnabled()
{
	SetEnabled<QLineEdit>( _parent, _lineEditName, _model->isEnabled() );
}
void	EditView::textEdited( const QString& value )
{
	// when the GUI wants to be changed, ParamView::setValue( QVariant )
	// is the method to call. It will do 3 things:
	//	1) emit( changed( value ) )
	//		--->>> connected to ParamModel::guiSetValue
	//	2) call virtual updateValue()
	//		subclasses must update widgets
	//	3) emit( setSetting(QString,QVariant) )
	//		--->>> connected to MainWindow

	setValue( V( value ) );
}
void	EditView::updateValue()
{
	QList<QLineEdit *>	mytab =
		 GetChildren<QLineEdit>( _parent, _lineEditName );

	for( int i = 0; i < mytab.size(); ++i ) {
		if( !mytab[ i ]->hasFocus() ) {
			mytab[ i ]->setText( V2S( _model->value() ) );
		}
	}
}
}	//	GH namespace
