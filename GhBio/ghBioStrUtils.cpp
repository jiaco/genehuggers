#include "ghBioStrUtils.h"

namespace	GH
{
int     SidSplit( const QString& sid, QString* org, const int& max )
{
	int     pos;
	QString chr;
	if( ( pos = sid.indexOf( '_' ) ) != -1 ) {
		if( org ) {
			(*org) = sid.mid( 0, pos );
		}
	}
	if( ( pos = sid.indexOf( "chr" ) ) == -1 ) {
		return( -1 );
	}
	chr = sid.mid( pos + 3 );
	if( chr == "X" ) {
		return( max + 1 );
	} else if( chr == "Y" ) {
		return( max + 2 );
	} else {
		return( chr.toInt() );
	}
}
int     SidSplit( const QVariant& sid, QString* org )
{
	return( SidSplit( sid.toString(), org ) );
}
bool	SortSidByChr( const QString& e1, const QString& e2 )
{
	return( SidSplit( e1 ) < SidSplit( e2 ) );
}
QString	SnarfSid( const QString& s )
{
	QString	rv = s;

	if( rv.endsWith( ".fa" ) ) {
		rv = rv.left( rv.size() - 3 );
	}
	if( rv.startsWith( "ref_" ) ) {
		rv = rv.mid( 4 );
	}
	return( rv );
}
QString	Strand( const int& aux )
{
	switch( aux ) {
		case	0:
			return( "+" );
			break;
		case	1:
			return( "-" );
			break;
		default:
			return( "." );
			break;
	}
}
int	StrandToAux( const QString& var )
{
/*
 * not really needed, default is 0
 *
	if( var == "0" || var == "+" ) {
		return( 0 );
	}
*/
	if( var == "1" || var == "-" ) {
		return( 1 );
	}
	return( 0 );

	//
	// save as I dont do this often
	//
	// rv = var == "+" ? 0 : 1;
}

}	//	GH namespace
