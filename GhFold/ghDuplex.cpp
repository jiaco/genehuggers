#include "ghDuplex.h"
namespace	GH
{
	Duplex::Duplex()
{
	paramsUpdated = false;
}
void	Duplex::init( bool setNoGU )
{
	/*
	 *	noGU is a global int in the Vienna code base
	 */
	noGU = 0;
	if( setNoGU == true ) {
		noGU = 1;
	}
	update_fold_params();
	paramsUpdated = true;
}
float	Duplex::fold( const QString& istr, const QString& jstr, Hyb& hyb )
{
	/*
	 *	duplexT is from Vienna
	 */
	duplexT	mfe;

	hyb.clear();

	if( !paramsUpdated ) {
		init();
	}
	mfe = duplexfold( istr.toAscii(), jstr.toAscii() );
	
	hyb.energy = mfe.energy;
	hyb.structure = QString( mfe.structure );
	hyb.ipos = mfe.i;
	hyb.jpos = mfe.j;
	hyb.iseq = istr;
	hyb.jseq = jstr;

	free( mfe.structure );
	return( hyb.energy );
}
}	//	GH namespace
