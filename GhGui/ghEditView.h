#ifndef	GH_EDITVIEW_H
#define	GH_EDITVIEW_H	1
#include "ghParamView.h"

namespace	GH
{
class	GH_DLL_EXPORT EditView	:	public	ParamView
{
	Q_OBJECT
public:
	EditView( ParamModel *model, QWidget *parent );

	void	addToGrid( QGridLayout *layout, const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );
	void	updateValue();
	void	updateEnabled();
	void	updateChecked();
	void	updateCheckable() {};

public slots:
	void	textEdited( const QString& value );
protected:
	QString	_widgetName, _checkBoxName;
};
}	//	GH namespace
#endif	//	GH_EDITVIEW_H
