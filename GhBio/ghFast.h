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
class	GH_DLL_EXPORT	Qual	: public	QList<int>
{
public:
	void		init();
	void		show( QTextStream& fp );

	QString		sid;
};
GH_DLL_EXPORT	bool	nextQual( Ifp& fp, Qual& qual );

class	GH_DLL_EXPORT Fast
{
public:
	static const int SeqLineSize = 70;

	Fast( const QString& seq = QString(), const QString& sid = QString() );
	void	init();

	void	setSid( const QString& sid );
	void	setSeq( const QString& seq );

	QString	seq() const;
	QString	sid() const;
	QString	idline() const;

	bool		next( Ifp& fp );
	static bool	Next( Ifp& fp, Fast& fast );

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
