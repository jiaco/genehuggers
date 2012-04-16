#include "ghMirParam.h"
namespace	GH
{
	MirParam::MirParam()
{
	max_loop = MirParam::MAX_LOOP;
	max_size = MirParam::MAX_SIZE;
	max_dg = MirParam::MAX_DG;
	min_qpp = MirParam::MIN_QPP;
	no_gu = MirParam::NO_GU;
	seeded = MirParam::SEEDED;
	complete = MirParam::COMPLETE;
	inputPath = USTR;
}
void	MirParam::parseArgs( QMap<QString,QString>& args )
{
	if( args.contains( "e" ) ) {
		max_dg = -1.0 * args[ "e" ].toFloat();
	}
	if( args.contains( "max_loop" ) ) {
		max_loop = args[ "max_loop" ].toInt();
	}
	if( args.contains( "max_size" ) ) {
		max_size = args[ "max_size" ].toInt();
	}
	if( args.contains( "min_qpp" ) ) {
		min_qpp = args[ "min_qpp" ].toInt();
	}
	if( args.contains( "no_gu" ) ) {
		no_gu = true;
	}
	if( args.contains( "-noseed" ) ) {
		seeded = false;
	}
	if( args.contains( "-complete" ) ) {
		complete = true;
	}
	
}
void	MirParam::checkArgs( QMap<QString,QString>& args )
{
	if( args.contains( "e" ) ) {
		max_dg = -1.0 * args[ "e" ].toFloat();
	}
	if( args.contains( "-noseed" ) ) {
		seeded = false;
	}
}
}	//	GH namespace
