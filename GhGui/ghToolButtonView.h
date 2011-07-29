#ifndef	GH_TOOLBUTTONVIEW_H
#define	GH_TOOLBUTTONVIEW_H	1
#include "ghEditView.h"

namespace	GH
{
// base class for File/Font/Color
//
class	GH_DLL_EXPORT	ToolButtonView	:	public	EditView
{
	Q_OBJECT
public:
	ToolButtonView( ParamModel *model, QWidget *parent );

	void	addToGrid( QGridLayout *layout, const int& tow, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );
	void	addAction( QAction* action );
	static bool	AddAction( QObject* parent, const QString& name,
			 QAction* action );

	void	updateEnabled();
	void	updateChecked();
	void	updateCheckable() {};
public slots:
	//virtual void	doDefaultAction() = 0;
signals:
	void defaultAction();
protected:
	QToolButton*	newToolButton();
	QMenu		*_menu;
	QAction		*_defaultAction;

	QString	_toolButtonName;

	static const ParamModel::Type _modelType = ParamModel::ToolButton;
};
}	//	GH namespace
#endif	//	GH_TOOLBUTTONVIEW_H
