#include "ghBed.h"

namespace	GH
{

	SubBed::SubBed( const QString& starts, const QString& ends )
{
	parse( starts, ends );
}
void	SubBed::parse( const QString& starts, const QString& ends )
{
	beg = Deblob( starts );
	end = Deblob( ends );
/*
	QList<int> btab = Deblob( starts );
	QList<int> ltab = Deblob( length );

	beg.clear();
	end.clear();

	for( int i = 0; i < btab.size(); ++i ) {
		beg << btab.at( i );
		end << btab.at( i ) + ltab.at( i );
	}
*/
}

bool    BedSort( const Bed& e1, const Bed& e2 )
{
        if( e1.beg != e2.beg ) {
                return( e1.beg < e2.beg );
        } else {
                return( e1.end < e2.end );
        }
}
bool	BedSetSort( const BedSet& e1, const BedSet& e2 )
{
	return( NumericChr( e1.sid ) < NumericChr( e2.sid ) );
}

        Bed::Bed()
        : SLoc()
{
        init();
}
void    Bed::init()
{
        SLoc::init();
        label = "";
}
void	Bed::showSgr( QTextStream& fp ) const
{
	fp << sid << "\t"
	 << beg << "\t"
	 << val << endl;
}
void	Bed::showWig( QTextStream& fp ) const
{
	fp << beg << "\t" << val << endl;
}
void	Bed::showBed( QTextStream& fp ) const
{
	fp << sid << "\t"	
	 << beg << "\t"
	 << end << "\t"
	 << label << "\t"
	 << val << "\t" 
	 << Strand( aux ) << endl;
}
QMap<QString,int>       Bed::defaultHeader()
{
        QMap<QString,int>       rv;

        rv.insert( "sid", 0 );
        rv.insert( "beg", 1 );
        rv.insert( "end", 2 );
        rv.insert( "label", UINT );
        rv.insert( "val", UINT );
        rv.insert( "aux", UINT );
        return( rv );
}
bool    Bed::isValidHeader( const QStringList& hdr, QMap<QString,int>& idx )
{
        int     i;
	QStringList	sidTypes, begTypes, endTypes,
			auxTypes, labelTypes, valTypes;

	QStringList	optional;


	sidTypes << "sid" << "chrom";
	begTypes << "beg" << "start" << "chromStart" << "txStart";
	endTypes << "end" << "stop" << "chromEnd" << "txEnd";
	auxTypes << "strand" << "aux";
	labelTypes << "#geneName" << "geneName" << "name" << "label";
	valTypes << "val" << "score";

	optional << "exonStarts" << "exonEnds" << "cdsStart" << "cdsEnd";

        idx.clear();

	foreach( QString s, sidTypes ) {
		if( ( i = hdr.indexOf( s ) ) != -1 ) {
			idx.insert( "sid", i );
			break;
		}
	}
	if( !idx.contains( "sid" ) ) {
		return( false );
	}
	foreach( QString s, begTypes ) {
		if( ( i = hdr.indexOf( s ) ) != -1 ) {
			idx.insert( "beg", i );
			break;
		}
	}
	if( !idx.contains( "beg" ) ) {
		return( false );
	}
	foreach( QString s, endTypes ) {
		if( ( i = hdr.indexOf( s ) ) != -1 ) {
			idx.insert( "end", i );
			break;
		}
	}
	if( !idx.contains( "end" ) ) {
		return( false );
	}
	foreach( QString s, auxTypes ) {
		if( ( i = hdr.indexOf( s ) ) != -1 ) {
			idx.insert( "aux", i );
			break;
		}
	}
	foreach( QString s, labelTypes ) {
		if( ( i = hdr.indexOf( s ) ) != -1 ) {
			idx.insert( "label", i );
			break;
		}
	}
	foreach( QString s, valTypes ) {
		if( ( i = hdr.indexOf( s ) ) != -1 ) {
			idx.insert( "val", i );
			break;
		}
	}
	foreach( QString s, optional ) {
		if( ( i = hdr.indexOf( s ) ) != -1 ) {
			idx.insert( s, i );
		}
	}
	return( true );
/*
        if( ( i = hdr.indexOf( "sid" ) ) == -1 ) {
		if( ( i = hdr.indexOf( "chrom" ) ) == -1 ) {
                	return( false );
		}
        }
        idx.insert( "sid", i );
        if( ( i = hdr.indexOf( "beg" ) ) == -1 ) {
                if( ( i = hdr.indexOf( "start" ) ) == -1 ) {
                        if( ( i = hdr.indexOf( "chromStart" ) ) == -1 ) {
                                return( false );
                        }
                }
        }
        idx.insert( "beg", i );
        if( ( i = hdr.indexOf( "end" ) ) == -1 ) {
                if( ( i = hdr.indexOf( "chromEnd" ) ) == -1 ) {
                        return( false );
                }
        }
        idx.insert( "end", i );

        if( ( i = hdr.indexOf( "name" ) ) == -1 ) {
                idx.insert( "label", i );
        } else if( ( i = hdr.indexOf( "label" ) ) == -1 ) {
                idx.insert( "label", i );
        }
        if( ( i = hdr.indexOf( "val" ) ) == -1 ) {
                idx.insert( "val", i );
        } else if( ( i = hdr.indexOf( "score" ) ) == -1 ) {
                idx.insert( "val", i );
        }
        if( ( i = hdr.indexOf( "strand" ) ) == -1 ) {
                idx.insert( "aux", i );
        } else if( ( i = hdr.indexOf( "aux" ) ) == -1 ) {
                idx.insert( "aux", i );
        }
        return( true );
*/
}

