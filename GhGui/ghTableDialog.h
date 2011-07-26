#ifndef	GH_TABLEDIALOG_H
#define	GH_TABLEDIALOG_H	1
#include <QtGui>
#include "ghData.h"
#include "ghGuiCommon.h"
#include "ghTableModel.h"
#include "ghTableView.h"

namespace	GH
{
class	GH_DLL_EXPORT	TableDialog	:	public	QDialog
{
	Q_OBJECT
public:
	TableDialog( QWidget* parent = 0 );

	void	setData( const QList<QMap<QString,QVariant> >& data );
	void	setData( const QStringList& header, 
		const QList<QList<QVariant> >& data );
	void	setData( const QStringList& header, const QList<Row>& data );

	static	void	ShowTable( const QList<QMap<QString,QVariant> >& data,
			QWidget* parent = 0 );
	static	void	ShowTable( const QStringList& hdr, const QList<Row>& data,
			const QString& title = QString(), QWidget* parent = 0 );

	void	reject();
	//	save and open in excel stuff needs a FileParam
	//	or else it will always start in some lame directory

protected:
	QStringList	_tableHeader;
	QList<QList<QVariant> >	_tableData;
	TableModel	*_tableModel;
	TableView	*_tableView;

	QGridLayout	*_layout;
	QScrollArea	*_scrollArea;
	QPushButton	*_btnClose;
};
}	//	GH namespace
#endif	//	GH_TABLEDIALOG_H
