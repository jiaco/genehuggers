#ifndef	GH_PARAMVIEW_H
#define	GH_PARAMVIEW_H	1
#include "GhCore.h"
#include <QtGui>

namespace	GH
{
/*
	Listing of template functions:

	GetParam( parent, name, cname );
	GetChildren( parent, name );
	SetChecked( parent, name, value );
	SetEnabled( parent, name, value );
	LoseFocus( parent, name );
	ConnectToSlot( parent, name, signal, target, slot );
	ConnectToSignal( parent, name, target, signal, slot )

 */
template<class T>
T*	GetParam( QObject *parent, const QString& name,
	 const char* cname = 0 )
{
	T*	rv = 0;

	// this would be interesting..but I could not get it
	//const char *cname = rv->metaObject()->className();
	//
	if( ( rv = parent->findChild<T *>( name ) ) == 0 ) {
		return( 0 );
	}
	//if( !cname.isEmpty() && !rv->inherits( cname ) ) {
	if( cname && !rv->inherits( cname ) ) {
		return( 0 );
	}
	return( rv );
}
template<class T>
QList<T *>	GetChildren( QObject *parent, const QString& name )
{
	return( parent->findChildren<T *>( name ) );
}
template<class T>
void    SetChecked( QObject* parent, const QString& widgetName,
	 const bool& value )
{
	QList<T *>      tab = GetChildren<T>( parent, widgetName );
	for( int i = 0; i < tab.size(); ++i ) {
		tab[ i ]->setChecked( value );
	}
}
template<class T>
void    SetEnabled( QObject* parent, const QString& widgetName,
	 const bool& value )
{
	QList<T *>      tab = GetChildren<T>( parent, widgetName );
	for( int i = 0; i < tab.size(); ++i ) {
		tab[ i ]->setEnabled( value );
	}
}
template<class T>
void    LoseFocus( QObject* parent, const QString& widgetName )
{
	QList<T *>      tab = GetChildren<T>( parent, widgetName );
	for( int i = 0; i < tab.size(); ++i ) {
		tab[ i ]->clearFocus();
	}
}
template<class T>
void	ConnectToSlot( QObject *parent, const QString& name,
	 const char *signal, QObject *target, const char *slot )
{
	T* p = GetParam<T>( parent, name );
	QObject::connect( p, signal, target, slot );
}
template<class T>
void	ConnectToSignal( QObject *parent, const QString& name, QObject* target,
	 const char *signal, const char *slot )
{
	T* p = GetParam<T>( parent, name );
	QObject::connect( target, signal, p, slot );
}
	 


class	GH_DLL_EXPORT ParamView	:	public	QObject
{
	Q_OBJECT

public:
	ParamView( ParamModel* model, QWidget *parent );

	ParamModel	*model() const;

	virtual void	updateValue() = 0;
	virtual void	updateEnabled() = 0;
	virtual void	updateChecked() = 0;
	virtual void	updateCheckable() = 0;
/*
	virtual	void	addToGrid( QGridLayout *layout,
			 const int& row, const int& col ) = 0;
*/
	virtual void	addToGrid( QGridLayout *layout,
			 const int& row, const int& col,
			 const int& rowSpan = 1, const int& colSpan = 1,
			 Qt::Alignment alignment = 0 ) = 0;

	// the SLOT( updateValue() ) gets connected in ParamView subclass
	//  constructors. As long as ParamModel->setValue() is used to
	//  change the model, then the view will get updated as per
	//  updateValue(). The settings are in MainWindow::paramChanged()
	//  and this is connected to ParamModel::paramChanged()
	//  just after construction inside MainWindow::createParamView()
	//

	QString	nameObject( const QString& subName );
	bool	isEnabled() const;
	bool	isChecked() const;
	bool	isCheckable() const;
public slots:
	void	setValue( const QVariant& value );
	void	setEnabled( const bool& enabled );
	void	setChecked( const bool& tf );
	void	setCheckable( const bool& tf );
signals:
	void	changed( const QVariant& value );
	void	checked( const bool& tf );
	void	enabled( const bool& enabled );
	void	setSetting( const QString& name, const QVariant& value );
protected:
	ParamModel	*_model;
	QWidget		*_parent;
	static const char*	className();
	static const	ParamModel::Type _modelType = ParamModel::Undef;
};
}	//	GH namespace
#endif	//	GH_PARAMVIEW_H
