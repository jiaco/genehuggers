#include "ghFutureDialog.h"

namespace	GH
{
	FutureDialog::FutureDialog( QWidget* parent, Qt::WindowFlags f )
	: QProgressDialog( parent, f )
{
	setWindowTitle( "Please Wait" );
	setWindowModality( Qt::NonModal );
	setLabelText( "Something is happening" );
	//setCancelButtonText( "Cancel" );
}
	FutureDialog::FutureDialog( const QString& labelText,
		const QString& cancelButtonText,
		int minimum, int maximum,
		QWidget* parent, Qt::WindowFlags f )
	: QProgressDialog( labelText, cancelButtonText, minimum, maximum, parent, f )
{
	setWindowModality( Qt::NonModal );
}
bool	FutureDialog::attach( QObject* fw )
{
	connect( fw, SIGNAL( finished() ), this, SLOT( reset() ) );
	connect( this, SIGNAL( canceled() ), fw, SLOT( cancel() ) );
	connect( fw, SIGNAL( progressRangeChanged(int,int) ),
	 this, SLOT( setRange(int,int) ) );
	connect( fw, SIGNAL( progressValueChanged(int) ),
	 this, SLOT( setValue(int) ) );
	// each connect can return false?
	return( true );
}

}	//	GH namespace
