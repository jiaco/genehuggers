#ifndef GH_FONTVIEW_H
#define	GH_FONTVIEW_H	1
#include "ghToolButtonView.h"

namespace	GH
{
class	GH_DLL_EXPORT FontView	:	public	ToolButtonView
{
	Q_OBJECT

public:
	FontView( ParamModel *model, QWidget *parent );

	void	addToGrid( QGridLayout *layout, const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );
	void	updateChecked() {};
	void	updateCheckable() {};

public slots:
	void	doBrowse();
protected:
	static const ParamModel::Type _modelType = ParamModel::Font;
};
}	//	GH namespace
#endif	//	GH_FONTVIEW_H

