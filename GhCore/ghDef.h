#ifndef	GH_DEF_H
#define	GH_DEF_H	1
#include <QtCore>
#include <QApplication>
#include <stdlib.h>

#ifdef	WIN32
#	ifdef GH_BUILD_EXPORT
#		define GH_DLL_EXPORT __declspec(dllexport)
#	else
#		define	GH_DLL_EXPORT __declspec(dllimport)
#	endif
#else
#	define	GH_DLL_EXPORT
#endif

namespace	GH	{

	//
	//	main() should only return these defined values
	//
	//	functions that return int as status should be
	//	using 1 as success and 0 as failure...
	//	BUT, most functions return bool now
	//
const	int	CLEAN = 0;	//!< application return value (success)
const	int	ERROR = 1;	//!< application return value (failure)

//inline void GH_MAIN( const QString& styleSheetName = "default" )
inline void GH_MAIN()
{
	Q_INIT_RESOURCE( style );
}
const	QString	GH_ORG_NAME = "GeneHuggers";
const	QString	GH_ORG_DOMAIN = "genehuggers.org";

inline	void	GH_CORE_SETUP( const QString& appName )
{
	QCoreApplication::setOrganizationName( GH_ORG_NAME );
	QCoreApplication::setOrganizationDomain( GH_ORG_DOMAIN );
	QCoreApplication::setApplicationName( appName );
}
const	QApplication::Type	CLI = QApplication::Tty;
	//!< shorthand for QApplication constructor
const	QApplication::Type	GUI = QApplication::GuiClient;
	//!< shorthand for QApplication constructor
const	QApplication::Type	GUI_S = QApplication::GuiServer;
	//!< shorthand for QApplication constructor

const	QString	GH_HOME = "GH_HOME";
const	QString	GH_IDF = "GH_IDF";

const	int		UINT = -1;	//!< integer undefined
const	float		UFLT = -1.0;	//!< float undefined
const	double		UDBL = -1.0;	//!< double undefined
const	QChar		UCHAR = '\0';	//!< QChar undefined
const	QString		USTR = "-";	//!< QString undefined
const	QVariant	UVAR = QVariant( USTR );	//!< QVariant undefined

//
//	This should be implemented for all types:
//	 int/float/double/QChar/QString/QVariant
//
inline	bool	UNDEF( const QString& s )
{
	if( s == USTR || s.isEmpty() ) {
		return( true );
	} else {
		return( false );
	}
}
//
//	MOVE TO GHBIO
//	NoVersion converts String.Whatever to String
//
inline	QString	NoV( const QString& s )
{
	int e = s.indexOf( '.' );
	if( e > 0 ) {
		return( s.mid( 0, e ) );
	} else {
		return( s );
	}
}

//
//	This should be way less (or never) used as we now properly
//	 use the Qt resources system to build things like headers
//	 into the application
//
/*!	\fn HOME
 	\brief	inline function to get GH_HOME environment variable

	\return QString with the environment variable or USTR if not defined

 	Many programs need to get at files inside the source code
 	and therefore need to find the full path to the source 
 	directory. This function uses the system enviornment variable
 	GH_HOME to locate the path. USTR is returned if the variable
 	is not set.
 */
inline QString	HOME()
{
	char	*env;
	env = getenv( GH_HOME.toAscii() );
	if( env == 0 ) {
		return( USTR );
	} else {
		return( QString( env ) );
	}
}
inline	QString	IDF()
{
	char	*env;
	env = getenv( GH_IDF.toAscii() );
	if( env == 0 ) {
		return( USTR );
	} else {
		return( QString( env ) );
	}
}


const	QString	EMSG_ARGS = "unable to parse arguments";
const	QString	EMSG_INPUT = "no input file found";
const	QString	EMSG_HDR = "unable to read input header";

//
//	move to GHBIO
//

const	QString	GH_LOC_SID	= "sid";
const	QString	GH_LOC_BEG	= "beg";
const	QString	GH_LOC_END	= "end";
const	QString	GH_LOC_AUX	= "aux";

const	QString	FTYPE_UTR5	= "UTR5";
const	QString	FTYPE_CDS	= "CDS";
const	QString	FTYPE_UTR3	= "UTR3";

const	QString	HUMAN_CHROMOSOMES =
"1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,X,Y,M";

const	QString	CHR_DIGITS = "0123456789XYM";

inline	QString	GetChr( const QString& s )
{
	int	b, e;

	b = s.indexOf( "chr" );
	if( b == -1 ) {
		b = 0;
	} else {
		b += 3;
	}
	for( e = b; e < s.size(); ++e ) {
		if( !CHR_DIGITS.contains( s.at( e ) ) ) {
			break;
		}
	}
	return( s.mid( b, e - b ) );
}
inline	int	NumericChr( const QString& s )
{
	QStringList	tab = HUMAN_CHROMOSOMES.split( "," );
	QString		chr = GetChr( s );
	if( tab.contains( chr ) ) {
		return( tab.indexOf( chr ) );
	}
	return( -1 );
}

inline  QMap<QString,QVariant>  JASON( const QString& text )
{
	QStringList     pairs, nv;
	QString         name, value;
	QMap<QString,QVariant>  rv;
	
	pairs = text.split( ';', QString::SkipEmptyParts );
	foreach( QString pair, pairs ) {
		if( !pair.contains( '=' ) ) {
			continue;
		}
		int x = pair.indexOf( '=' );
		name = pair.mid( 0, x );
		value = pair.mid( x + 1 );

/*
		nv = pair.split( '=' );
		if( nv.size() != 2 ) {
			continue;
		}
		name = nv.at( 0 );
		value = nv.at( 1 );
*/
		if( name.startsWith( "'" ) && name.endsWith( "'" ) ) {
			name = name.mid( 1, name.size() - 2 );
		}
		if( value.startsWith( "'" ) && value.endsWith( "'" ) ) {
			value = value.mid( 1, value.size() - 2 );
		}
		if( rv.contains( name ) ) {
			rv[ name ] = QVariant( value );
		} else {
			rv.insert( name, QVariant( value ) );
		}
	}
	return( rv );
}
}	//	GH namespace
#endif	//	GH_DEF_H
