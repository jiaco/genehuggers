#ifndef	GH_APP_H
#define	GH_APP_H	1
#include <QtCore>
#include "ghDef.h"
#include "ghVariant.h"
#include "ghParam.h"
#include "ghOfp.h"
#include "ghErrorprone.h"

namespace	GH
{
inline  QMap<QString,QString>   Args( const int argc, char** argv )
{
        QMap<QString,QString>   rv;

        for( int ac = 1; ac < argc; ++ac ) {
                QString aname = argv[ ac ];
                QString avalue = "true";
                if( ac + 1 < argc ) {
                        avalue = argv[ ac + 1 ];
                        if( avalue.startsWith( '-' ) ) {
                                avalue = "true";
                        } else {
                                ++ac;
                        }
                }
                if( aname.startsWith( '-' ) ) {
                        rv.insert( aname.mid( 1 ), avalue );
                }
        }
        return( rv );
}
class	GH_DLL_EXPORT	App	:	public GH::Errorprone
{
public:
	App( int argc, char** argv );

	void	usage();
	QString	arg0() const;
	bool	isGui() const;
	void	showError() const;
	const QMap<QString,QString> args() const;

	bool	hasParam( const QString& name ) const;
	bool	addParam( Param param );
	bool	addParam( const QString& name,
		 const QString& defaultValue = QString(),
		 const Param::ParamType& type = Param::UndefParam,
		 const QString& displayName = QString() );


	Param	param( const QString& name );
	Param*	pparam( const QString& name );
	QVariant	paramValue( const QString& name ) const;
	QStringList	paramNames() const;

	bool	checkArgs();

// in ObjApp there are 2 signals
//	void	paramChanged()
//	void	message()

// in ObjApp these become slots
	void	setParam( const QString& name, const QVariant& value,
		 const bool& silent = true );
	void	guiSetParam( const QString& name, const QVariant& value );

protected:
	bool			_isGui;
	QString			_arg0;
	QMap<QString,QString>	_args;
	QMap<QString,Param>	_param;
	QStringList		_paramNames;
	QString			USAGE;
};
}	//	GH namespace
#endif	//	GH_APP_H
