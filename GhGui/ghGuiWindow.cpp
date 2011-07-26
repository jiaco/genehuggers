#include "ghGuiWindow.h"

namespace	GH
{
	GuiWindow::GuiWindow( ObjApp *app )
	: QMainWindow()
	, GuiBase( app )
{
	useLastGeometry();
}
void	GuiWindow::useLastGeometry()
{
	if( _settings->contains( _GEOM ) ) {
		QMainWindow::restoreGeometry( _settings->value( _GEOM ).toByteArray() );
	}
	if( _settings->contains( _STATE ) ) {
		QMainWindow::restoreState( _settings->value( _STATE ).toByteArray() );
	}
}
void	GuiWindow::closeEvent( QCloseEvent* event )
{
	_settings->setValue( _GEOM, QMainWindow::saveGeometry() );
	_settings->setValue( _STATE, QMainWindow::saveState() );
	QMainWindow::closeEvent( event );
}
	GuiWindow::~GuiWindow()
{
	//delete _settings;
}
/*
QVariant	GuiWindow::getSetting( const QString& name )
{
	// return an inValid QVariant if there is no setting for name
	//
	QVariant	rv = QVariant();
	if( _settings->contains( name ) ) {
		rv = _settings->value( name );
	}
	return( rv );
}
void	GuiWindow::updateSetting( const QString& name, const QVariant& value )
{
	_settings->setValue( name, value );
}
void	GuiWindow::showMessage( const QString& text, const int& type )
{
	QMessageBox::Icon icon = static_cast<QMessageBox::Icon>( type );
	QMessageBox	msg;

	msg.setIcon( icon );
	msg.setText( text );
	msg.exec();
}
*/
}	//	GH namespace
