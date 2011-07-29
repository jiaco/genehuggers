#ifndef	GH_CHOICEVIEW_H
#define	GH_CHOICEVIEW_H	1
#include "ghParamView.h"
#include "ghStringListDialog.h"

namespace	GH
{
class	GH_DLL_EXPORT ChoiceView	:	public	ParamView
{
	Q_OBJECT
public:
	ChoiceView( ParamModel *model, QWidget *parent );

	static bool	SetChoices( QObject *parent, const QString& name,
			const QStringList& choices );

	void	addToGrid( QGridLayout *layout,
		 const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );

	static bool	AddAsCombo( QObject *parent, const QString& name,
		 QGridLayout *layout, const int& row, const int& col );
	static bool	AddAsSelector( QObject *parent, const QString& name,
		 QGridLayout *layout, const int& row, const int& col );
	static bool	AddAsRadioGroup( QObject *parent, const QString& name,
		 QGridLayout *layout, const int& row, const int& col );
	void	updateValue();
	void	updateEnabled();

	void	updateChecked() {};
	void	updateCheckable() {};

public slots:
	void	setChoices( const QStringList& choices );
	void	currentIndexChanged( const QString& value );
	void	currentButtonChanged( const int& id );
	void	showSelector();
signals:
	void	activated( int index );
	void	activated( const QString& text );
protected:
	void	addAsCombo( QGridLayout *layout,
		 const int& row, const int& col );
	void	addAsSelector( QGridLayout *layout,
		 const int& row, const int& col );
	void	addAsRadioGroup( QGridLayout *layout,
		 const int& row, const int& col );

	void	updateCombo();
	void	updateSelEdit();

	QStringList	_choices;
	QString		_previousSelection;	// stored as CSV

	QString	_comboName;
	QString	_selBtnName;	// selector widget
	QString	_selEditName;	// selector widget

	static const ParamModel::Type _modelType = ParamModel::Choice;
};
}	//	GH namespace
#endif	//	GH_CHOICEVIEW_H
