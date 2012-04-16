#include "ghMirScan.h"
namespace	GH
{
bool	MirScan::search( const QList<Row>& query, const Seq& seq,
	 QList<Hyb>& result )
{
	foreach( Row q, query ) {
		if( param.seeded ) {
			getSeeds( STR( q[ "rpad" ] ), seq );
			foreach( int pos, seed ) {
				QString s = seq.mid( pos - param.max_size,
						param.max_size + 7 );
				duplex.fold( STR( q[ "seq" ] ), s, hyb );
				hyb.process( pos );
				if( 
			}
		}
	}
}
}	//	GH namespace