        BedSet::BedSet()
	: DacBase()
{
        init();
}
void    BedSet::init()
{
        sid = "";
	DacBase::init();
        data.clear();
}
bool	BedSet::load( const QString& path )
{
        Ifp		fp;
        QString		line;
        QStringList	tok;

	bool	rv = false;

        Bed		bed;
        TrackInfo	trackInfo;

        QString fileSuffix = FileSuffix( path ).toLower();
        trackInfo.setDefaultType( "bed" );

        sid = "";

	init();
        if( !fp.open( path ) ) {
		setError( fp.error() );
                return( false );
        }
        if( fileSuffix == "txt" || fileSuffix == "ghd" ) {
                line = fp.next();
                tok = line.split( "\t" );
		QMap<QString,int>	hidx;
		if( !Bed::isValidHeader( tok, hidx ) ) {
			//
			// two options here, there is bed data
			// or this file should not attempt to be read...
			// have to guess using the first line
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
			// here we used a header, so get the next line (data)
			line = fp.next();
		}

                do {
                        tok = line.split( "\t" );
                        bed.sid = tok[ hidx[ "sid" ] ];
                        bed.beg = tok[ hidx[ "beg" ] ].toInt();
                        bed.end = tok[ hidx[ "end" ] ].toInt();
                        if( hidx[ "label" ] != UINT ) {
                                bed.label = tok[ hidx[ "label" ] ];
                        }
                        if( hidx[ "val" ] != UINT ) {
                                bed.val = tok[ hidx[ "val" ] ].toDouble();
                        }
                        if( hidx[ "aux" ] != UINT ) {
                                if( tok[ hidx[ "aux" ] ] == "0" ) {
                                        bed.aux = 0;
                                } else if( tok[ hidx[ "aux" ] ] == "1" ) {
                                        bed.aux = 1;
                                } else {
                                        bed.aux =
                                         tok[ hidx[ "aux" ] ] == "+" ? 0 : 1;
                                }
                        }
                        if( sid.isEmpty() ) {
                                sid = bed.sid;
                        }
                        if( bed.sid == sid ) {
                                data << bed;
                        }
                } while( nextLine( fp, line ) );
        } else if( fileSuffix == "bed" ) {
                while( nextLine( fp, line ) ) {
                        if( line.startsWith( "browser" ) ) {
                                continue;
                        }
                        if( line.startsWith( "track" ) ) {
                                trackInfo.init( line );
                                continue;
                        }
                        bed = Bed_parser( line );
                        if( sid.isEmpty() ) {
                                sid = bed.sid;
                        }
                        if( bed.sid == sid ) {
                                data << bed;
                        }
                }
        } else if( fileSuffix == "gff" || fileSuffix == "gtf" ) {
		setCritical( "Bed still has no gff/gtf reader" );
		goto RETURN_RV;
        }
	rv = true;
RETURN_RV:;
        fp.close();
        qSort( data.begin(), data.end(), BedSort );
        return( rv );
}
bool	BedSet::load()
{
	QVariant	v = getParam( "InputFile" );
	if( !v.isValid() ) {
		setCritical(
		 "BedSet::load() called without embedded 'InputFile'" );
		return( false );
	}
	return( load( S( v ) ) );
}
void	BedSet::showSgr( QTextStream& fp ) const
{
	for( int i = 0; i < data.size(); ++i ) {
		data.at( i ).showSgr( fp );
	}
}
void	BedSet::showWig( QTextStream& fp ) const
{
	for( int i = 0; i < data.size(); ++i ) {
		data.at( i ).showWig( fp );
	}
}
void	BedSet::showBed( QTextStream& fp ) const
{
	for( int i = 0; i < data.size(); ++i ) {
		data.at( i ).showBed( fp );
	}
}
void	BedSet::write() const
{
        // params is part of the DacBase and
        // it is assumed that there is a WriteFormat, WritePath, WriteMode
        //
        const QString   PARAM_FORMAT = "BedWriteFormat";
        const QString   PARAM_PATH = "BedWritePath";
        const QString   PARAM_MODE = "BedWriteMode";

        QString format = "bed";
        QString path = "";
        QString mode = "";

        Ofp     fp;

        if( _params.contains( PARAM_FORMAT ) ) {
                format = _params[ PARAM_FORMAT ].toString();
        }
        if( _params.contains( PARAM_PATH ) ) {
                path = _params[ PARAM_PATH ].toString();
        }
        if( _params.contains( PARAM_MODE ) ) {
                mode = STR( _params[ PARAM_MODE ] );
        }
        if( path.isEmpty() ) {
                return;
        }
        if( mode == "append" ) {
                fp.append( path );
        } else {
                fp.open( path );
        }
        if( format == "wig" ) {
                showWig( fp );
        } else if( format == "sgr" ) {
                showSgr( fp );
        } else {
                showBed( fp );
        }
        fp.close();
        return;
}
        //
        //      BED READERS
        //
        //      BED6: sid bed end name score strand
        //      BED5: sid bed end name score
        //      BED4: sid bed end name
        //      BED3: sid bed end
        //
        //      BED12: sid bed end name score strand
        //      thickStart thickEnd itemRgb blockCount blockSizes blockStarts
        //
        //      BEDPE: sid1 beg1 end1 sid2 beg2 end2 name score strand1 strand2
        //       [...] (plus any number of "user" fields)
        //

Bed     Bed_parser( const QString& line )
{
        Bed     rv;
        QStringList     tok;

        tok = line.split( "\t", QString::SkipEmptyParts );
        if( tok.size() < 3 ) {
                return( rv );
        }
        rv.sid = tok[ 0 ];
        rv.beg = tok[ 1 ].toInt();
        rv.end = tok[ 2 ].toInt();
        if( tok.size() >= 4 ) {
                rv.label = tok[ 3 ];
        }
        if( tok.size() >= 5 ) {
                rv.val = tok[ 4 ].toDouble();
        }
        if( tok.size() >= 6 ) {
                rv.aux = tok[ 5 ] == "+" ? 0 : 1;
        }
        // ignore everything else for now...
        //
        return( rv );
}
//
//      single file reader for multi-file input
//
BedSet  BedSet_reader( const QString& path )
{
        QString fileSuffix = FileSuffix( path ).toLower();

        Ifp     fp;
        QString line;
        BedSet  rv;
        Bed     bed;
        QStringList     tok;

        TrackInfo       trackInfo;
        trackInfo.setDefaultType( "bed" );

        rv.sid = "";

        if( !fp.open( path ) ) {
		rv.setError( fp.error() );
                return( rv );
        }
        if( fileSuffix == "txt" || fileSuffix == "ghd" ) {
                line = fp.next();
                tok = line.split( "\t" );
		QMap<QString,int>	hidx;
		if( !Bed::isValidHeader( tok, hidx ) ) {
			// two options here, there is bed data
			// or this file should not attempt to be read...
			// have to guess
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
			// here we are guessing the header by the first line of data
		} else {
			// here we used a header, so get the next line (data)
			line = fp.next();
		}

                do {
                        tok = line.split( "\t" );
                        bed.sid = tok[ hidx[ "sid" ] ];
                        bed.beg = tok[ hidx[ "beg" ] ].toInt();
                        bed.end = tok[ hidx[ "end" ] ].toInt();
                        if( hidx[ "label" ] != UINT ) {
                                bed.label = tok[ hidx[ "label" ] ];
                        }
                        if( hidx[ "val" ] != UINT ) {
                                bed.val = tok[ hidx[ "val" ] ].toDouble();
                        }
                        if( hidx[ "aux" ] != UINT ) {
                                if( tok[ hidx[ "aux" ] ] == "0" ) {
                                        bed.aux = 0;
                                } else if( tok[ hidx[ "aux" ] ] == "1" ) {
                                        bed.aux = 1;
                                } else {
                                        bed.aux =
                                         tok[ hidx[ "aux" ] ] == "+" ? 0 : 1;
                                }
                        }
                        if( rv.sid.isEmpty() ) {
                                rv.sid = bed.sid;
                        }
                        if( bed.sid == rv.sid ) {
                                rv.data << bed;
                        }
                } while( ( line = fp.next() ) != USTR );
        } else if( fileSuffix == "bed" ) {
                while( ( line = fp.next() ) != USTR ) {
                        if( line.startsWith( "browser" ) ) {
                                continue;
                        }
                        if( line.startsWith( "track" ) ) {
                                trackInfo.init( line );
                                continue;
                        }
                        bed = Bed_parser( line );
                        if( rv.sid.isEmpty() ) {
                                rv.sid = bed.sid;
                        }
                        if( bed.sid == rv.sid ) {
                                rv.data << bed;
                        }
                }
        } else if( fileSuffix == "gff" || fileSuffix == "gtf" ) {
		goto RETURN_RV;
        }
RETURN_RV:;
        fp.close();
        qSort( rv.data.begin(), rv.data.end(), BedSort );
        return( rv );
}
//
// single file input reader
//
BedSetList      BedSetList_reader( const QString& path )
{
        QString fileSuffix = FileSuffix( path ).toLower();
        Ifp     fp;
        QString line;
        QStringList     tok;
        TrackInfo       trackInfo;
        BedSetList      rv;

        int     idx = -1;
        Bed     bed;
        QString lastSid = USTR;

        trackInfo.setDefaultType( "bed" );

        if( !fp.open( path ) ) {
                return( rv );
        }

        if( fileSuffix == "txt" || fileSuffix == "ghd" ) {
                line = fp.next();
                tok = line.split( "\t" );
		QMap<QString,int>	hidx;
		if( !Bed::isValidHeader( tok, hidx ) ) {
			// two options here, there is bed data
			// or this file should not attempt to be read...
			// have to guess
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
			// here we are guessing the header by the first line of data
		} else {
			// here we used a header, so get the next line (data)
			line = fp.next();
		}
                do {
                        tok = line.split( "\t" );
                        bed.sid = tok[ hidx[ "sid" ] ];
                        bed.beg = tok[ hidx[ "beg" ] ].toInt();
                        bed.end = tok[ hidx[ "end" ] ].toInt();
                        if( hidx[ "label" ] != UINT ) {
                                bed.label = tok[ hidx[ "label" ] ];
                        }
                        if( hidx[ "val" ] != UINT ) {
                                bed.val = tok[ hidx[ "val" ] ].toDouble();
                        }
                        if( hidx[ "aux" ] != UINT ) {
                                if( tok[ hidx[ "aux" ] ] == "0" ) {
                                        bed.aux = 0;
                                } else if( tok[ hidx[ "aux" ] ] == "1" ) {
                                        bed.aux = 1;
                                } else {
                                        bed.aux =
                                         tok[ hidx[ "aux" ] ] == "+" ? 0 : 1;
                                }
                        }
			if( hidx.contains( "exonStarts" ) ) {
				bed.sub = new SubBed( tok[ hidx[ "exonStarts" ] ],
					 tok[ hidx[ "exonEnds" ] ] );
			}
                        if( bed.sid != lastSid ) {
                                idx = SidSetIndex<BedSet>( rv, bed.sid );
                                if( idx == -1 ) {
                                        BedSet  bs;
                                        bs.sid = bed.sid;
                                        idx = rv.size();
                                        rv << bs;
                                }
                                lastSid = bed.sid;
                        }
                        rv[ idx ].data << bed;
                } while( ( line = fp.next() ) != USTR );
        } else if( fileSuffix == "bed" ) {
                while( ( line = fp.next() ) != USTR ) {
                        if( line.startsWith( "browser" ) ) {
                                continue;
                        }
                        if( line.startsWith( "track" ) ) {
                                trackInfo.init( line );
                                continue;
                        }
                        bed = Bed_parser( line );
                        if( bed.sid != lastSid ) {
                                idx = SidSetIndex<BedSet>( rv, bed.sid );
                                if( idx == -1 ) {
                                        BedSet  bs;
                                        bs.sid = bed.sid;
                                        idx = rv.size();
                                        rv << bs;
                                }
                                lastSid = bed.sid;
                        }
                        rv[ idx ].data << bed;
                }
        } else if( fileSuffix == "gff" || fileSuffix == "gtf" ) {
		goto RETURN_RV;
        }
RETURN_RV:;
        fp.close();
        for( int i = 0; i < rv.size(); ++i ) {
                qSort( rv[ i ].data.begin(), rv[ i ].data.end(), BedSort );
        }
        return( rv );
}
BedList BedList_fromWZoneList( const WZoneList& data )
{
        BedList rv;

        for( int i = 0; i < data.size(); ++i ) {
                Bed bed;
                bed.beg = data.at( i ).beg;
                bed.end = data.at( i ).end;
                bed.aux = 0;
                bed.label = bed.sid = "";
                bed.val = data.at( i ).val;
                rv << bed;
        }
        return( rv );
}

}	//	GH namespace
