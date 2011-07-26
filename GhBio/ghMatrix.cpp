#include "ghMatrix.h"
#include <QDebug>

namespace	GH
{
	IUPAC::IUPAC()
{
	(*this).insert( 'A', "A" );
	(*this).insert( 'C', "C" );
	(*this).insert( 'G', "G" );
	(*this).insert( 'T', "T" );
	(*this).insert( 'R', "AG" );
	(*this).insert( 'Y', "CT" );
	(*this).insert( 'S', "GC" );
	(*this).insert( 'W', "AT" );
	(*this).insert( 'K', "GT" );
	(*this).insert( 'M', "AC" );
	(*this).insert( 'B', "CGT" );
	(*this).insert( 'D', "AGT" );
	(*this).insert( 'H', "ACT" );
	(*this).insert( 'V', "ACG" );
	(*this).insert( 'N', "ACGT" );
}
double	Log2( const double& d )
{
	double	rv;

	rv = log( d ) / log( 2 );
	return( rv );
}
	NucFreq::NucFreq()
{
	init( 0.25, 0.25, 0.25, 0.25 );
}
void	NucFreq::init( const double& a, const double& c,
	 const double& g, const double& t )
{
	clear();
	append( a );
	append( c );
	append( g );
	append( t );
	return;
}
void	NucFreq::setHumanGenome()
{
	init( NF_HUMAN_A, NF_HUMAN_C, NF_HUMAN_G, NF_HUMAN_T );
}
bool	NucFreq::read( Ifp& fp )
{
	QString	lbuf;
	QStringList	tok;

	if( ( lbuf = fp.next() ) == USTR ) {
		return( false );
	}
	tok = lbuf.split( "\t" );
	if( tok.size() != 4 ) {
		return( false );
	}
	init( tok[ A ].toDouble(), tok[ C ].toDouble(), tok[ G ].toDouble(), tok[ T ].toDouble() );
	return( true );
}
bool	NucFreq::load( const QString& path )
{
	Ifp	fp;
	if( !fp.open( path ) ) {
		return( false );
	}
	read( fp );
	fp.close();
	return( true );
}
void	NucFreq::show( Ofp& ofp )
{
	ofp << at( A ) << "\t" << at( C ) << "\t" << at( G ) << "\t" << at( T ) << endl;
}
void	CalcNucFreq( const QString& s, NucFreq& nf )
{
	MatrixPosition	mp;

	int	cnt = 0;
	foreach( QChar ch, s ) {
		int i = B2I( ch );
		if( i != N ) {
			++mp[ i ];
			++cnt;
		}
	}
	for( int i = A; i <= T; ++i ) {
		mp[ i ] /= cnt;
	}
	nf.init( mp[ A ], mp[ C ], mp[ G ], mp[ T ] );
}
	MatrixPosition::MatrixPosition()
{
	for( int i = 0; i < MAT_SIZE; ++i ) {
		append( 0.0 );
	}
}
double	MatrixPosition::score( const QChar& ch ) const
{
/*
	int i = B2I( ch );
	if( i == N ) {
		qDebug()  << ch << "\t" << (*this).at( i );
	}
*/
	return( (*this).at( B2I( ch ) ) );
}
double	MatrixPosition::min( const bool& nonZero )
{
	double rv = 1000.0;
	for( int pos = A; pos <= T; ++pos ) {
		if( nonZero && (*this).at( pos ) == 0 ) {
			continue;
		}
		if( (*this).at( pos ) < rv ) {
			rv = (*this).at( pos );
		}
	}
	return( rv );
}
double	MatrixPosition::max()
{
	double rv = (*this).at( A );
	for( int pos = C; pos <= T; ++pos ) {
		if( (*this).at( pos ) > rv ) {
			rv = (*this).at( pos );
		}
	}
	return( rv );
}
double	Matrix::score( const QString& s, const int& pos ) const
{
	double	rv = 0.0;
	for( int i = 0; i < size(); ++i ) {
		if( s.size() < pos + i ) {
			break;
		}
		rv += (*this)[ i ].score( s.at( pos + i ) );
	}
	return( rv );
}
double	Matrix::threshold()
{
	return( thresh );
}
void	Matrix::init()
{
	clear();
	id = "";
	thresh = 0.0;
	auxFlag = 0;
	return;
}
bool	Matrix::form( const QString& site, const QString& sid )
{
	init();
	id = sid;
	sequence = site;
	for( int pos = 0; pos < site.size(); ++pos ) {
		MatrixPosition mp;
		if( site[ pos ] == '.' || site[ pos ] == 'N' ) {
			mp[ A ] += 0.25;
			mp[ C ] += 0.25;
			mp[ G ] += 0.25;
			mp[ T ] += 0.25;
		} else if( site[ pos ] == '[' ) {
			QString	iupac;
			++pos;
			while( pos < site.size() && site[ pos ] != ']' ) {
				iupac.append( site[ pos ] );
				++pos;
			}
			if( iupac.size() < 1 ) {
				return( false );
			}
			foreach( QChar ch, iupac ) {
				mp[ B2I( ch ) ] += 1.0 / iupac.size();
			}
		} else {
			++mp[ B2I( site[ pos ] ) ];
		}
		(*this) << mp;
	}
	return( true );
}
bool	Matrix::append( const QString& site )
{
	if( site.size() != size() ) {
		return( false );
	}
	for( int pos = 0; pos < size(); ++pos ) {
		int	i = B2I( site[ pos ] );
		if( i >= A && i <= T ) {
			++(*this)[ pos ][ i ];
		}
	}
	return( true );
}
void	Matrix::formLog2( const NucFreq& nf )
{
	for( int pos = 0; pos < size(); ++pos ) {
		double a, c, g, t, sum;
		a = (*this)[ pos ][ A ];
		c = (*this)[ pos ][ C ];
		g = (*this)[ pos ][ G ];
		t = (*this)[ pos ][ T ];
		sum = a + g + c + t;
		(*this)[ pos ][ A ] = Log2( ( a / sum ) / nf[ A ] );
		(*this)[ pos ][ C ] = Log2( ( c / sum ) / nf[ C ] );
		(*this)[ pos ][ G ] = Log2( ( g / sum ) / nf[ G ] );
		(*this)[ pos ][ T ] = Log2( ( t / sum ) / nf[ T ] );
	}
}
bool	Matrix::isSimple()
{
	return( simple );
}
bool	Matrix::readMeme( Ifp& fp, const NucFreq& nf )
{
	bool	foundMotif = false;
	QString	lbuf;
	QStringList	tok, slist;

	init();
	while( ( lbuf = fp.next() ) != USTR ) {
		if( lbuf.startsWith( "MOTIF" ) ) {
			foundMotif = true;
			break;
		}
	}
	if( !foundMotif ) {
		return( false );
	}
	while( ( lbuf = fp.next() ) != USTR ) {
		if( lbuf.contains( "BLOCKS format" ) ) {
			tok = lbuf.split( ' ', QString::SkipEmptyParts );
			id = QString( "meme%1" ).arg( tok[ 1 ] );
			break;
		}
	}
	fp.next();
	fp.next();

	while( ( lbuf = fp.next() ) != USTR ) {
		if( lbuf.startsWith( "//" ) ) {
			break;
		}
		tok = lbuf.split( ' ', QString::SkipEmptyParts );
		slist << tok[ tok.size() - 2 ];
	}
	if( slist.size() == 0 ) {
		return( false );
	}
/*
	if we keep track of all counts and then check if A+t or A+C or C+g or T=G is 
	dominant?
 */
	double	t_a, t_c, t_g, t_t;
	t_a = t_c = t_g = t_t = 0.0;

	for( int j = 0; j < slist[ 0 ].size(); ++j ) {
		double a, c, g, t;
		a = c = g = t = 0.0;
		for( int i = 0; i < slist.size(); ++i ) {
			if( slist[ i ][ j ] == 'A' ) {
				++a;
			} else if( slist[ i ][ j ] == 'C' ) {
				++c;
			} else if( slist[ i ][ j ] == 'G' ) {
				++g;
			} else if( slist[ i ][ j ] == 'T' ) {
				++t;
			}
		}
		t_a += a;
		t_c += c;
		t_g += g;
		t_t += t;

		double	max = a + c + g + t;
		MatrixPosition	p;
		p[ A ] = Log2( ( a / max ) / nf[ A ] );
		p[ C ] = Log2( ( c / max ) / nf[ C ] );
		p[ G ] = Log2( ( g / max ) / nf[ G ] );
		p[ T ] = Log2( ( t / max ) / nf[ T ] );
		(*this) << p;
	}
	t_a /= slist.size();
	t_c /= slist.size();
	t_g /= slist.size();
	t_t /= slist.size();

	simple = false;
	if( t_a > 9 || t_c > 9 || t_g > 9 || t_t > 9 ) {
		simple = true;
	} else if(
		 t_a + t_c > 9.2 ||
		 t_a + t_g > 9.2 ||
		 t_a + t_t > 9.2 ||
		 t_c + t_g > 9.2 ||
		 t_c + t_t > 9.2 ||
		 t_g + t_t > 9.2  ) {
		simple = true;
	}

// had to disable this, numbers were always big...
	simple = false;

	thresh = score( slist[ 0 ], 0 );
	for( int i = 1; i < slist.size(); ++i ) {
		double	 x = score( slist[ i ], 0 );
		if( x < thresh ) {
			thresh = x;
		}
	}
	return( true );
}
double	Matrix::assign( const QString& sval, const double& bump )
{
	if( sval == "-1" ) {
		return( -9999 );
	} else {
		return( sval.toDouble() + bump );
	}
}
bool	Matrix::read( Ifp& fp, const NucFreq& nf, const double& bump )
{
	QString	lbuf;
//
//	in order to avoid -inf in the matrix, added the bump variable
//
	init();
	if( ( lbuf = fp.next() ) == USTR ) {
		return( false );
	}
	if( !lbuf.startsWith( ">" ) ) {
		return( false );
	}
	// parse defline
	terms = lbuf.mid( 1 ).split( " ", QString::SkipEmptyParts );
	id = terms[ 0 ];
	thresh = terms[ 1 ].toDouble();
	while( ( lbuf = fp.next() ) != USTR ) {
		if( lbuf.startsWith( ">" ) ) {
			fp.back();
			break;
		}
		MatrixPosition	p;
		QStringList	tok;
		tok = lbuf.split( "\t" );
		double a = assign( tok[ A ], bump );
		double c = assign( tok[ C ], bump );
		double g = assign( tok[ G ], bump );
		double t = assign( tok[ T ], bump );
/*
		double	a = tok[ A ].toDouble() + bump;
		double	c = tok[ C ].toDouble() + bump;
		double	g = tok[ G ].toDouble() + bump;
		double	t = tok[ T ].toDouble() + bump;
*/
		double	max = 0;
		if( a > 0 ) {
			max += a;
		}
		if( c > 0 ) {
			max += c;
		}
		if( g > 0 ) {
			max += g;
		}
		if( t > 0 ) {
			max += t;
		}
		//double	max = a + c + g + t;
		if( a > 0 ) {
			p[ A ] = Log2( ( a / max ) / nf[ A ] );
		} else {
			p[ A ] = a;
		}
		if( c > 0 ) {
			p[ C ] = Log2( ( c / max ) / nf[ C ] );
		} else {
			p[ C ] = c;
		}
		if( g > 0 ) {
			p[ G ] = Log2( ( g / max ) / nf[ G ] );
		} else {
			p[ G ] = g;
		}
		if( t > 0 ) {
			p[ T ] = Log2( ( t / max ) / nf[ T ] );
		} else {
			p[ T ] = t;
		}
	//	p[ A ] = Log2( ( a / max ) / nf[ A ] );
	//	p[ C ] = Log2( ( c / max ) / nf[ C ] );
	//	p[ G ] = Log2( ( g / max ) / nf[ G ] );
	//	p[ T ] = Log2( ( t / max ) / nf[ T ] );
		(*this) << p;
	}
	return( true );
}
bool	Matrix::parse( const QList<QList<QString> >& data )
{
	init();
	for( int pos = 0; pos < data.size(); ++pos ) {
		MatrixPosition p;

		p[ A ] = data[ pos ][ A ].toDouble();
		p[ C ] = data[ pos ][ C ].toDouble();
		p[ G ] = data[ pos ][ G ].toDouble();
		p[ T ] = data[ pos ][ T ].toDouble();
		(*this) << p;
	}
	return( true );
}
bool	Matrix::parse( const QList<QList<QString> >& data, const NucFreq& nf )
{
	init();
	for( int pos = 0; pos < data.size(); ++pos ) {
		MatrixPosition p;

		double a = data[ pos ][ A ].toDouble();
		double c = data[ pos ][ C ].toDouble();
		double g = data[ pos ][ G ].toDouble();
		double t = data[ pos ][ T ].toDouble();
		double	max = a + c + g + t;
		p[ A ] = Log2( ( a / max ) / nf[ A ] );
		p[ C ] = Log2( ( c / max ) / nf[ C ] );
		p[ G ] = Log2( ( g / max ) / nf[ G ] );
		p[ T ] = Log2( ( t / max ) / nf[ T ] );
		(*this) << p;
	}
	return( true );
}
bool	Matrix::freqAdjust( const NucFreq& nf )
{
	for( int pos = 0; pos < size(); ++pos ) {
		double max = 0.0;
		for( int i = A; i <= T; ++i ) {
			max += (*this)[ pos ][ i ];
		}
		for( int i = A; i <= T; ++i ) {
			double	nval = 0.0;
			double	oval = (*this)[ pos ][ i ];
			if( oval != 0 ) {
				nval = Log2( ( oval / max ) / nf[ i ] );
			}
			(*this)[ pos ][ i ] = nval;
		}
	}
	return( true );
}
bool	Matrix::mmThreshold( const int& nMm, const bool& lowest )
{
	QList<double>	tab;

	if( size() - 1 <= nMm ) {
		return( false );
	}

	for( int pos = 0; pos < size(); ++pos ) {
		tab << (*this)[ pos ].min( true );
	}
	qSort( tab.begin(), tab.end() );

	// lowest is first entry in tab
	int	beg, end;
	if( lowest ) {
		beg = nMm;
		end = tab.size();
	} else {
		beg = 0;
		end = tab.size() - nMm;
	}

	thresh = 0.0;
	for( int pos = beg; pos < end; ++pos ) {
		thresh += tab[ pos ];
	}
	return( true );
}
void	Matrix::showAsPercent( Ofp& fp, const bool& revcomp )
{
	double	tot = (*this)[ 0 ][ A ];
	tot += (*this)[ 0 ][ C ];
	tot += (*this)[ 0 ][ G ];
	tot += (*this)[ 0 ][ T ];

	fp << ">" << id;
	fp << endl;
	if( revcomp ) {
	for( int i = (*this).size() - 1; i >= 0; --i ) {
		fp << ( (*this)[ i ][ T ] / tot ) * 100 << "\t";
		fp << ( (*this)[ i ][ G ] / tot ) * 100 << "\t";
		fp << ( (*this)[ i ][ C ] / tot ) * 100 << "\t";
		fp << ( (*this)[ i ][ A ] / tot ) * 100 << "\n";
	}
	} else {
	for( int i = 0; i < (*this).size(); ++i ) {
		fp << ( (*this)[ i ][ A ] / tot ) * 100 << "\t";
		fp << ( (*this)[ i ][ C ] / tot ) * 100 << "\t";
		fp << ( (*this)[ i ][ G ] / tot ) * 100 << "\t";
		fp << ( (*this)[ i ][ T ] / tot ) * 100 << "\n";
	}
	}
}
void	Matrix::show( Ofp& fp, const double& mean, const double& sdev )
{
	fp << ">" << id;
	if( mean != 0.0 ) {
		fp << " " << mean;
	}
	if( sdev != 0.0 ) {
		fp << " " << sdev;
	}
	fp << endl;
	for( int i = 0; i < (*this).size(); ++i ) {
		fp << (*this)[ i ][ A ] << "\t";
		fp << (*this)[ i ][ C ] << "\t";
		fp << (*this)[ i ][ G ] << "\t";
		fp << (*this)[ i ][ T ] << "\n";
	}
}
void	Matrix::setAux( const int& on_off )
{
	auxFlag = on_off;
}
int	Matrix::aux()
{
	return( auxFlag );
}
int	LoadMatrix( const QString& path, QList<Matrix>& mlist, const NucFreq& nf )
{
	int	rv = UINT;
	Ifp	fp;
	Matrix	matrix;

	if( !fp.open( path ) ) {
		return( rv );
	}
	if( path.endsWith( ".meme" ) ) {
qDebug() << "Reading meme file";
		for( rv = 0; matrix.readMeme( fp, nf ); ++rv ) {
qDebug() << " 1";
			if( !matrix.isSimple() ) {
qDebug() << " 2";
				mlist << matrix;
			}
		}
	} else {
		for( rv = 0; matrix.read( fp, nf ); ++rv ) {
			mlist << matrix;
		}
	}
	fp.close();
	return( rv );
}
void	ReverseComplementMatrix( const Matrix& in, Matrix& out )
{
	out.id = in.id;
	out.terms = in.terms;
	out.thresh = in.thresh;
	for( int i = in.size() - 1; i >= 0; --i ) {
		MatrixPosition p;
		p[ A ] = in[ i ][ T ];
		p[ C ] = in[ i ][ G ];
		p[ G ] = in[ i ][ C ];
		p[ T ] = in[ i ][ A ];
		out << p;
	}
	out.setAux( 1 );
	return;
}

}	//	GH namespace
