#include "ghBedLoader.h"

namespace	GH
{
bool	BedLoader::load( const QVariant& param,
	 QList<BedSet>& data, const bool& isGui )
{
	QStringList	files = SL( param );

	if( files.size() <= 0 ) {
		setCritical( "BedLoader: no files to load" );
	} else if( files.at( 0 ).isEmpty() ) {
		setCritical( "BedLoader: empty file name" );
	} else if( files.size() == 1 ) {
		QFuture<BedSetList> f = RunFuture(
			QtConcurrent::run( this,
			    &BedLoader::loadMultiSidPerFile,
			    files.at( 0 ) ),
			 isGui );
		data = f.result();
	} else {		// more than 1 file
		data.clear();
		foreach( QString fname, files ) {
			BedSet	bedset;
			bedset.setParam( "InputFile", V( fname ) );
			data << bedset;
		}
		QFuture<void> f = RunFuture(
			QtConcurrent::map( data, &BedSet::load ),
			 isGui );
		for( int i = 0; i < data.size(); ++i ) {
			if( !data.at( i ).errorState() ) {
				setError( data.at( i ).error() );
				break;
			}
		}
	}
	return( errorState() ); 
}
/*
bool		BedLoader::loadOneSidPerFile( const QString& path )
{
	
}
*/
QList<BedSet>	BedLoader::loadMultiSidPerFile( const QString& path )
{
	QList<BedSet>	bedsetlist;
	Bed		bed;

	QString		sid, lastSid, fileSuffix, line;
	QStringList	tok;
	Ifp		fp;
	int		idx;

	//
	//	is this ever actually used?
	TrackInfo	trackInfo;

	clearError();
	trackInfo.setDefaultType( "bed" );

	if( !fp.open( path ) ) {
		setError( fp.error() );
		return( bedsetlist );
	}
	idx = UINT;
	lastSid = USTR;
	fileSuffix = FileSuffix( path ).toLower();
	
	if( fileSuffix == "bed" ) {

		while( nextLine( fp, line ) ) {
			if( line.startsWith( "browser" ) ) {
				continue;
			}
			if( line.startsWith( "track" ) ) {
				trackInfo.init( line );
				continue;
			}
			bed = Bed_parser( line );
			if( bed.sid != lastSid ) {
				idx = SidSetIndex<BedSet>( bedsetlist, bed.sid );
				if( idx == UINT ) {
					BedSet bedset;
					bedset.sid = bed.sid;
					idx = bedsetlist.size();
					bedsetlist << bedset;
				}
				lastSid = bed.sid;
			}
			bedsetlist[ idx ].data << bed;
		}

	} else if( fileSuffix == "txt" || fileSuffix == "ghd" ) {

		// even more error handling?
		line = fp.next();
		tok = line.split( "\t" );
		QMap<QString,int>	hidx;
		if( !Bed::isValidHeader( tok, hidx ) ) {
			//
			//	WARN or what, this is just a guess, user
			//	 can easily provide different data
			//	 that will be read, but meaningless
			//
			hidx = Bed::defaultHeader();
			if( tok.size() >= 4 ) {
				hidx[ "label" ] = 3;
			}
			if( tok.size() >= 5 ) {
				hidx[ "val" ] = 4;
			}
			if( tok.size() >= 6 ) {
				hidx[ "aux" ] = 5;
			}
		} else {
			// even more error handling?
			line = fp.next();
		}
		do {
			tok = line.split( "\t" );
			bed.sid = tok[ hidx[ "sid" ] ];
			bed.beg = I( tok[ hidx[ "beg" ] ] );
			bed.end = I( tok[ hidx[ "end" ] ] );
			if( hidx[ "label" ] != UINT ) {
				bed.label = tok[ hidx[ "label" ] ];
			}
			if( hidx[ "val" ] != UINT ) {
				bed.val = D( tok[ hidx[ "val" ] ] );
			}
			if( hidx[ "aux" ] != UINT ) {
				bed.aux = StrandToAux( tok[ hidx[ "aux" ] ] );
			}
			if( hidx.contains( "exonStarts" ) ) {
				bed.sub = new SubBed( tok[ hidx[ "exonStarts" ] ],
				 tok[ hidx[ "exonEnds" ] ] );
			}
			if( bed.sid != lastSid ) {
				idx = SidSetIndex<BedSet>( bedsetlist, bed.sid );
				if( idx == UINT ) {
					BedSet bedset;
					bedset.sid = bed.sid;
					idx = bedsetlist.size();
					bedsetlist << bedset;
				}
				lastSid = bed.sid;
			}
			bedsetlist[ idx ].data << bed;
		} while( nextLine( fp, line ) );

	} else if( fileSuffix == "gff" || fileSuffix == "gtf" ) {
		//
		//	TODO 
		//
		goto RETURN_RV;
	}

RETURN_RV:;
	// could check fp.error() and pass to self if set?
	fp.close();
	for( int i = 0; i < bedsetlist.size(); ++i ) {
		qSort( bedsetlist[ i ].data.begin(), bedsetlist[ i ].data.end(), BedSort );
	}
	return( bedsetlist );
}

}	//	GH namespace
