#include "ghImageWidget.h"

namespace	GH
{
	ImageWidget::ImageWidget( QImage* image, FileView* saveParamView,
	 QWidget* parent )
	: QWidget( parent, Qt::Window )
{
	_image = image;
	_saveParamView = saveParamView;

	_layout = new QGridLayout;

	_label = new QLabel( this );
	_label->setPixmap( QPixmap::fromImage( (*image) ) );

/*
	_btnClose = PushButton( "Close", this );
	connect( _btnClose, SIGNAL( clicked() ), this, SLOT( close() ) );
*/

	_btnSave = PushButton( "Save", this );
	connect( _btnSave, SIGNAL( clicked() ), this, SLOT( doSave() ) ); 

	_layout->addWidget( _label, 0, 0, 1, -1 );
	_saveParamView->addToGrid( _layout, 1, 0 );
	//_layout->addWidget( _btnClose, 1, 0 );
	_layout->addWidget( _btnSave, 1, _layout->columnCount() );

	setLayout( _layout );

}
void	ImageWidget::doSave()
{
	QString	path = S( _saveParamView->model()->value() );
	if( path.isEmpty() ) {
		MessageBox( "Please select a Save path first", Error::Warn );
		return;
	}
	_image->save( S( _saveParamView->model()->value() ), "PNG" );

/*
	QString	path = QFileDialog::getSaveFileName();
	_image->save( path, "PNG" );
*/
}
}	//	GH namespace
