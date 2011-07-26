#ifndef	GH_FUTUREDIALOG_H
#define	GH_FUTUREDIALOG_H	1
#include <QtGui>
#include <GhCore.h>

namespace	GH
{
class	GH_DLL_EXPORT FutureDialog	: public QProgressDialog
{
	Q_OBJECT

public:
	FutureDialog( QWidget* parent = 0, Qt::WindowFlags f = Qt::Dialog );
	FutureDialog( const QString& labelText, const QString& cancelButtonText,
			int minimum = 0, int maximum = 100, QWidget* parent = 0,
			 Qt::WindowFlags f = Qt::Dialog );

	bool	attach( QObject* fw );
};
}	//	GH namespace
#endif	//	GH_FUTUREDIALOG_H
