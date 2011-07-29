#ifndef	GH_BOOLEANVIEW_H
#define	GH_BOOLEANVIEW_H	1
#include "ghParamView.h"

namespace	GH
{
class	GH_DLL_EXPORT BooleanView	:	public	ParamView
{
	Q_OBJECT

public:
	BooleanView( ParamModel *model, QWidget *parent );

//	void	addToGrid( QGridLayout *layout, const int& row, const int& col );
        void    addToGrid( QGridLayout *layout, const int& row, const int& col,
                 const int& rowSpan = 1, const int& colSpan = 1, 
                 Qt::Alignment alignment = 0 );

	QAction*	action() const;
	static QAction*	Action( QObject *parent, const QString& name );

	static bool	AddAsGroupBox( QObject* parent, const QString& name,
			QGridLayout *layout, const int& row, const int& col,
			 QGridLayout *innerLayout );
	static bool	AddAsGroupBox( QObject* parent, const QString& name,
			QGridLayout *layout, const int& row, const int& col,
			 QGridLayout *innerLayout,
			 const int& rowSpan, const int& colSpan, Qt::Alignment alignment );
	static bool	AddAsCheckBox( QObject* parent, const QString& name,
			QGridLayout *layout, const int& row, const int& col );
	static bool	AddAsRadioButton( QObject* parent, const QString& name,
			QGridLayout *layout, const int& row, const int& col );


	void	updateValue();
	void	updateEnabled();
	void	updateChecked() {};
	void	updateCheckable() {};

public slots:
	void	toggle( const bool& value );
protected:
	void	addAsGroupBox( QGridLayout *layout,
		 const int& row, const int& col, QGridLayout *innerLayout );
	void	addAsGroupBox( QGridLayout *layout,
		 const int& row, const int& col, QGridLayout *innerLayout,
		const int& rowSpan, const int& colSpan, Qt::Alignment alignment );
	void	addAsCheckBox( QGridLayout *layout,
		 const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );
	void	addAsRadioButton( QGridLayout *layout,
		 const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );

	QCheckBox*	newCheckBox();
	QRadioButton*	newRadioButton();

	QString	_checkBoxName, _groupBoxName, _radioButtonName;
	QAction	*_action;

	static const ParamModel::Type _modelType = ParamModel::Boolean;
};
}	//	GH namespace
#endif	//	GH_BOOLEANVIEW_H
