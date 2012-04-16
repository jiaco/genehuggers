#ifndef	GH_DUPLEX_H
#define	GH_DUPLEX_H	1

extern	"C"	{
#include <fold.h>
#include <fold_vars.h>
#include <duplex.h>
}
#include "GhCore.h"
#include "GhBio.h"

/*
 *	I = mir J = subject
 */

namespace	GH
{
class	Duplex
{
public:
	Duplex();
	void	init( bool setNoGU = false );
	float	fold( const QString& istr, const QString& jstr, Hyb& hyb );
private:
	bool	paramsUpdated;
/*
	float	energy;
	int	ipos, jpos;
	QString	iseq, jseq;
	QString	structure;
*/
};
}	//	GH namespace
#endif
