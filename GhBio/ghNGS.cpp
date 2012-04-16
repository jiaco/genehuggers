#include "ghNGS.h"

namespace	GH
{
	SffLoader::SffLoader()
{
	_suffixFast = "fasta";
	_suffixQual = "qual";
	_suffixFlow = "flow";
}
	SffLoader::~SffLoader()
{
	_fpFast.close();
	_fpQual.close();
	_fpFlow.close();
}
bool	SffLoader::setBasePath( const QString& basePath )
{
	_basePath = basePath;
	_pathFast = QString( "%1.%2" )
		.arg( _basePath ).arg( _suffixFast );
	_pathQual = QString( "%1.%2" )
		.arg( _basePath ).arg( _suffixQual );
	_pathFlow = QString( "%1.%2" )
		.arg( _basePath ).arg( _suffixFlow );

	if( !_fpFast.open( _pathFast ) ) {
		return( false );
	}
	if( !_fpQual.open( _pathQual ) ) {
		return( false );
	}
	if( !_fpFlow.open( _pathFlow ) ) {
		return( false );
	}
	return( true );
}
bool	SffLoader::next( Sff& sff )
{
	return( Fast::Next( _fpFast, sff.sid, sff.seq )
	 && Qual::Next( _fpQual, sff.sid, sff.qual )
	 && ( ( sff.endFlow = Flow::Next( _fpFlow, sff.sid, sff.flow ) ) 
	 != UINT ) );
}
}	//	GH namespace
