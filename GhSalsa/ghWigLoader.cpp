#include "ghWigLoader.h"

namespace	GH
{
bool	WigLoader::load( const QVariant& param,
	 QList<WigSet>& data, const bool& isGui )
{
	QStringList	files = SL( param );

	if( files.size() <= 0 ) {
		setCritical( "WigLoader: no files to load" );
	} else if( files.at( 0 ).isEmpty() ) {
		setCritical( "WigLoader: empty file name" );
	} else if( files.size() == 1 ) {
		QFuture<WigSetList> f = RunFuture(
			QtConcurrent::run( this,
			    &WigLoader::loadMultiSidPerFile,
			    files.at( 0 ) ),
			 isGui );
		data = f.result();
	} else {		// more than 1 file
		data.clear();
		foreach( QString fname, files ) {
			WigSet	wigset;
			wigset.setParam( "InputFile", V( fname ) );
			data << wigset;
		}
		QFuture<void> f = RunFuture(
			QtConcurrent::map( data, &WigSet::load ),
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
bool		WigLoader::loadOneSidPerFile( const QString& path )
{
	
}
*/
QList<WigSet>	WigLoader::loadMultiSidPerFile( const QString& path )
{
	WigSetList	wigsetlist;
	WigSet		wigset;
	Wig		wig;

	QString		sid, lastSid, fileSuffix, line;
	QStringList	tok;
	Ifp		fp;

	clearError();

	if( !fp.open( path ) ) {
		setError( fp.error() );
		return( wigsetlist );
	}
	lastSid = USTR;
	fileSuffix = FileSuffix( path ).toLower();
	
	if( fileSuffix == "sgr" ) {

		while( nextLine( fp, line ) ) {
			tok = line.split( "\t" );
			wig.parse( tok, &sid, 0 );
			if( sid != lastSid ) {
				if( wigset.size() > 0 ) {
					wigsetlist << wigset;
				}
				wigset.init();
				wigset.sid = sid;
				lastSid = sid;
			}
			wigset.data << wig;
		}
		if( wigset.size() > 0 ) {
			wigsetlist << wigset;
		}


	} else if( fileSuffix == "txt" || fileSuffix == "ghd" ) {

		// even more error handling?
		line = fp.next();
		tok = line.split( "\t" );
		int sidAt = 0;
		int begAt = 1;
		int valAt = 2;

		if( tok[ sidAt ] == "sid" ) {
			if( !Wig::validHeader( tok, &sidAt, &begAt, &valAt ) ) {
				setCritical( "asd" );
				goto RETURN_RV;
			}
			line = fp.next();
		} else if( tok.size() != 3 ) {
			setCritical( "asd" );
			goto RETURN_RV;
		}
		//
		//	warn? headless input, default sid/beg/val
		//
		do {
			tok = line.split( "\t" );
			sid = tok[ sidAt ];
			if( sid != lastSid ) {
				if( wigset.size() > 0 ) {
					wigsetlist << wigset;
				}
				wigset.init();
				wigset.sid = sid;
				lastSid = sid;
			}
			wig.beg = I( tok[ begAt ] );
			wig.val = D( tok[ valAt ] );
			wigset.data << wig;
		} while( nextLine( fp, line ) );
		if( wigset.size() > 0 ) {
			wigsetlist << wigset;
		}

	} else if( fileSuffix == "eland" ) {
		//
		//	TODO (see ghWig.cpp)
		//
	} else {	// wig or vstep
		while( nextLine( fp, line ) ) {
			if( line.startsWith( "track" ) ) {
				wigset.trackInfo.init( line );
				continue;
			} else if( line.startsWith( "variableStep" ) ||
			 line.startsWith( "fixedStep" ) ) {
				if( wigset.size() > 0 ) {
					wigsetlist << wigset;
				}
				wigset.init( line );
				continue;
			}
			if( wigset.trackInfo.type().startsWith( "wiggle" ) ) {
				if( wigset.isVariable() ) {
					wig.parse( line, 0, 0 );
					wigset.data << wig;
				} else {
					setCritical( "Wig in fixed step is still unsupported" );
					goto RETURN_RV;
				}
			} else if( wigset.trackInfo.type().startsWith( "bed" )){
				int step = 0;
				wig.parse( line, &sid, &step );
				if( sid != wigset.sid ) {
					if( wigset.size() > 0 ) {
						wigsetlist << wigset;
					}
					wigset.init();
					wigset.sid = sid;
					wigset.step = step;
				}
				wigset.data << wig;
			}
		}
		if( wigset.size() > 0 ) {
			wigsetlist << wigset;
		}
	}

RETURN_RV:;
	// could check fp.error() and pass to self if set?
	fp.close();
	return( wigsetlist );
}

}	//	GH namespace
