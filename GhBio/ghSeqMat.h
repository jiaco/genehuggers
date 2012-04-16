#ifndef	GH_SEQMAT_H
#define	GH_SEQMAT_H	1
#include <GhBio.h>

namespace	GH
{
class	GH_DLL_EXPORT SeqMat
{
public:
	SeqMat( const QString& pattern, const QString& label = "NA" );
	~SeqMat();

	int	size() const;
	int	bestMatch( const QString& target, const int& mm, double& score );
	int	match( const QString& target, const int& mm = 0 );
	int	rmatch( const QString& target, const int& mm = 0 );
	double	score( const QString& target ) const;
	double	rscore( const QString& target ) const;
	double	lastScore() const;
	QString pattern() const;
	QString label() const;
	int	mismatches() const;
protected:
	QString	_pattern;
	QString	_label;
	int	_mismatches;
	double	**_m;
	double	**_rm;
	int	_size;
	double	_score;
};
}	//	GH namespace
#endif	//	GH_SEQMAT_H
