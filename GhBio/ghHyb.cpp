#include "ghHyb.h"
#include <QDebug>

namespace	GH
{
bool	Hyb::isValid( const MirParam& param ) const
{
	bool	rv = false;
	if( param.complete && !isQueryComplete( 2 ) ) {
		return( rv );
	}
/*
	if( param.complete &&
	 ( qbeg != 0 || qend < ( qlen - 3 ) ) ) {
		return( rv );
	}
*/
	if( energy > param.max_dg ) {
		return( rv );
	}
	if( subjectSize() > param.max_size ) {
		return( rv );
	}
	if( qpp() < param.min_qpp ) {
		return( rv );
	}
	if( maxLoopSize() > param.max_loop ) {
		return( rv );
	}
	rv = true;
	return( rv );
}
QString	Hyb::getAlign() const
{
	return( abuf );
}
bool	Hyb::isQueryComplete( const int& deltaEnd ) const
{
	if( ibeg == 0 && iend > iseq.size() - ( deltaEnd + 1 )  ) {
		return( true );
	}
	return( false );
}
int	Hyb::subjectSize() const
{
	return( jbuf.size() );
}
int	Hyb::maxLoopSize() const
{
	int	max = 0;
	int	cnt = 0;

	foreach( QChar ch, ibuf ) {
		if( cnt > 0 ) {
			if( ch == '-' ) {
				++cnt;
				continue;
			} else if( cnt > max ) {
				max = cnt;
			}
			cnt = 0;
		} else if( ch == '-' ) {
			++cnt;
		}
	}
	cnt = 0;
	foreach( QChar ch, jbuf ) {
		if( cnt > 0 ) {
			if( ch == '-' ) {
				++cnt;
				continue;
			} else if( cnt > max ) {
				max = cnt;
			}
			cnt = 0;
		} else if( ch == '-' ) {
			++cnt;
		}
	}
	return( max );
}
double	Hyb::qpp() const
{
	double	rv = 0.0;
	double bot = 0.0;
	foreach( QChar ch, ibuf ) {
		if( ch == '-' ) {
			continue;
		}
		++bot;
		if( ch.isUpper() ) {
			++rv;
		}
/*
		if( ch != '-' && ch.isUpper() ) {
			rv += 1.0;
		}
*/
	}
	rv /= bot;
	//rv /= ibuf.size();
	return( rv );
}
Token	Hyb::header()
{
	Token	rv;
	rv
	 << "sid" << "beg" << "end"
	 << "energy"
	 << "qid" << "qbeg" << "qend"
	 << "sbuf" << "abuf" << "qbuf";
	return( rv );
}
/*
void	Hyb::giveHeader( Hdr& header )
{
	header.clear();
	header
	 << "sid" << "beg" << "end"
	 << "energy"
	 << "qid" << "qbeg" << "qend"
	 << "sbuf" << "abuf" << "qbuf";
}
*/
void	Hyb::giveRow( Row& row )
{
	row.clear();
	// i = mir j = subject

	row.insert( "sid", QVariant( jid ) );
	row.insert( "beg", QVariant( jbeg ) );
	row.insert( "end", QVariant( jend ) );

	row.insert( "energy", QVariant( energy ) );
	row.insert( "qid", QVariant( iid ) );
	row.insert( "qbeg", QVariant( ibeg ) );
	row.insert( "qend", QVariant( iend ) );

	row.insert( "sbuf", QVariant( jbuf ) );
	row.insert( "abuf", QVariant( abuf ) );
	row.insert( "qbuf", QVariant( ibuf ) );
}
bool	Hyb::fromRow( const Row& row )
{
	jid = STR( row[ "sid" ] );
	jbeg = INT( row[ "beg" ] );
	jend = INT( row[ "end" ] );
	energy = DBL( row[ "energy" ] );
	iid = STR( row[ "qid" ] );
	ibeg = INT( row[ "qbeg" ] );
	iend = INT( row[ "qend" ] );
	jbuf = STR( row[ "sbuf" ] );
	abuf = STR( row[ "abuf" ] );
	ibuf = STR( row[ "qbuf" ] );
	return( true );
}
void	Hyb::pretty( Ofp& fp, bool reverse )
{
/*
 *      the alignment is generated:
 *              query   (i) 5'-3'
 *      and     subject (j) 3'-5'
 *
 *      reversing it makes more sense for mirs since then the
 *      subject is 5'-3'
 */
        if( reverse ) {
/*
                Seq::Reverse( abuf );
                Seq::Reverse( ibuf );
                Seq::Reverse( jbuf );
                fp << "5'- " << ibuf << " -3'"
                 << "\t" << ibeg + 1 << " to " << iend + 1
                 << " on " << iid << endl;

                fp << "    " << abuf << endl;

                fp << "3'- " << jbuf << " -5'"
                 << "\t" << jend + 1 << " to " << jbeg + 1
                 << " on " << jid << endl;

                Seq::Reverse( abuf );
                Seq::Reverse( ibuf );
                Seq::Reverse( jbuf );
*/
        } else {
                fp << "3'- " << ibuf << " -5'"
                 << "\t" << iend + 1 << " to " << ibeg + 1
                 << " on " << iid << endl;

                fp << "    " << abuf << endl;

                fp << "5'- " << jbuf << " -3'"
                 << "\t" << jbeg + 1 << " to " << jend + 1
                 << " on " << jid << endl;
        }
        fp << "dG = " << energy << endl << endl;
        return;
}
void	Hyb::process( int offset )
{
	// psd =  parenthetical structure diagram

	QString	ipsd, jpsd;
	bool	inJ = false;
	int	i, j, si, sj;

	for( i = 0; i < structure.size(); ++i ) {
		if( structure[ i ] == '&' ) {
			inJ = true;
		} else if( inJ ) {
			jpsd += structure[ i ];
		} else {
			ipsd += structure[ i ];
		}
	}
//hyb.clearStructure();
//hyb.dg = energy;
	i = ipsd.size() - 1;
	j = 0;
	si = ipos - 1;
	sj = jpos - 1;
	iend = si;
	jbeg = sj;
	while( i >= 0 && j < jpsd.size() ) {
		if( ipsd[ i ] == '.' && jpsd[ j ] == '.' ) {
			appendLoop( iseq[ si ], jseq[ sj ] );
			--i;
			--si;
			++j;
			++sj;
		} else if( ipsd[ i ] == '(' && jpsd[ j ] == ')' ) {
			appendPair( iseq[ si ], jseq[ sj ] );
			--i;
			--si;
			++j;
			++sj;
		} else if( ipsd[ i ] == '(' ) {
			appendIBulge( jseq[ sj ] );
			++j;
			++sj;
		} else if( jpsd[ j ] == ')' ) {
			appendJBulge( iseq[ si ] );
			--i;
			--si;
		}
	}
	ibeg = si + 1;
	jbeg += offset;
	jend = sj + offset;

	qbeg = ibeg;
	qend = iend;
	qlen = iseq.size();
}
void	Hyb::clear()
{
	iseq = jseq = structure = "";
	ipos = jpos = 0;
	energy = 0.0;

	abuf = ibuf = jbuf = "";
}
void	Hyb::appendLoop( const QChar& ich, const QChar& jch )
{
	abuf += ' ';
	ibuf += ich.toLower();
	jbuf += jch.toLower();
}
void	Hyb::appendPair( const QChar& ich, const QChar& jch )
{
	if( Fast::IsGU( ich, jch ) ) {
		abuf += ':';
	} else {
		abuf += '|';
	}
	ibuf += ich;
	jbuf += jch;
}
void	Hyb::appendIBulge( const QChar& jch )
{
	abuf += ' ';
	ibuf += '-';
	jbuf += jch.toLower();
}
void	Hyb::appendJBulge( const QChar& ich )
{
	abuf += ' ';
	ibuf += ich.toLower();
	jbuf += '-';
}
}	//	GH namespace
