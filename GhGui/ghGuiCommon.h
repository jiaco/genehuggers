#ifndef	GH_GUICOMMON_H
#define	GH_GUICOMMON_H	1
#include <QtGui>

namespace	GH
{
inline	QPushButton*	PushButton( const QString& text, QWidget* parent = 0 )
{
	QPushButton	*rv = new QPushButton( text, parent );
	//rv->setFocusPolicy( Qt::NoFocus );
	rv->setFocusPolicy( Qt::ClickFocus );
	return( rv );
}
inline	QPushButton*	PushButton( const QString& text, QWidget* parent,
			 QObject* obj, const char* slot )
{
	QPushButton	*rv = new QPushButton( text, parent );
	//rv->setFocusPolicy( Qt::NoFocus );
	rv->setFocusPolicy( Qt::ClickFocus );
	QObject::connect( rv, SIGNAL( clicked() ), obj, slot );
	return( rv );
}
inline	void	OpenWith( const QString& path )
{
	QDir	d( path );

	QDesktopServices::openUrl(
	 QUrl( QString( "file://" + d.absolutePath() ) )
	);
}
}	//	GH namespace
#endif	//	GH_GUICOMMON_H
