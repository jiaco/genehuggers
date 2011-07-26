#ifndef	GH_STRINGLISTDIALOG_H
#define	GH_STRINGLISTDIALOG_H	1
#include <QtGui>
#include "GhCore.h"
#include "ghGuiCommon.h"
#include "ghListModel.h"

namespace	GH
{
class	GH_DLL_EXPORT StringListDialog	:	public QDialog
{
	Q_OBJECT
public:
	StringListDialog( QWidget* parent = 0,
		const QStringList& allValues = QStringList(),
		const QStringList& currentValues = QStringList() );

	void	setAllValues( const QStringList& values );
	void	setSelectedValues( const QStringList& values );

	QStringList	selectedValues() const;


	static QStringList	selectStringList( QWidget* parent,
			 const QStringList& allValues,
			 const QStringList& currentValues = QStringList() );

public slots:
	void	completerActivated( const QString& s );
	void	clearSelected();

	void	accept();
	void	reject();

protected:
	QStringList	_allValues;
	QStringList	_selectedValues;

	ListModel	*_listModel;
	QItemSelectionModel	*_selectionModel;
	QListView	*_listView;
	QPushButton	*_btnClear, *_btnCancel, *_btnDone;
	QLabel		*_edlabel;
	QLineEdit	*_editer;
	QCompleter	*_completer;
};
}	//	GH namespace
#endif	//	GH_STRINGLISTDIALOG_H
