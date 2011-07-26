#ifndef	MYAPP_H
#define	MYAPP_H	1
#include <GhCore.h>
#include <GhGui.h>
#include "ghParam2.h"

namespace	GH
{

class	MyApp	:	public	ObjApp
{
	Q_OBJECT

public:
	MyApp( int argc, char** argv );

	void	doit();

	void	makeBtnParams();

	bool	addParam2( Param2 param );
	QMap<QString,Param2>	_param2;
	QStringList		_param2Names;
	Param2*	pparam2( const QString& name );
	QStringList		param2Names() const;
private:
	Ofp	cout, cerr;

};

}	//	GH namespace
#endif	//	MYAPP_H
