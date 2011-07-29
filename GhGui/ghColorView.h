#ifndef GH_COLORVIEW_H
#define	GH_COLORVIEW_H	1
#include "ghToolButtonView.h"

namespace	GH
{
class	GH_DLL_EXPORT ColorView	:	public	ToolButtonView
{
	Q_OBJECT

public:
	ColorView( ParamModel *model, QWidget *parent );

	void	addToGrid( QGridLayout *layout, const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );
	void	updateChecked() {};
	void	updateCheckable() {};

public slots:
	void	doBrowse();
	void	updateValue();
protected:
	static const ParamModel::Type _modelType = ParamModel::Color;
};
}	//	GH namespace
#endif	//	GH_COLORVIEW_H

