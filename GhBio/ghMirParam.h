#ifndef	GH_MIR_PARAM_H
#define	GH_MIR_PARAM_H	1
#include "ghDef.h"
namespace	GH
{
class	GH_DLL_EXPORT	MirParam
{
static	const int	MAX_LOOP = 8;
static	const int	MAX_SIZE = 32;
static	const float	MAX_DG = -16.0;
static	const float	MIN_QPP = 0.6;
//static	const bool	NO_GU = true;
static	const bool	NO_GU = false;
static	const bool	SEEDED = true;
static	const bool	COMPLETE = false;

public:
	MirParam();
	void	parseArgs( QMap<QString,QString>& args );
	void	checkArgs( QMap<QString,QString>& args );

	float	max_dg;
	int	max_loop;
	int	max_size;
	float	min_qpp;
	bool	no_gu;
	bool	seeded;
	bool	complete;
	QString	inputPath;
};
}	//	GH namespace
#endif
