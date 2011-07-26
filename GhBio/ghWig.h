#ifndef	GH_WIG_H
#define	GH_WIG_H	1
#include "GhCore.h"
#include "ghBioStrUtils.h"

	// File contains:
	//	Wig
	//	WigList
	//	TrackInfo
	//	WigInfo
	//	WigSet
	//	WigSetList

namespace	GH
{
	//
	//	set of file types that can be coerced to wig
	//
const	QStringList	WIG_SUFFIXES = QStringList()
			 << "wig" << "vstep" << "sgr"
			 << "txt" << "ghd" << "eland";
const	QString		WIG_SUFFIX =
			 "Data (*.wig *.vstep *.sgr *.txt *.ghd *.eland)";

class	GH_DLL_EXPORT	Wig
{
public:
		Wig();
	void	init();

	void	parse( const QString& line,
		 QString* sid = 0, int* step = 0 );
	void	parse( const QStringList& tok,
		 QString* sid = 0, int* step = 0 );
static	bool	validHeader( const QStringList& hdr,
		 int* sid, int* beg, int* val );
	
	void	show( QTextStream& fp, const QString& sid = "" ) const;
	void	showSgr( QTextStream& fp, const QString& sid ) const;
	void	showBed( QTextStream& fp, const QString& sid, const int& len,
		 const QString& bedName, const QString& bedStrand ) const;

	// data members

	int	beg;
	double	val;
};

typedef	QList<Wig>	WigList;

GH_DLL_EXPORT	bool	WigSort( const Wig& e1, const Wig& e2 );

GH_DLL_EXPORT	QList<double>	WigSubsetValues( const QList<Wig>& data,
		 const int& lo, const int& hi,
		 const int& startIndex = 0 );

	//
	//	WigSet inherits WigInfo and has a TrackInfo
	//
const	QString	UCSC_TRACK_HEADER =
	"type,name,description,visibility,color,itemRgb,colorByStrand,"
	"useScore,group,priority,db,offset,url,htmlUrl";

class	GH_DLL_EXPORT	TrackInfo	:	public QMap<QString,QVariant>
{
public:
	TrackInfo( const QString& s = "" );

	void	init( const QString& s = "" );

	QString	type() const;
	void	setDefaultType( const QString& s );
private:
	QStringList	hdr;
	QString		_defaultType;
};
	//
	//	WigInfo is the variableStep or fixedStep line
	//
const	QString	WIG_STEP_HEADER = "chrom,span,start,step";


/*
 *	need a way to construct the WigInfo from SGR input
 */

class	GH_DLL_EXPORT	WigInfo
{
public:
	WigInfo( const QString& s = "" );

	void	init( const QString& s = "" );

	QString	sid;
	int	span, start, step;

	bool	isVariable() const;
	bool	isFixed() const;
private:
	bool	_variableFlag;
};

	//
	//	WigInfo needs to be '_' hidden
	//	 multiple inheritance, a no-no or not??
	//
	//	The DacBase exists to get _params into map and mapped calls
	//
class	GH_DLL_EXPORT	WigSet	:	public	WigInfo, public DacBase
{
public:
	WigSet( const QString& s = "" );

	void	init( const QString& s = "" );

		// this version compatible with mapped()
		//
	bool	load( const QString& path );

		// this version compatible with map()
		//
	bool	load();

	void	setBedFields( const QString& name = ".",
		 const QString& strand = "." );

	void	showSgr( QTextStream& fp ) const;
	void	showWig( QTextStream& fp ) const;
	void	showBed( QTextStream& fp ) const;
	void	fixStep();
	void	smooth();
	void	write();

	WigList		data;
	int		size() const;

	TrackInfo	trackInfo;
private:
	QString	bedName, bedStrand;
};

GH_DLL_EXPORT bool	WigSetSort( const WigSet& e1, const WigSet& e2 );

GH_DLL_EXPORT WigList	WigListSmooth( const WigList& data,
			 const int& dataSize, // imager->xspace
			 const int& windowSize, // sws param
			 const int& minPosition, // axes.xmin
			 const int& incPosition // xincr
			);

typedef	QList<WigSet>	WigSetList;

/*
 * under deprecations
 */
GH_DLL_EXPORT WigSet		WigSet_reader( const QString& path );
GH_DLL_EXPORT WigSetList	WigSetList_reader( const QString& path );
}	//	GH namespace
#endif	//	GH_WIG_H
