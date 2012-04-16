#include "ghVFold.h"

namespace	GH
{
	Fold::Fold()
{
	paramsUpdated = false;
}
void	Fold::init()
{
	paramsUpdated = true;
}
float	Fold::minFold( const QString& seq, QString& structure )
{
	float	rv;

	char	*s1 = new char[ seq.size() + 1 ];
	char	*s2 = new char[ seq.size() + 1 ];

	strcpy( s1, seq.toAscii() );
	rv = fold( s1, s2 );
	structure = QString( s2 );
	delete s1;
	delete s2;
	return( rv );
}
}	//	GH namespace
