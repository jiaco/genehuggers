#ifndef	GH_FAST_H
#define	GH_FAST_H	1
#include "ghDef.h"
#include "ghIfp.h"
#include "ghToken.h"

namespace	GH
{
inline	QChar	Complement( const QChar& qch )
{
	int	ch = qch.toAscii();
	switch( ch ) {
		case	'A':
			return( 'T' );
			break;
		case	'a':
			return( 't' );
			break;
		case	'T':
			return( 'A' );
			break;
		case	't':
			return( 'a' );
			break;
		case	'C':
			return( 'G' );
			break;
		case	'c':
			return( 'g' );
			break;
		case	'G':
			return( 'C' );
			break;
		case	'g':
			return( 'c' );
			break;
		default:
			return( 'N' );
			break;
	}
}
class	GH_DLL_EXPORT	Flow
{
public:
	static const QString	Key;
	static	int	Floor( const double& val );
	static	QString	Call( const double& val, const int& keyIndex );
	static	QString	Call4( const QList<double>& data, const int& pos );
	static	int	CorrectEnd( const QList<double>& data, const int& end,
const double& maxHomoP, const double& signalMin, const double& noiseMax );

	static	QString	ReadBases( const QList<double>& data,
		 const int& end, QList<int>& itab);

	static	bool	RemoveIfFound( QList<double>& data, const QString& seq,
			 const int& pos = 0 );

	static	int	Next( Ifp& fp, QString& sid, QList<double>& data );
	static	bool	CheckAndRemove( QTextStream& fp, QList<double>& data, int& pos,
			 const QString& seq );
	static	void	Show( QTextStream& fp,	
			 const QList<double>& data, const int& pos );
	static	QString	Read( const QList<double>& data, const int& pos,
			 const int& len = 4, const double& threshold = 0.5 );
};
class	GH_DLL_EXPORT	Qual	: public	QList<int>
{
public:
	void		init();
	void		show( QTextStream& fp );

	static	bool	Next( Ifp& fp, QString& sid, QList<int>& data );

	QString		sid;
};
GH_DLL_EXPORT	bool	nextQual( Ifp& fp, Qual& qual );

class	GH_DLL_EXPORT Fast
{
public:
	static const int SeqLineSize = 70;
	static bool IsGU( const QChar& i, const QChar& j );
	static QString&	Reverse( QString& s );
	static QString& ReverseComplement( QString& s );

	Fast( const QString& seq = QString(), const QString& sid = QString() );
	void	init();

	void	setSid( const QString& sid );
	void	setSeq( const QString& seq );

	QString	seq() const;
	QString	sid() const;
	QString	idline() const;

	bool		next( Ifp& fp );
	static bool	Next( Ifp& fp, Fast& fast );

	static bool	Next( Ifp& fp, QString& sid, QString& data );

	void		show( QTextStream& fp ) const;
	static void	Show( QTextStream& fp, const Fast& fast );

	static	QChar	IdDelimiter();
	static	QString	SnarfSid( const QString& idline, bool* okay = 0 );
protected:
	QString	_seq;
	QString	_sid;
	QString	_idline;
	static const int _IdDelimiter = '>';
};


}	//	GH namespace
#endif	//	GH_FAST_H
