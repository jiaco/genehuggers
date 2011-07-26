#ifndef	GH_BEDLOADER_H
#define	GH_BEDLOADER_H	1

#include <GhCore.h>
#include <ghGuiDac.h>
#include <ghBed.h>

namespace	GH
{
class	GH_DLL_EXPORT	BedLoader	:	public GH::Errorprone
{
public:

	bool	load( const QVariant& param,
		 QList<BedSet>& data, const bool& isGui );

		//
		//	wrapper to BedSet::load()
		// 	 - not actually used in load
		//	  as we use QtConcurrent::map() on a QList<BedSet>
		//	 - in fact, not sure how to implement it
		//
	//bool	loadOneSidPerFile( const QString& path );

		//
		//	this used to be called WigSetList_reader
		//
	QList<BedSet>	loadMultiSidPerFile( const QString& path );
};
}	//	GH namespace
#endif	//	GH_BEDLOADER_H
