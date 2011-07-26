#ifndef	GH_PUI2_H
#define	GH_PUI2_H	1

#include <GhGui.h>
#include "ghParam2.h"
#include "ghActionParam2.h"

namespace GH
{
inline QGridLayout*	PUI2( QWidget *parent, MyApp* app, const QStringList& plist = QStringList() )
{
	QGridLayout	*rv = new QGridLayout;

	int	nRow = 0;
	QStringList	paramNames = plist;
	if( paramNames.size() == 0 ) {
		paramNames = app->param2Names();
	}
	foreach( QString pname, paramNames ) {
/*
		ActionParam	*p = new ActionParam2( parent, app->pparam2( pname ) );
		rv->addLayout( p->asLayout(), 0, 0 );
*/
/*
		switch( 
		case	Param2::ActionParam:
*/
			rv->addLayout( new ActionParam2( parent,
			 app->pparam2( pname ) ), nRow, 0 );
			++nRow;
		//	break;
	}

	return( rv );
}
}	//	GH namespace
#endif
