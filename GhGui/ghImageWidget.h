#ifndef	GH_IMAGEWIDGET_H
#define	GH_IMAGEWIDGET_H	1
#include <GhCore.h>
#include "ghGuiCommon.h"
#include "ghFileView.h"
#include "ghMainWindow.h"

namespace GH
{
class	GH_DLL_EXPORT ImageWidget	:	public	QWidget
{
	Q_OBJECT

public:
	ImageWidget( QImage* image, FileView* saveParamView,
		QWidget* parent = 0 );
public slots:
	void	doSave();
private:
	QGridLayout	*_layout;
	FileView	*_saveParamView;

	QLabel*	_label;
	QPushButton*	_btnClose;
	QPushButton*	_btnSave;
	QImage*	_image;
};
}	//	GH namespace
#endif	//	GH_IMAGEWIDGET_H
