#ifndef GH_FONTVIEW_H
#define	GH_FONTVIEW_H	1
#include "ghEditView.h"

namespace	GH
{
class	GH_DLL_EXPORT FontView	:	public	EditView
{
	Q_OBJECT

public:
	FontView( ParamModel *model, QWidget *parent );

	void	addToGrid( QGridLayout *layout, const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );
	void	updateEnabled();
	void	updateChecked() {};
	void	updateCheckable() {};

public slots:
	void	doBrowse();
protected:
	QString	_browseBtnName;
	QAction	*_browseAction;
	QMenu	*_menu;
};
}	//	GH namespace
#endif	//	GH_FONTVIEW_H

