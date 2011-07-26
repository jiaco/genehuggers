#ifndef	GH_VARIANTLISTDIALOG_H
#define	GH_VARIANTLISTDIALOG_H	1
#include <QtGui>
#include "GhCore.h"
#include "ghGuiCommon.h"

	//
	//	class created from VariantMapDialog but
	//	for things that need to retain an order 
	//	in their presentation
	//
namespace	GH
{
class	GH_DLL_EXPORT	VariantListDialog	:	public	QDialog
{
	Q_OBJECT
public:
	VariantListDialog( QWidget* parent,
	 const QStringList& labelList, const QVariant& variantList );

	static	QVariant	VariantList( QWidget* parent,
				 const QStringList& labelList,
				 const QVariant& variantList );

	QVariant	value();
	QVariant	getValue();

public slots:
	void	accept();

private:
	QPushButton	*_btnDone;
	QGridLayout	*_layout;
	int		_rowNumber;
	QStringList	_labels;
	QVariant	_value;
};
}	//	GH namespace
#endif	//	GH_VARIANTLISTDIALOG_H
