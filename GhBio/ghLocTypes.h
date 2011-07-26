#ifndef	GH_LOCTYPES_H
#define	GH_LOCTYPES_H	1
#include "ghWig.h"

namespace	GH
{
	//      BLoc    (read block)
	//
class	GH_DLL_EXPORT	BLoc	:	public	Wig
{
public:
	BLoc();

	void    init();
	void    calcScore( const WigList& data,
		 const double& min, const double& max );

	//	data members
	//
	int     end;
	int     aux;

};

typedef	QList<BLoc>     BLocList;

	//      SLoc    BLoc with an sid
	//      SGR is sid:beg:val but makes more sense to use WigSetList
	//
class	GH_DLL_EXPORT	SLoc	:	public BLoc
{
public:
	SLoc();

	void    init();

	//	data members
	//
	QString sid;
};

typedef	QList<SLoc>	SLocList;

}	//	GH namespace
#endif	//	GH_LOCTYPES_H
