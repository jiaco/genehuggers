#include "ghFast.h"

namespace	GH
{
const QString	Flow::Key = QString( "TACG" );


int	Flow::Next( Ifp& fp, QString& sid, QList<double>& data )
{
	Token	tok( Lexer::WhiteSpace );
	int	rv;

	sid.clear();
	data.clear();

	if( !tok.next( fp ) ) {
		return( false );
	}
	if( tok.size() == 1 && !tok.next( fp ) ) {
		// first line of file has # flows
		return( false );
	}
	sid = tok.at( 0 );
	rv = INT( tok.at( 1 ) );
	for( int i = 2; i < tok.size(); ++i ) {
		data << DBL( tok.at( i ) );
	}
	return( rv );
}
// Issue here, if you want to remove TA from TAACGTC, you
// have to keep 1 A to retain ACGTC
// data is the flow data
// pos is the place to start
// seq is the sequence to subtract out (if found)
// return is bool (true if found)
int	Flow::Floor( const double& val )
{
	return( floor( val + 0.5 ) );
}
QString	Flow::Call( const double& val, const int& keyIndex )
{
	int	cnt;
	QString	rv;


	if( ( cnt = Flow::Floor( val ) ) > 0 ) {
		rv = QString( cnt, Flow::Key.at( keyIndex ) );
	}
	return( rv );
}
QString	Flow::Call4( const QList<double>& data, const int& pos )
{
	QString	rv;

	for( int i = 0; i < Flow::Key.size(); ++i ) {
		rv += Flow::Call( data.at( pos + i ), i );
	}
	return( rv );
}
int	Flow::CorrectEnd( const QList<double>& data, const int& end,
const double& maxHomoP, const double& signalMin, const double& noiseMax )
{
/*
	double	maxSignal = 0.0;
	double	maxHomoP = 0.0;
	foreach( double d, data ) {
		if( d > maxHomoP ) {
			maxHomoP = d;
		}
	}
*/
	double maxSignal = maxHomoP + 0.49;

	int	deadSpot = 0;
	while( deadSpot < end ) {
		int signal = 0;
		int noise = 0;
		for( int i = 0; i < 4; ++i ) {
			double	intensity = data.at( i + deadSpot );
			if( intensity > signalMin ) {
				++signal;
				if( intensity < noiseMax ||
				 intensity > maxSignal ) {
					++noise;
				}
			}
		}
		if( noise > 0 || signal == 0 ) {
			break;
		}
		deadSpot += 4;
	}
	return( deadSpot );
}
//
//	Take a flowgram, make a sequence and a QList<int>
//	where each position in the sequence points to the
//	index in the flowgram where the base was called
QString	Flow::ReadBases( const QList<double>& data, const int& end,
		QList<int>& itab )
{
	QString	rv;
	itab.clear();
	for( int i = 0; i < end; i += 4 ) {
		for( int j = 0; j < 4; j++ ) {
			int x = ( int )( data.at( i + j ) + 0.5 );
			//int x = Flow::Floor( data.at( i + j ) );
			for( int y = 0; y < x; y++ ) {
				rv += Flow::Key.at( j );
				itab << i + j;
			}
		}
	}
	return( rv );
}
bool	Flow::RemoveIfFound( QList<double>& data, const QString& seq, const int& pos)
{
	QString	ss, rs;
	QString	fs;

	int	fidx = pos;
	int	zeroFlows = 0;
	int	extraFlows = 0;
	int	homoFlows = 0;
	int	cnt = 0;
	rs = seq;
	while( ss.size() < seq.size() ) {
		fs = Call4( data, fidx );
		if( fs.size() <= rs.size() ) {
			if( !rs.startsWith( fs ) ) {
				return( false );
			}
			// can remove 1 block
			ss += fs;	// grow ss with removed bases
			// need to make rs the remaining part of seq
			rs = seq.mid( ss.size() );
			zeroFlows += 4;
		} else {
			if( !fs.startsWith( rs ) ) {
				return( false );
			}
			// need to zero part of a block
			// here either part of seq is in next block:w
			// fs has more bases in it than rs
			// either rs is a substr 
			// or there is a homopolymer
			cnt = 0;
			for( int i = 0; i < Flow::Key.size(); ++i ) {
				cnt = Flow::Floor( data.at( fidx + i ) );
				if( cnt < 1 ) {
					continue;
				} else if( cnt == 1 ) {
					ss += Flow::Key.at( i );
					rs = seq.mid( ss.size() );
					extraFlows = ( i + 1 );
					cnt = 0;
				} else {
					// subtract 1 for each base in rs
					// cannot zero this flow
					// we know that rs starts fs
					// so the bases should match up
					while( !rs.isEmpty() ) {
						ss += Flow::Key.at( i );
						rs = seq.mid( ss.size() );
						--cnt;
					}
					homoFlows = ( i + 1 );
				}
				if( ss.size() >= seq.size() ) {
					break;
				}
			}
		}
		fidx += 4;
	}
/*
	qDebug() << "Removed " << ss << " requires " << zeroFlows;
	qDebug() << " and extra " << extraFlows << " and homo " << homoFlows;
	if( cnt > 0 ) {
		qDebug() << "Example cnt > 0 " << cnt;
	}
*/
	for( int i = 0; i < zeroFlows; ++i ) {
		data.removeAt( 0 );
	}
	if( cnt > 0 ) {
		for( int i = 0; i < homoFlows; ++i ) {
			data[ i ] = 0.0;
		}
		data[ homoFlows ] = cnt;
	} else if( extraFlows > 0 ) {
		for( int i = 0; i < extraFlows; ++i ) {
			data[ i ] = 0.0;
		}
	}
	return( true );
}
/*
int	Flow::FlowCountForLength( const QList<double>& data, const int& pos,
	 const int& len )
{
	QString ss;
	QString	key = "TACG";

	int	fidx = pos;

	while( ss.size() < len ) {
		for( int i = 0; i < key.size(); ++i ) {
			ss += Flow::Call( data.at( fidx + i ) );
		}
		
	}
}
*/
QString	Flow::Read( const QList<double>& data, const int& pos, const int& len,
	 const double& threshold )
{
	QString	rv;
	int	 i = 0;
	QString	key = "TACG";

	while( i < len ) {
		for( int j = 0; i < len && j < key.size(); ++j ) {
			if( data.at( pos + i + j ) >= threshold ) {
				int cnt = floor(data.at( pos+i+j ) + threshold);
				rv += QString( cnt, key.at( j ) );

				//rv += key.at( j );
			}
		}
		i += key.size();
	}
	return( rv );
}
void	Flow::Show( QTextStream& fp, const QList<double>& data, const int& pos )
{
	fp << QString( "%1 %2 %3 %4 %5" )
		 .arg( data.at( pos + 0 ), 5, 'f', 2 )
		 .arg( data.at( pos + 1 ), 5, 'f', 2 )
		 .arg( data.at( pos + 2 ), 5, 'f', 2 )
		 .arg( data.at( pos + 3 ), 5, 'f', 2 )
		 .arg( Flow::Call4( data, pos ) ) << endl;
}
bool	Flow::CheckAndRemove( QTextStream& fp, QList<double>& data, int& pos,
	 const QString& seq )
{
	double	thresh = 0.5;

	QString	key = "TACG";

	int	fidx = pos;
	int	sidx = 0;
	int	send = seq.size();
	
	int	blockClear = 0;
	int	extraClear = 0;
	while( sidx < send && fidx < data.size() ) {
		int	nread = 0;
		int	nseq = 0;
		for( int i = 0; i < key.size(); ++i ) {
			if( data.at( fidx + i ) >= thresh ) {
				int cnt = floor( data.at( fidx + i ) + thresh );
				++nread;
				bool gotEm = true;
				for( int j = 0; j < cnt; ++j ) {
					if( sidx < send &&
						seq.at( sidx ) == key.at( i ) ) {
						//++nseq;
						++sidx;
					} else {
						gotEm = false;
					}
				}
				if( gotEm ) {
					++nseq;
				}
/*
				++nread;
				if( sidx < send &&
				 seq.at( sidx ) == key.at( i ) ) {
					++nseq;
					//data[ fidx + i ] = -1;
					++sidx;
				}
*/
			}
		}
		if( nread == nseq ) {
			++blockClear;
			// remove 4 flows
		} else if( nseq < nread && sidx == send ) {
			extraClear = nseq;
			// zero the flows upto nseq
			// this can only work if we are done with target
		} else {
			break;
			// this is the return false condition
			// we have read in seq that did not match target
			// data comes back unmodified
		}
		fidx += 4;
	}
	if( sidx != send ) {
		fp << "Failed to find " << seq << endl;
		return( false );
	}
/*
for( int i = 0; i < ( blockClear + 1 ); ++i ) {
	Flow::Show( fp, data, pos + ( i * 4 ) );
}
fp << "BLOCKS " << blockClear << " Plus extra " << extraClear << endl;
*/
	for( int i = 0; i < blockClear; ++i ) {
		for( int j = 0; j < 4; ++j ) {
			data.removeAt( 0 );
		}
	}
	for( int i = 0; i < extraClear; ++i ) {
		data[ i ] = 0.0;
	}
	return( true );
}
/*
		t = data.at( fidx + 0 );
		a = data.at( fidx + 1 );
		c = data.at( fidx + 2 );
		g = data.at( fidx + 3 );

		if( t > thresh && seq.at( sidx ) == 'T' ) {
			data[ fidx + 0 ] = -1;
			++sidx;
		}
		if( a > thresh && seq.at( sidx ) == 'A' ) {
			data[ fidx + 1 ] = -1;
			++sidx;
		}
		
		QString ss;
		if( t > thresh ) {
			ss += 'T';
		}
		if( a > thresh ) {
			ss += 'A';
		}
		if( c > thresh ) {
			ss += 'C';
		}
		if( g > thres ) {
			ss += 'G';
		}
	}
}
*/		
bool	Qual::Next( Ifp& fp, QString& sid, QList<int>& data )
{
	Token	tok( Lexer::WhiteSpace );

	sid.clear();
	data.clear();

	if( !tok.next( fp )  ) {
		return( false );
	}
	if( !tok.line().startsWith( '>' ) ) {
		return( false );
	}
	sid = tok.at( 0 ).mid( 1 );
	while( tok.next( fp ) ) {
		if( tok.line().startsWith( '>' ) ) {
			fp.back();
			break;
		}
		for( int i = 0; i < tok.size(); ++i ) {
			data << INT( tok.at( i ) );
		}
	}
	return( true );
}
QString&	Fast::Reverse( QString& s )
{
        QStack<QChar>   stack;
        const int       len = s.size();

        for( int i = 0; i < len; ++i ) {
                stack.push( s[ i ] );
        }
        for( int i = 0; !stack.empty(); ++i, stack.pop() ) {
                s[ i ] = stack.top();
        }
        return( s );
}
QString&	Fast::ReverseComplement( QString& s )
{
        QStack<QChar>   stack;
        const int       len = s.size();

        for( int i = 0; i < len; ++i ) {
                stack.push( Complement( s[ i ] ) );
        }
        for( int i = 0; !stack.empty(); ++i, stack.pop() ) {
                s[ i ] = stack.top();
        }
        return( s );
}
bool	Fast::IsGU( const QChar& i, const QChar& j )
{
        bool    rv = false;

        switch( i.toAscii() ) {
                case    'G':
                case    'g':
                        switch( j.toAscii() ) {
                                case    'T':
                                case    't':
                                case    'U':
                                case    'u':
                                        rv = true;
                                        break;
                        }
                        break;
                case    'T':
                case    't':
                case    'U':
                case    'u':
                        switch( j.toAscii() ) {
                                case    'G':
                                case    'u':
                                        rv = true;
                                        break;
                        }
                        break;
        }
        return( rv );
}
bool	Fast::Next( Ifp& fp, QString& sid, QString& data )
{
	QString	line;

	sid.clear();
	data.clear();

	if( !nextLine( fp, line ) || !line.startsWith( '>' ) ) {
		return( false );
	}
	sid = line.mid( 1 );
	while( nextLine( fp, line ) ) {
		if( line.startsWith( '>' ) ) {
			fp.back();
			break;
		}
		data += line;
	}
	return( true );
}
void	Qual::init()
{
	sid = "";
	clear();
}
void	Qual::show( QTextStream& fp )
{
	fp << ">" << sid << endl;
	int i = 0;
	fp << at( i );
	for( ++i; i < size(); ++i ) {
		fp << " " << at( i );
	}
	fp << endl;
}
bool	nextQual( Ifp& fp, Qual& qual )
{
	QString	line;
	Token	tok( Lexer::WhiteSpace );

	qual.init();

	if( !nextLine( fp, line ) ) {
		return( false );
	}
	if( !line.startsWith( '>' ) ) {
		return( false );
	}
	qual.sid = line.mid( 1 );
	while( tok.next( fp ) ) {
		if( tok.line().startsWith( '>' ) ) {
			fp.back();
			break;
		}
		foreach( QString t, tok ) {
			qual << t.toInt();
		}
	}
/*	changed to handle multi-line qual values
	if( !tok.next( fp ) ) {
		return( false );
	}
	foreach( QString t, tok ) {
		qual << t.toInt();
	}
*/
	return( true );
}
	Fast::Fast( const QString& seq, const QString& sid )
{
	init();
	_seq = seq;
	_sid = sid;
}
void	Fast::init()
{
	_seq = _sid = _idline = QString();
}
void	Fast::setSeq( const QString& seq )
{
	_seq = seq;
}
void	Fast::setSid( const QString& sid )
{
	_sid = sid;
}
QString	Fast::seq() const
{
	return( _seq );
}
QString	Fast::sid() const
{
	return( _sid );
}
QString	Fast::idline() const
{
	return( _idline );
}
bool	Fast::next( Ifp& fp )
{
	bool	okay;
	QString	s;

	init();
	if( !nextLine( fp, _idline ) ) {
		return( false );
	}
	_sid = SnarfSid( _idline, &okay );
	if( !okay ) {
		return( false );
	}
	while( nextLine( fp, s ) ) {
		if( s.startsWith( IdDelimiter() ) ) {
			fp.back();
			break;
		}
		_seq += s;
	}
	//
	//	really do not know if empty seqs should be fail or not...
	//
	if( _seq.size() > 0 ) {
		return( true );
	}
	return( false );
}
bool	Fast::Next( Ifp& fp, Fast& fast )
{
	return( fast.next( fp ) );
}
void	Fast::show( QTextStream& fp ) const
{
	fp << IdDelimiter() << _sid << endl;
	for( int i = 0; i < _seq.size(); i += SeqLineSize ) {
		fp << _seq.mid( i, SeqLineSize ) << endl;
	}
}
void	Fast::Show( QTextStream& fp, const Fast& fast )
{
	fast.show( fp );
}
QChar	Fast::IdDelimiter()
{
	return( QChar( _IdDelimiter ) );
}
QString	Fast::SnarfSid( const QString& idline, bool* okay )
{
	QString	rv;
	QString	endChars = " |";


	if( okay ) {
		*okay = true;
	}

	if( idline.size() > 0 && idline.at( 0 ) == IdDelimiter() ) {
		int end = 1;
		for( ; end < idline.size(); ++end ) {
			if( endChars.contains( idline.at( end ) ) ) {
				--end;
				break;
			}
		}
		rv = idline.mid( 1, end );
		return( rv );
	}
	if( okay ) {
		*okay = false;
	}
	return( rv );
}
}	//	GH namespace
