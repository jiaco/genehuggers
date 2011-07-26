#include "ghVariantMapDialog.h"

namespace	GH
{
	VariantMapDialog::VariantMapDialog( QWidget* parent,
	 const QVariant& variantMap )
	: QDialog( parent )
{
	QMap<QString,QVariant>	map = variantMap.toMap();
	_mapKeys = map.keys();

	_rowNumber = 0;
	_layout = new QGridLayout;


	foreach( QString key, _mapKeys ) {
		QLabel	*label = new QLabel( key );
		QLineEdit *edit = new QLineEdit( S( map[ key ] ), this );
		edit->setObjectName( key );
		label->setBuddy( edit );
		_layout->addWidget( label, _rowNumber, 0 );
		_layout->addWidget( edit, _rowNumber, 1 );
		++_rowNumber;
	}

	_btnDone = PushButton( "Done", this );
	_layout->addWidget( _btnDone, _rowNumber, 0 );
	++_rowNumber;

	connect( _btnDone, SIGNAL( clicked() ),
	 this, SLOT( accept() ) );

	setLayout( _layout );
}
void		VariantMapDialog::accept()
{
	_value = getValue();

	done( QDialog::Accepted );
}
QVariant	VariantMapDialog::value()
{
	return( _value );
}
QVariant	VariantMapDialog::getValue()
{
	_value.clear();

	QMap<QString,QVariant>	map;

	foreach( QString key, _mapKeys ) {
		QVariant	val;
		QLineEdit *edit = findChild<QLineEdit *>( key );
		if( edit ) {
			val = edit->text();
		}
		map.insert( key, val );
	}
	_value = V( map );
	return( _value );
}
QVariant	VariantMapDialog::VariantMap( QWidget* parent,
		 const QVariant& variantMap )
{
	VariantMapDialog	dialog( parent, variantMap );

	if( dialog.exec() == QDialog::Accepted ) {
		return( dialog.value() );
	}
	return( QVariant() );
	//return( variantMap );
}
}	//	GH namespace
