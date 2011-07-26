#ifndef	GH_GRIDWIDGET_H
#define	GH_GRIDWIDGET_H	1
#include <QtGui>
#include "ghData.h"
#include "ghGuiCommon.h"
#include "ghTableModel.h"
#include "ghTableView.h"
#include "ghHeaderView.h"

namespace	GH
{
class	GH_DLL_EXPORT	GridWidget	:	public	QWidget
{
	Q_OBJECT
public:
	GridWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );

	void	setData( const QList<QMap<QString,QVariant> >& data );
	//void	setVerticalHeader( const QStringList& header );
	void	setData( const QStringList& header, 
		const QList<QList<QVariant> >& data );
	void	setData( const QStringList& header, 
		 const QStringList& vheader,
		const QList<QList<QVariant> >& data );
	void	setData( const QStringList& header, const QList<Row>& data );
	void	setData( const QStringList& header, const QStringList& vheader,
		  const QList<Row>& data );

	static	void	ShowTable( const QList<QMap<QString,QVariant> >& data,
			QWidget* parent = 0 );
	static	void	ShowTable( const QStringList& hdr, const QList<Row>& data,
			const QString& title = QString(), QWidget* parent = 0 );
	static	void	ShowTable( const QStringList& hdr, const QStringList& vhdr,
			const QList<Row>& data,
			const QString& title = QString(), QWidget* parent = 0 );

	TableView*	tableView();
	TableView*	tableView() const;
	QStringList	tableHeader() const;

	QVariant	dataAt( const int& row, const QString& colName ) const;
public slots:
	void	clickHandler( const QModelIndex& index );
signals:
	void	clicked( const QVariant& row, const QVariant& column );

protected:
	QStringList		_tableHeader;
	QStringList		_verticalHeader;
	QList<QList<QVariant> >	_tableData;

	TableModel	*_tableModel;
	TableView	*_tableView;
	HeaderView	*_headerView;

	QGridLayout	*_layout;
	QScrollArea	*_scrollArea;
};
}	//	GH namespace
#endif	//	GH_GRIDWIDGET_H
