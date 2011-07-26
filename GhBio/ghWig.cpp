#include "ghWig.h"

namespace	GH
{

//
//	specific to legube-embl-eland format files for now
/*
	moved to ghBioStrUtils
QString SnarfSid( const QString& s )
{
	QString sid = s;
	if( sid.endsWith( ".fa" ) ) {
		sid = sid.left( sid.size() - 3 );
	}
	if( sid.startsWith( "ref_" ) ) {
		sid = sid.mid( 4 );
	}
	return( sid );
}
*/


QList<double>	WigSubsetValues( const QList<Wig>& data,
		 const int& lo, const int& hi, const int& startIndex )
{
	QList<double>	rv;

	if( hi <= lo ) {
		return( rv );
	}
	if( startIndex < 0 || startIndex >= data.size() ) {
		return( rv );
	} 
	for( int i = startIndex; i < data.size(); ++i ) {
		if( data.at( i ).beg < lo ) {
			continue;
		}
		if( data.at( i ).beg > hi ) {
			break;
		}
		rv << data.at( i ).val;
	}
	return( rv );
}

bool	WigSort( const Wig& e1, const Wig& e2 )
{
	return( e1.beg < e2.beg );
}
bool	WigSetSort( const WigSet& e1, const WigSet& e2 )
{
	return( NumericChr( e1.sid ) < NumericChr( e2.sid ) );
}
	/*
	 *	Wig class
	 */

	Wig::Wig()
{
	init();
}
void	Wig::init()
{
	beg = UINT;
	val = 0.0;
}
void	Wig::show( QTextStream& fp, const QString& sid ) const
{
	if( val == 0 ) {
		return;
	}
	if( !sid.isEmpty() ) {
		fp << sid << "\t";
	}
	fp << beg << "\t" << val << endl;
}
void	Wig::showSgr( QTextStream& fp, const QString& sid ) const
{
	if( val == 0 ) {
		return;
	}
	show( fp, sid );
}
void	Wig::showBed( QTextStream& fp, const QString& sid, const int& len,
	 const QString& bedName, const QString& bedStrand ) const
{
	if( val == 0 ) {
		return;
	}
	fp << sid << "\t"
	 << beg << "\t"
	 << beg + len << "\t"
	 << bedName << "\t"
	 << val << "\t"
	 << bedStrand << endl;
}
bool	Wig::validHeader( const QStringList& hdr, int* sid, int* beg, int* val )
{
	//
	//	static method
	//
	if( hdr.at( 0 ) != "sid" ) {
		return( false );
	}
	*sid = 0;
	if( ( *beg = hdr.indexOf( "beg" ) ) == -1 ) {
		return( false );
	}
	if( ( *val = hdr.indexOf( "val" ) ) == -1 ) {
		if( ( *val = hdr.indexOf( "score" ) ) == -1 ) {
			return( false );
		}
	}
	return( true );
}
void	Wig::parse( const QStringList& tok, QString* sid, int* step )
{
	if( sid ) {
		*sid = "";
	}
	beg = 0;
	val = 0.0;

	switch( tok.size() ) {
		case	1:
			// if this gets fixed, need to set beg from the
			// wigstep info
			val = tok[ 0 ].toDouble();
			break;
		case	2:
			beg = tok[ 0 ].toInt();
			val = tok[ 1 ].toDouble();
			break;
		case	3:
			if( sid ) {
				*sid = tok[ 0 ];
			}
			beg = tok[ 1 ].toInt();
			val = tok[ 2 ].toDouble();
			break;
		case	4:
			if( sid ) {
				*sid = tok[ 0 ];
			}
			beg = tok[ 1 ].toInt();
			if( step ) {
				*step = tok[ 2 ].toInt() - beg;
			}
			val = tok[ 3 ].toDouble();
			break;
		default:
			break;
	}
	return;
}
void	Wig::parse( const QString& line, QString* sid, int* step )
{
	return( parse( line.split( "\t" ), sid, step ) );
}

