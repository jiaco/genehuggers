#ifndef	GH_ACTIONVIEW_H
#define	GH_ACTIONVIEW_H	1
#include "ghParamView.h"
namespace	GH
{
class	GH_DLL_EXPORT ActionView	:	public	ParamView
{
	Q_OBJECT

public:
	ActionView( ParamModel *model, QWidget* parent );

//void	addToGrid( QGridLayout *layout, const int& row, const int& col );
	void	addToGrid( QGridLayout *layout, const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );


	void		addListener( QObject *obj, const char *slot );
	static bool	AddListener( QObject *parent, const QString& name,
			 QObject *obj, const char *slot );

	QAction*	action() const;
	static QAction*	Action( QObject *parent, const QString& name );

//public slots:
	void	updateValue();
	void	updateEnabled();
	void	updateChecked() {};
	void	updateCheckable() {};
	// pass Checkable and Checked straight to the QAction

protected:
	QPushButton	*newPushButton();

	QAction	*_action;
	QString	_pushButtonName;

	static	const ParamModel::Type _modelType = ParamModel::Action;
};
}	//	GH namespace
#endif	//	GH_ACTIONVIEW_H
