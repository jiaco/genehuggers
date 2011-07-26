#ifndef	GH_WIGLOADER_H
#define	GH_WIGLOADER_H	1

#include <GhCore.h>
#include <ghGuiDac.h>
#include <ghWig.h>

namespace	GH
{
class	GH_DLL_EXPORT WigLoader	:	public GH::Errorprone
{
public:
	//WigLoader();

	bool	load( const QVariant& param,
		 QList<WigSet>& data, const bool& isGui );

		//
		//	wrapper to WigSet::load()
		// 	 - not actually used in load
		//	  as we use QtConcurrent::map() on a QList<WigSet>
		//	 - in fact, not sure how to implement it
		//
	//bool	loadOneSidPerFile( const QString& path );

		//
		//	this used to be called WigSetList_reader
		//
	QList<WigSet>	loadMultiSidPerFile( const QString& path );
};
}	//	GH namespace
#endif	//	GH_WIGLOADER_H