	/*
	 *	TrackInfo class (inside of WigSet)
	 */

	TrackInfo::TrackInfo( const QString& s )
{
	hdr = UCSC_TRACK_HEADER.split( "," );
	init( s );
}
void	TrackInfo::init( const QString& s )
{
	_defaultType = "wiggle_0";
	ParseNameValuePairs( s, hdr, (*this), 1 );
	if( value( "type" ).isNull() ) {
		(*this)[ "type" ] = _defaultType;
	}
}
void	TrackInfo::setDefaultType( const QString& s )
{
	_defaultType = s;
}
QString	TrackInfo::type() const
{
	return( value( "type" ).toString() );
}

	/*
	 *	WigInfo class (parent class to WigSet)
	 */

	WigInfo::WigInfo( const QString& s )
{
	init( s );
}
void	WigInfo::init( const QString& s )
{
	QMap<QString,QVariant>	data;

	_variableFlag = true;	// make default for SGR format
	start = 0;
	step = 1;
	span = 0;
	sid = "";
	if( s.startsWith( "variableStep" ) ) {
		_variableFlag = true;
	} else if( s.startsWith( "fixedStep" ) ) {
		_variableFlag = false;
	} else {
		return;
	}
	ParseNameValuePairs( s, WIG_STEP_HEADER.split( "," ), data, 1 );
	if( data.contains( "span" ) ) {
		span = data[ "span" ].toInt();
	}
	if( data.contains( "step" ) ) {
		step = data[ "step" ].toInt();
	}
	if( data.contains( "start" ) ) {
		start = data[ "start" ].toInt();
	}
	if( data.contains( "chrom" ) ) {
		sid = data[ "chrom" ].toString();
	}
}
bool	WigInfo::isVariable() const
{
	return( _variableFlag );
}
bool	WigInfo::isFixed() const
{
	return( !_variableFlag );
}
	/*
	 *	WigSet class (subclass of WigInfo and DacBase
	 */

