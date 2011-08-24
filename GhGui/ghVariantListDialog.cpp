#include "ghVariantListDialog.h"

namespace	GH
{
	VariantListDialog::VariantListDialog( QWidget* parent,
	 const QStringList& labelList,
	 const QVariant& variantList )
	: QDialog( parent )
{
	QList<QVariant>	list = variantList.toList();

	_labels = labelList;

	_layout = new QGridLayout;
	setLayout( _layout );
	_btnDone = new QPushButton( "Done", this );
	connect( _btnDone, SIGNAL( clicked() ),
	 this, SLOT( accept() ) );

	int i = 0;
	for( i = 0; i < _labels.size(); ++i ) {
		QLabel *label = new QLabel( labelList.at( i ) );
		QString	s = "undef";
		if( list.size() > i ) {
			s = S( list.at( i ) );
		}
		QLineEdit *edit = new QLineEdit( s, this );
		edit->setObjectName( labelList.at( i ) );
		label->setBuddy( edit );
		_layout->addWidget( label, i, 0 );
		_layout->addWidget( edit, i, 1 );
	}
	_layout->addWidget( _btnDone, i, 0 );
}
void		VariantListDialog::accept()
{
	_value = getValue();

	done( QDialog::Accepted );
}
QVariant	VariantListDialog::value()
{
	return( _value );
}
QVariant	VariantListDialog::getValue()
{
	_value.clear();

	QList<QVariant>	list;
	for( int i = 0; i < _labels.size(); ++i ) {
		QVariant	val;
		QLineEdit *edit = findChild<QLineEdit *>( _labels.at( i ) );
		if( edit ) {
			val = edit->text();
		}
		list << val;
	}
	_value = QVariant( list );
	
	return( _value );
}
QVariant	VariantListDialog::VariantList( QWidget* parent,
		 const QStringList& labelList,
		 const QVariant& variantList )
{
	VariantListDialog	dialog( parent, labelList, variantList );

	if( dialog.exec() == QDialog::Accepted ) {
		return( dialog.value() );
	}
	return( QVariant() );
}
}	//	GH namespace
