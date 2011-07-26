#ifndef	GH_BEDUTILS_H
#define	GH_BEDUTILS_H	1
#include <GhCore.h>
#include "ghBed.h"

namespace	GH
{
GH_DLL_EXPORT WigSet	WigSet_ValFromCount( WigSet& data );

GH_DLL_EXPORT WigSet	WigSet_Smooth( const WigSet& data,
	 		 const QMap<QString,QVariant>& params );

GH_DLL_EXPORT WigList	Bounds( const WigSetList& input,
		 const BedSetList& features,
		 const int& dws, const int& sws );

GH_DLL_EXPORT WigList	Smooth( const WigList& data,
	 		 const int& dataSize, const int& windowSize,
	 		 const int& minPosition, const int& incPosition );

GH_DLL_EXPORT	double	WigList_average( const WigList& data, 
	 const double& minValue = UDBL, const double& maxValue = UDBL );
GH_DLL_EXPORT	void	WigList_findSeeds( const WigList& data, WZoneList& seeds,
	 const int& minNumProbes, const double& minValue,
	 const double& maxValue, const double& minScore );

GH_DLL_EXPORT	void	WZoneList_calcScore( const WigList& data, WZoneList& wzlist, 
	 const double& minValue = UDBL, const double& maxValue = UDBL );
GH_DLL_EXPORT	double	WZone_calcCoverage( const WZone& wz );

GH_DLL_EXPORT	void	WZoneList_coverageFilter( WZoneList& wzlist, const double& min );
GH_DLL_EXPORT	void	WZoneList_scoreFilter( WZoneList& wzlist, const double& min );
GH_DLL_EXPORT	void	WZoneList_minSizeFilter( WZoneList& wzlist, const int& min );
GH_DLL_EXPORT	void	WZoneList_merge( WZoneList& wzlist, const int& distance );

GH_DLL_EXPORT	int	BestHiExtend( const WigList& data, const double& maxValue,
	 const int& width, const int& pos, double& score );
GH_DLL_EXPORT	int	BestLoExtend( const WigList& data, const double& maxValue,
	 const int& width, const int& pos, double& score );

GH_DLL_EXPORT	int	WZoneList_extend( const WigList& data, WZoneList& wzlist,
	 const double& maxValue, const int& width, const double& pct );


GH_DLL_EXPORT	void    FormOverZones( const WigList& data, WigList& tab,
         const int& windowSize, const double& valueCutoff,
         const double& percentOver );
GH_DLL_EXPORT	void    MergeZones( const WigList& data, const WigList& dtab,
         WZoneList& mtab, const int& windowSize,
         const int& mergeDistance );
}	//	GH namespace
#endif	//	GH_HTGSCONCURRENT_H