	WigSet::WigSet( const QString& s )
	: WigInfo( s ), DacBase()
{
	init( s );
	setBedFields();
}
void	WigSet::init( const QString& s )
{
	WigInfo::init( s );
	DacBase::init();
	data.clear();
}
bool	WigSet::load( const QString& path )
{
	QString	fileSuffix = FileSuffix( path ).toLower();

	Ifp	fp;
	QString	line;
	QStringList	tok;
	QString		currentSid = "";

	Wig	w;

	bool	rv = false;

	init();
	if( !fp.open( path ) ) {
		setCritical( "WigSet::load() unable to open '" + path + "'" );
		return( false );
	}
	if( fileSuffix == "sgr" ) {
//
//	need to set WigInfo stuff
//
		while( ( line = fp.next() ) != USTR ) {
			tok = line.split( "\t" );
			w.parse( tok, &currentSid, 0 );
			if( sid.isEmpty() ) {
				sid = currentSid;
			}
			if( currentSid == sid ) {
				data << w;
			}
		}
	} else if( fileSuffix == "txt" || fileSuffix == "ghd" ) {
		line = fp.next();
		tok = line.split( "\t" );
		int	sidAt = 0;
		int	begAt = 1;
		int	valAt = 2;
		if( tok[ sidAt ] == "sid" ) {
			if( !Wig::validHeader( tok, &sidAt, &begAt, &valAt ) ) {
				setCritical( "WigSet::load() unable to "
				 "validate header of input file" );
				goto RETURN_RV;
			}
			line = fp.next();
		} else if( tok.size() != 3 ) {
			setCritical( "WigSet::load() unable to parse input" );
			goto RETURN_RV;
		}
		do {
			tok = line.split( "\t" );
			currentSid = tok[ sidAt ];
			if( sid.isEmpty() ) {
				sid = currentSid;
			}
			if( currentSid == sid ) {
				w.beg = tok[ begAt ].toInt();
				w.val = tok[ valAt ].toDouble();
				data << w;
			}
		} while( ( line = fp.next() ) != USTR );
	} else {
		while( ( line = fp.next() ) != USTR ) {
			if( line.startsWith( "track" ) ) {
				trackInfo.init( line );
				continue;
			} else if( line.startsWith( "variableStep" ) ) {
				init( line );
				continue;
			}
			if( trackInfo.type().startsWith( "wiggle" ) ) {
				if( isVariable() ) {
					w.parse( line, 0, 0 );
					data << w;
				} else {
					setCritical( "WigSet::load() "
			 " fixed step is currently an unsupported format" );
					// handle fixedStep
					goto RETURN_RV;
				}
			} else if( trackInfo.type().startsWith( "bed" ) ) {
				int step = 0;
				w.parse( line, &currentSid, &step );
				if( sid.isEmpty() ) {
					sid = currentSid;
					step = step;
				}
				if( currentSid == sid ) {
					data << w;
				}
			}
		}
	}
	rv = true;
RETURN_RV:;
	fp.close();
	return( rv );
}
bool	WigSet::load()
{
	// get the path out of DacBase
	QVariant	v = getParam( "InputFile" );
	if( !v.isValid() ) {
		setCritical(
		 "WigSet::load() called without embedded 'InputFile'" );
		return( false );
	}
	QString path = S( v );
	return( load( path ) );
}
void	WigSet::setBedFields( const QString& name, const QString& strand )
{
	bedName = name;
	bedStrand = strand;
}
int	WigSet::size() const
{
	return( data.size() );
}
void	WigSet::showWig( QTextStream& fp ) const
{
	fp << "track ";
	if( trackInfo.keys().contains( "type" ) &&
	 !trackInfo[ "type" ].toString().isEmpty() ) {
		fp << " type=" << trackInfo[ "type" ].toString();
	} else {
		fp << " type=wiggle_0";
	}
	foreach( QString key, trackInfo.keys() ) {
		if( key == "type" ) {
			continue;
		} 
		if( !trackInfo[ key ].toString().isEmpty() ) {
			fp << " " << key << "=" << trackInfo[ key ].toString();
		}
	}
	fp << endl;
	if( isVariable() ) {
		fp << "variableStep";
	} else {
		fp << "fixedStep";
	}
	fp << " chrom=" << sid;
	if( start != 0 ) {
		fp << " start=" << start;
	}
	if( step > 1 ) {
		fp << " step=" << step;
	}
	if( span != 0 ) {
		fp << " span=" << span;
	}
	fp << endl;
	for( int i = 0; i < data.size(); ++i ) {
		if( isVariable() ) {
			fp << data.at( i ).beg << "\t";
		}
		fp << data.at( i ).val << endl;
	}
}
void	WigSet::showSgr( QTextStream& fp ) const
{
	for( int i = 0; i < data.size(); ++i ) {
		data.at( i ).showSgr( fp, sid );
	}
}
void	WigSet::showBed( QTextStream& fp ) const
{
	for( int i = 0; i < data.size(); ++i ) {
		data.at( i ).showBed( fp, sid, span, bedName, bedStrand );
/*
		fp << sid << "\t"
		 << data.at( i ).beg << "\t"
		 << data.at( i ).beg + span << "\t"
		 << bedName << "\t"
		 << data.at( i ).val << "\t"
		 << bedStrand << endl;
*/
	}
}
void	WigSet::write()
{
	// params is part of the DacBase and
	// it is assumed that there is
	//	WigWriteFormat, WigWritePath, WigWriteMode
	//
	const QString	PARAM_FORMAT = "WriteFormat";
	const QString	PARAM_PATH = "WritePath";
	const QString	PARAM_MODE = "WriteMode";

	QString	format = "wig";
	QString	path = "";
	QString mode = "";

	Ofp	fp;

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
	if( format == "bed" ) {
		showBed( fp );
	} else if( format == "sgr" ) {
		showSgr( fp );
	} else {
		showWig( fp );
	}

	fp.close();
	return;
}
void	WigSet::smooth()
{
	const QString PARAM_STRING = "WigSmoothingWindowSize";
	QList<Wig>	tdata;
	Wig		t;
	int	w;

	if( !_params.contains( PARAM_STRING ) ) {
		return;
	}
	w = _params[ PARAM_STRING ].toInt();

qDebug() << "w=" << w;	

	int	endPos = data.at( data.size() - 1 ).beg;
	for( int i = 0; i < data.size(); ++i ) {
		if ( i % 1000 ) {
			qDebug() << i;
		}
		t.beg = data.at( i ).beg;
		int	lo = qMax( 0, data.at( i ).beg - ( w / 2 ) );
		int	hi = qMin( endPos, data.at( i ).beg + ( w /2 ) );
		int	cnt = 0;
		double	val = 0.0;
		for( int j = 0; j < data.size(); ++j ) {
			if( data.at( j ).beg < lo ) {
				continue;
			}
			if( data.at( j ).beg > hi ) {
				break;
			}
			val += data.at( j ).val;
			++cnt;
		}
		if( cnt > 0 ) {
			val /= cnt;
		}
		t.val = val;
		tdata << t;
	}
	data = tdata;
}
void	WigSet::fixStep()
{
	//	actual smoothing will be outputing
	//	same number of points with averaging across
	//	 a window centered on each point
	// params is part of the DacBase and
	// it is assumed that there is a WigSmoothingWindowSize
	// paramter set somewhere outside this call...
	//

	const QString PARAM_STRING = "FixStep";
	int	w;
	QList<Wig>	tdata;
	int		tsize;

	double	loBlock =0.0, hiBlock = 0.0;
	int	i = 0;
	int	cnt = 0;
	int	firstPos, lastPos;

	if( !_params.contains( PARAM_STRING ) ) {
		return;
	}
	w = _params[ PARAM_STRING ].toInt();
	
	firstPos = ( w / 2 );
	lastPos = data.at( data.size() - 1 ).beg - ( w / 2 );
	tsize = ( lastPos - firstPos ) / w;

	for( i = 0; data.at( i ).beg < (w/2); ++i ) {
		loBlock += data.at( i ).val;
		++cnt;
	}
	if( cnt > 0 ) {
		loBlock /= cnt;
	}
	for( int pos = firstPos; pos <= lastPos; pos += w ) {
		cnt = 0;
		hiBlock = 0.0;
		while( data.at( i ).beg < pos + firstPos ) {
			hiBlock += data.at( i ).val;
			++cnt;
			++i;
		}
		if( cnt > 0 ) {
			hiBlock /= cnt;
		}
		
		Wig w;
		w.beg = pos;
		w.val = ( loBlock + hiBlock ) / 2;
		tdata << w;

		loBlock = hiBlock;
	}
	data = tdata;
	return;
}

WigList		WigListSmooth( const WigList& data,
		 const int& dataSize, const int& windowSize,
		 const int& minPosition, const int& incPosition ) 
{
	WigList		rv;
	int 		lastIdx = 0;
	
        for( int i = 0; i < dataSize; ++i ) {
                int lo = minPosition + ( i * incPosition );
                int hi = lo + windowSize;
                Wig p;

                p.beg = i;
                p.val = 0;
                int      cnt = 0;
                for( int idx = lastIdx; idx < data.size(); ++idx ) {
                        if( data[ idx ].beg < lo ) {
                                lastIdx = idx;
                                continue;
                        }
                        if( data[ idx ].beg > hi ) {
                                break;
                        }
                        p.val += data[ idx ].val;
                        ++cnt;
                }
		if ( cnt > 0 ) {
               		p.val /= cnt;
		}
                rv << p;
        }
	return( rv );
}

