#ifndef	GH_HYB_H
#define	GH_HYB_H	1
#include <GhCore.h>
#include "ghFast.h"
#include "ghMirParam.h"
namespace	GH
{
class	GH_DLL_EXPORT	Hyb
{
public:
	static	Token	header();

	QString	getAlign() const;
	int	subjectSize() const;
	double	qpp() const;
	int	maxLoopSize() const;
	bool	isQueryComplete( const int& deltaEnd = 0 ) const;
	bool	isValid( const MirParam& param ) const;

	//void	giveHeader( Hdr& header );
	void	giveRow( Row& row );
	bool	fromRow( const Row& row );
	void	pretty( Ofp& fp, bool reverse = true );
	void	process( int offset = 0 );
	void	clear();

	void	appendLoop( const QChar& ich, const QChar& jch );
	void	appendPair( const QChar& ich, const QChar& jch );
	void	appendIBulge( const QChar& jch );
	void	appendJBulge( const QChar& ich );

	QString	iseq, jseq;
	int	ipos, jpos;
	float	energy;
	QString	structure;

	QString	iid, jid;
	int	qbeg, qend, qlen;
private:
	QString	ibuf, jbuf, abuf;
	int	ibeg, iend, jbeg, jend;
};
}	//	GH namespace
#endif
