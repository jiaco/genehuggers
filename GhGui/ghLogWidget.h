#ifndef	GH_LOG_WIDGET_H
#define	GH_LOG_WIDGET_H	1
#include <QtGui>
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT LogWidget	:	public	QWidget
{
	Q_OBJECT

public:
	LogWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );

	void	setMinimumSize( int minw, int minh );
public slots:
	void	log( const QString& s );
	void	clear();
private:	
	QTextEdit	*_textEdit;
	QVBoxLayout	*_layout;
};
}	//	GH namespace
#endif	//	GH_LOG_WIDGET_H