	//
	//	re-implemented in WigLoader::loadMultiSidPerFile()
	//	 with added Error support
	//
WigSetList	WigSetList_reader( const QString& path )
{
	QString	fileSuffix = FileSuffix( path ).toLower();
	WigSetList	rv;

	Ifp		fp;
	QString		line;
	QStringList	tok;

	WigSet	ws;
	Wig	w;

	QString	sid, lastSid = USTR;

	if( !fp.open( path ) ) {
		return( rv );
	}
	if( fileSuffix == "sgr" ) {
		// handle SGR format separately
		// sid beg val no end therefore no step 
		//
		while( ( line = fp.next() ) != USTR ) {
			tok = line.split( "\t" );
			w.parse( tok, &sid, 0 );
			if( sid != lastSid ) {
				if( ws.size() > 0 ) {
					rv << ws;
				}
				ws.init();
				ws.sid = sid;
				lastSid = sid;
			}
			ws.data << w;
		}
		if( ws.size() > 0 ) {
			rv << ws;
		}
	} else if( fileSuffix == "txt" || fileSuffix == "ghd" ) {
		line = fp.next();
		tok = line.split( "\t" );
		int	sidAt = 0;
		int	begAt = 1;
		int	valAt = 2;
		if( tok[ sidAt ] == "sid" ) {
			if( !Wig::validHeader( tok, &sidAt, &begAt, &valAt ) ) {
				goto RETURN_RV;
			}
			line = fp.next();
		} else if( tok.size() != 3 ) {
			goto RETURN_RV;
		}
		 // we just assume/pray that headless input will be sid beg val
		do {
			tok = line.split( "\t" );
			sid = tok[ sidAt ];

			if( sid != lastSid ) {
				if( ws.size() > 0 ) {
					rv << ws;
				}
				ws.init();
				ws.sid = sid;
				lastSid = sid;
			}
			w.beg = tok[ begAt ].toInt();
			w.val = tok[ valAt ].toDouble();
			ws.data << w;
		} while( ( line = fp.next() ) != USTR );
		if( ws.size() > 0 ) {
			rv << ws;
		}
	} else if( fileSuffix == "eland" ) {
		QString eland_hdr =
		 "machine,run,lane,tile,x,y,index,"
		 "read,seq,quality,sid,contig,beg,strand,"
		 "cigar,score,pair-score,pair-sid,pair-contig,"
		 "pair-beg,pair-strand,filterflag";

		QMap<QString,WigSet>	tdata;

		w.val = 0;

		Token tok, hdr;
		hdr <<  eland_hdr.split( "," );
		tok.attachHeader( hdr );
		while( tok.next( fp ) ) {
		
			if( tok[ "filterflag" ] != "Y" ||
		 	 tok[ "strand" ].isEmpty() ) {
				continue;
			}
			sid = SnarfSid( tok[ "sid" ] );
			w.beg = tok[ "beg" ].toInt();
		// need to convert to counts
		//	w.val = tok[ "score" ].toDouble();

			if( tdata.contains( sid ) ) {
				tdata[ sid ].data << w;
			} else {
//qDebug() << "Create temp container for " << sid;
				WigSet t;
				t.sid = sid;
				t.data << w;
				tdata.insert( sid, t );
			}
		}
		// tdata needs to be manipulated into WigSetList (SORTED)

		foreach( QString s, tdata.keys() ) {
			qSort( tdata[ s ].data.begin(),
			 tdata[ s ].data.end(), WigSort );
//qDebug() << "Sort " << s << " " << tdata[ s ].data.size();

			rv << tdata[ s ];
		}
//qDebug() << "Loaded: " << rv.size();
		
	} else {
//
// should be wig or vstep files only here
//
		while( ( line = fp.next() ) != USTR ) {
			if( line.startsWith( "track" ) ) {
				ws.trackInfo.init( line );
				continue;
			} else if( line.startsWith( "variableStep" ) ||
			 line.startsWith( "fixedStep" ) ) {
				if( ws.size() > 0 ) {
					rv << ws;
				}
				ws.init( line );
				continue;
			}
			if( ws.trackInfo.type().startsWith( "wiggle" ) ) {
				if( ws.isVariable() ) {
					w.parse( line, 0, 0 );
					ws.data << w;
				} else {
					// handle fixedStep
					goto RETURN_RV;
				}
			} else if( ws.trackInfo.type().startsWith( "bed" ) ) {
				int step = 0;
				w.parse( line, &sid, &step );
				if( sid != ws.sid ) {
					if( ws.size() > 0 ) {
						rv << ws;
					}
					ws.init();
					ws.sid = sid;
					ws.step = step;
				}
				ws.data << w;
			}
		}
		if( ws.size() > 0 ) {
			rv << ws;
		}
	}
RETURN_RV:;
	fp.close();
	return( rv );
}
	//
	//	WigSet_reader is only going to handle 1 sid in the file
	//	              as it is called when multiple paths are specified
	//
WigSet	WigSet_reader( const QString& path )
{
	QString	fileSuffix = FileSuffix( path ).toLower();

	Ifp	fp;
	QString	line;
	QStringList	tok;
	QString		sid = "";

	//	WigSet constructor will call an empty init()
	//
	WigSet	rv;
	Wig	w;

//qDebug() << "LOADING " << path;
	if( !fp.open( path ) ) {
		return( rv );
	}
	rv.init();
	if( fileSuffix == "sgr" ) {
//qDebug() << "PATH IN SGR FORMAT";
//
//	need to set WigInfo stuff
//
		while( ( line = fp.next() ) != USTR ) {
			tok = line.split( "\t" );
			w.parse( tok, &sid, 0 );
			if( rv.sid.isEmpty() ) {
				rv.sid = sid;
			}
			if( sid == rv.sid ) {
				rv.data << w;
			}
		}
//qDebug() << "SHOULD BE DONE";
	} else if( fileSuffix == "txt" || fileSuffix == "ghd" ) {
		line = fp.next();
		tok = line.split( "\t" );
		int	sidAt = 0;
		int	begAt = 1;
		int	valAt = 2;
		if( tok[ sidAt ] == "sid" ) {
			if( !Wig::validHeader( tok, &sidAt, &begAt, &valAt ) ) {
				goto RETURN_RV;
			}
			line = fp.next();
		} else if( tok.size() != 3 ) {
			goto RETURN_RV;
		}
		do {
			tok = line.split( "\t" );
			sid = tok[ sidAt ];
			if( rv.sid.isEmpty() ) {
				rv.sid = sid;
			}
			if( sid == rv.sid ) {
				w.beg = tok[ begAt ].toInt();
				w.val = tok[ valAt ].toDouble();
				rv.data << w;
			}
		} while( ( line = fp.next() ) != USTR );
	} else {
		while( ( line = fp.next() ) != USTR ) {
			if( line.startsWith( "track" ) ) {
				rv.trackInfo.init( line );
				continue;
			} else if( line.startsWith( "variableStep" ) ) {
				rv.init( line );
				continue;
			}
			if( rv.trackInfo.type().startsWith( "wiggle" ) ) {
				if( rv.isVariable() ) {
					w.parse( line, 0, 0 );
					rv.data << w;
				} else {
					// handle fixedStep
					goto RETURN_RV;
				}
			} else if( rv.trackInfo.type().startsWith( "bed" ) ) {
				int step = 0;
				w.parse( line, &sid, &step );
				if( rv.sid.isEmpty() ) {
					rv.sid = sid;
					rv.step = step;
				}
				if( sid == rv.sid ) {
					rv.data << w;
				}
			}
		}
	}
	RETURN_RV:;
//qDebug() << "CLOSE FILE AND RETURN";
	fp.close();
	return( rv );
}

}	//	GH namespace
