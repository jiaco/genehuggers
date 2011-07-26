#include "ghLocTypes.h"

	//	file contains
	//		Bloc (subclass of Wig)
	//		Sloc (subclass of Bloc, parent of Bed)

namespace	GH
{

	BLoc::BLoc()
	: Wig()
{
	init();
}
void    BLoc::init()
{
	Wig::init();
	end = UINT;
	aux = 0;
}
void    BLoc::calcScore( const WigList& data,
         const double& min, const double& max )
{
        int     cnt = 0;

        val = 0;
        for( int i = 0; i < data.size(); ++i ) {
                if( data.at( i ).beg < beg ) {
                        continue;
                }
                if( data.at( i ).beg > end ) {
                        break;
                }
                if( val >= min && val <= max ) {
                        val += data.at( i ).val;
                        ++cnt;
                }
        }
        val /= cnt;
}

	SLoc::SLoc()
 	: BLoc()
{
	init();
}
void    SLoc::init()
{
	BLoc::init();
	sid = "";
}

}	//	GH namespace
