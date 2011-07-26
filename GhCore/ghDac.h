#ifndef	GH_DAC_H
#define	GH_DAC_H	1
#include "ghDef.h"
#include "ghErrorprone.h"
//
//	DAC = Divide and Conquer (QtConcurrent dependent classes)
//

namespace	GH
{
template<class T>
void SetParams( QList<T>& data, const QMap<QString,QVariant>& params )
{
	for( int i = 0; i < data.size(); ++i ) {
		data[ i ].setParams( params );
	}
}
/*
 *	This template is independent of the GUI but is dependent on class T
 *	 being a subclass of DacBase...
 *
 *	Programs will probably only call the similar function in
 *	 GhGui/ghGuiDacUtils.h
 */
template<class T>
void	WriteDacList( QList<T>& data )
{
	for( int i = 0; i < data.size(); ++i ) {
		data.at( i ).write();
	}
}


class	GH_DLL_EXPORT	DacBase	:	public Errorprone
{
public:
	DacBase();
	void	init();

	void	setParam( const QString& pname, const QVariant& pvalue );
	void	setParams( const QMap<QString,QVariant>& params );
	void	injectParams( const QMap<QString,QVariant>& params );

	bool		hasParam( const QString& pname ) const;
	QVariant	getParam( const QString& pname ) const;

	const QMap<QString,QVariant>	params() const;
protected:
	QMap<QString,QVariant>	_params;
};
}	//	GH namespace
#endif	//	GH_DAC_H

