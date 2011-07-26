#include "ghGuiBase.h"

namespace	GH
{
	GuiBase::GuiBase( ObjApp *app )
{
	_settings = new QSettings();

	connect( app, SIGNAL( message(QString,int) ),
	 this, SLOT( showMessage(QString,int) ) );

}
	GuiBase::~GuiBase()
{
	delete _settings;
}
QVariant	GuiBase::getSetting( const QString& name )
{
	// return an inValid QVariant if there is no setting for name
	//
	QVariant	rv = QVariant();
	if( _settings->contains( name ) ) {
		rv = _settings->value( name );
	}
	return( rv );
}
void	GuiBase::updateSetting( const QString& name, const QVariant& value )
{
	_settings->setValue( name, value );
}
void	GuiBase::showMessage( const QString& text, const int& type )
{
	QMessageBox::Icon icon = static_cast<QMessageBox::Icon>( type );
	QMessageBox	msg;

	msg.setIcon( icon );
	msg.setText( text );
	msg.exec();
}
}	//	GH namespace
