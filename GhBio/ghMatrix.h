#ifndef	GH_MATRIX_H
#define	GH_MATRIX_H	1
#include <GhCore.h>
#include <math.h>

/*
 *	should probably be calculating the threshold from the nucfreq
	something like the max score * 0.8?
 */

namespace	GH
{
static	const	int	A = 0;
static	const	int	C = 1;
static	const	int	G = 2;
static	const	int	T = 3;
static	const	int	N = 4;
static	const	int	MAT_SIZE = 5;

/*
 * IUPAC
IUPAC	R	AG
IUPAC	Y	CT
IUPAC	S	GC
IUPAC	W	AT
IUPAC	K	GT
IUPAC	M	AC
IUPAC	B	CGT
IUPAC	D	AGT
IUPAC	H	ACT
IUPAC	V	ACG
IUPAC	N	ACGT
 */
class	GH_DLL_EXPORT	IUPAC	:	public	QMap<QChar,QString>
{
public:
	IUPAC();
};
GH_DLL_EXPORT	double	Log2( const double& d );

inline int	B2I( const QChar& ch )
{
	switch( ch.toAscii() ) {
		case	'a':
		case	'A':
			return( A );
			break;
		case	'c':
		case	'C':
			return( C );
			break;
		case	'g':
		case	'G':
			return( G );
			break;
		case	't':
		case	'T':
		case	'u':
		case	'U':
			return( T );
			break;
		default:
			return( N );
			break;
	}
}
inline	QChar	I2B( const int& i )
{
	switch( i ) {
		case	A:
			return( 'A' );
			break;
		case	C:
			return( 'C' );
			break;
		case	G:
			return( 'G' );
			break;
		case	T:
			return( 'T' );
			break;
	}
	return( 'N' );
}
const	double	NF_HUMAN_A = 0.295;
const	double	NF_HUMAN_C = 0.205;
const	double	NF_HUMAN_G = 0.205;
const	double	NF_HUMAN_T = 0.295;

class	GH_DLL_EXPORT	NucFreq	:	public QList<double>
{
public:
	NucFreq();
	void	init( const double& a, const double& c,
			 const double& g, const double& t );
	void	setHumanGenome();
	void	show( Ofp& ofp );
	bool	read( Ifp& fp );
	bool	load( const QString& path );
};
class	GH_DLL_EXPORT	MatrixPosition	:	public	QList<double>
{
public:
	MatrixPosition();
	double	score( const QChar& ch ) const;
	double	min(  const bool& nonZero = false );
	double	max();
};
class	GH_DLL_EXPORT	Matrix	:	public	QList<MatrixPosition>
{
public:
	double	assign( const QString& sval, const double& bump = 1.0 );
	bool	read( Ifp& fp, const NucFreq& nf, const double& bump = 1.0 );
	bool	parse( const QList<QList<QString> >& data, const NucFreq& nf );
	bool	parse( const QList<QList<QString> >& data );
	bool	readMeme( Ifp& fp, const NucFreq& nf );

	bool	freqAdjust( const NucFreq& nf );
	bool	mmThreshold( const int& nMm, const bool& lowest = true );
	bool	form( const QString& site, const QString& sid = USTR );
	bool	append( const QString& site );
	void	formLog2( const NucFreq& nf = NucFreq() );
	void	show( Ofp& fp, const double& mean = 0.0,
		 const double& sdev = 0.0 );
	void	showAsPercent( Ofp& fp, const bool& revcomp = false );
	void	init();
	double	score( const QString& s, const int& pos ) const; 
	double	threshold();
	
	void	setAux( const int& on_off );
	int	aux();
	QString	id, sequence;
	QStringList	terms;
	double	thresh;
	bool	isSimple();
private:
	bool	simple;
	int	auxFlag;
};
// see gh3/seqTools/gatcOrder for a MatrixMask example
GH_DLL_EXPORT	void	CalcNucFreq( const QString& s, NucFreq& nf );
GH_DLL_EXPORT	int	LoadMatrix( const QString& path, QList<Matrix>& mlist,
	 const NucFreq& nf );

GH_DLL_EXPORT	void	ReverseComplementMatrix( const Matrix& in, Matrix& out );

}	//	GH namespace
#endif	//	GH_MATRIX_H
