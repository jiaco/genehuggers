#ifndef	GH_STRUTILS_H
#define	GH_STRUTILS_H	1
#include <QDir>
#include <QFileInfo>
#include "ghDef.h"
#include "ghVariant.h"

namespace	GH
{
GH_DLL_EXPORT	QStringList	SafeStringList( const QVariant& v );

GH_DLL_EXPORT	QString	ChompSuffix( const QString& s );
GH_DLL_EXPORT	QString	ChompSlash( const QString& s );
GH_DLL_EXPORT	QString FileSuffix( const QString& s );
GH_DLL_EXPORT	QString FileSuffix( const QStringList& slist );
GH_DLL_EXPORT	QString BasePath( const QString& s );
GH_DLL_EXPORT	QString BaseName( const QString& s );
GH_DLL_EXPORT	QString FileName( const QString& s );

GH_DLL_EXPORT	QString	StripHTML( const QString& s );
GH_DLL_EXPORT	QString	SnarfBetween( const QString& s, const QChar& lo, const QChar& hi );

GH_DLL_EXPORT	void	Recurse( const QString& path, QStringList& data );
GH_DLL_EXPORT	QStringList	Scan( const QString& path,
				 const QStringList& suffixList );

GH_DLL_EXPORT	bool	FindPath( QString& path );

GH_DLL_EXPORT	void	ParseNameValuePairs( const QString& buf,
			 const QStringList& hdr,
			 QMap<QString,QVariant>& data,
			 const int& firstOffset = 1 );

GH_DLL_EXPORT	QList<int>	Deblob( const QString& s );
GH_DLL_EXPORT	QString		Reblob( const QList<int>& tab );

GH_DLL_EXPORT	QList<int>	StringIndexAll( const QString& q, const QString& s );

inline	bool	comment( const QString& s )
{
	if( s.startsWith( '#' ) ) {
		return( true );
	}
	return( false );
}
inline	QString	DecodePercent( const QString& s, const QString& newlines="" )
{
	QString	t = s;
	if( !newlines.isEmpty() ) {
		t = t.replace( "%0A", newlines, Qt::CaseInsensitive );
	}
	return( QUrl::fromPercentEncoding( t.toAscii() ) );
}
inline	QMultiMap<QString,QString>	GTFMap( const QString& s )
{
	QMultiMap<QString,QString>      rv;
	QStringList     tok;
	QString key, value;
	int     kb, ke, nb, ne;

        // split on ';' to get number of attributes
        // then look for '=' OR ' ' and optional '"'

        // name value;[ ]name value;name "value";name=value; name="value ha";

	tok = s.split( ';' );
	foreach( QString attr, tok ) {
		attr = attr.trimmed();
		kb = 0;
		if( ( ke = attr.indexOf( '=' ) ) == -1 ) {
			ke = attr.indexOf( ' ' );
		}
		if( ke == -1 ) {
			continue;
		}
		nb = ke + 1;
		ne = attr.size();
		if( attr.count( '"' ) == 2 ) {
			++nb;
			--ne;
		}
		key = attr.mid( kb, ke - kb );
		value = attr.mid( nb, ne - nb );

		rv.insert( key, value );
		if( key == "db_xref" && value.startsWith( "taxon" ) ) {
			QStringList t = value.split( ":" );
			rv.insert( "taxid", t.at( 1 ) );
		}
	}
	return( rv );
}
}	//	GH namespace
#endif	//	GH_STRUTILS_H
