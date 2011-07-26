#ifndef	GH_VARIANTMAPDIALOG_H
#define	GH_VARIANTMAPDIALOG_H	1
#include <QtGui>
#include "GhCore.h"
#include "ghGuiCommon.h"

namespace	GH
{
class	GH_DLL_EXPORT	VariantMapDialog	:	public	QDialog
{
	Q_OBJECT
public:
	VariantMapDialog( QWidget* parent, const QVariant& variantMap );

	static	QVariant	VariantMap( QWidget* parent,
				 const QVariant& variantMap );

	QVariant	value();
	QVariant	getValue();

public slots:
	void	accept();

private:
	QPushButton	*_btnDone;
	QGridLayout	*_layout;
	int		_rowNumber;
	QStringList	_mapKeys;
	QVariant	_value;
};
}	//	GH namespace
#endif	//	GH_VARIANTMAPDIALOG_H
