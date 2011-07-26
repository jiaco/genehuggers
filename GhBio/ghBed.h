#ifndef	GH_BED_H
#define	GH_BED_H	1
#include "ghLocTypes.h"
#include "ghBioStrUtils.h"

namespace	GH
{
const   QString BED_SUFFIX = "Data (*.bed *.txt *.ghd)";

	//	Sub is a sublocation within a Bed
class	GH_DLL_EXPORT	SubBed
{
public:
	SubBed( const QString& starts, const QString& ends );

	void	parse( const QString& starts, const QString& ends );
	QList<int>	beg;
	QList<int>	end;
};
        //      Bed is a SLoc with a label
        //
class   GH_DLL_EXPORT   Bed     :       public SLoc
{
public:
                Bed();
        void    init();

	void	showBed( QTextStream& fp ) const;
	void	showWig( QTextStream& fp ) const;
	void	showSgr( QTextStream& fp ) const;

static  QMap<QString,int>       defaultHeader();
static  bool                    isValidHeader( const QStringList& hdr,
				 QMap<QString,int>& idx );
	//	data members
	//
        QString label;
	SubBed	*sub;
};

typedef QList<Bed>      BedList;

GH_DLL_EXPORT   bool    BedSort( const Bed& e1, const Bed& e2 );

        //      WZone is a working-zone has b & e indexes into a WigList
        //
class   GH_DLL_EXPORT   WZone   :       public  BLoc
{
public:
	//	data members
	//
        int     b, e;
};

typedef QList<WZone>    WZoneList;

class   GH_DLL_EXPORT   BedSet	: public DacBase
{
public:
                BedSet();
        void    init();

		// this version compatible with mapped()
		//
	bool	load( const QString& path );

		// this version compatible with map()
		//
	bool	load();


	void	showBed( QTextStream& fp ) const;
	void	showWig( QTextStream& fp ) const;
	void	showSgr( QTextStream& fp ) const;
	void	write() const;

	//	data members
	//
        QString sid;
        BedList         data;
};

typedef QList<BedSet>   BedSetList;

GH_DLL_EXPORT bool	BedSetSort( const BedSet& e1, const BedSet& e2 );

GH_DLL_EXPORT Bed     Bed_parser( const QString& line );
GH_DLL_EXPORT BedSet		BedSet_reader( const QString& path );
GH_DLL_EXPORT BedSetList	BedSetList_reader( const QString& path );

GH_DLL_EXPORT BedList BedList_fromWZoneList( const WZoneList& data );

}	//	GH namespace
#endif	//	GH_BED_H
