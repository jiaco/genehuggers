#ifndef	GH_AXES_H
#define	GH_AXES_H	1
#include <GhCore.h>
#include <GhBio.h>

namespace	GH
{
class	GH_DLL_EXPORT Axes
{
public:
	Axes();
	
	qreal	xspace, xrange, xmin, xmax;
	int	xincr;
	qreal	yspace, yrange, ymin, ymax;
	
	void	xinit( const WigList& data );
	void	xinit( const int& lo, const int& hi );
	void	yinit( const WigList& data );
	void	yinit( const double& lo, const double& hi );
	void	ycalc( const WigList& data,
		const double& dlo, const double& dhi );
	qreal	getX( const int& pos ) const;
	qreal	getY( const double& val, bool* ok = 0 ) const;
private:
	bool	initX, initY;
};
}	//	GH namespace
#endif	//	GH_AXES_H
