#include "ghStrUtils.h"

namespace	GH
{
QStringList     SafeStringList( const QVariant& v )
{
	//
	//	This is redundant with SL() in ghVariant
	//
	QStringList     rv;

	if( v.type() == QVariant::String ) {
		rv << v.toString();
	} else if( v.type() == QVariant::StringList ) {
		rv = v.toStringList();
	}
	return( rv );
}

bool	FindPath( QString& path )
{
	QString target = path;
	QString	ghHome;
	QStringList	fileList;

	if( ( ghHome = HOME() ) == USTR ) {
		return( false );
	}
	path = QString( "%1/headers" ).arg( ghHome );
	Recurse( path, fileList );
	foreach( QString f, fileList ) {
		if( FileName( f ) == target ) {
			path = f;
			return( true );
		}
	}
	return( false );
}
QList<int>	StringIndexAll( const QString& q, const QString& s )
{
	QList<int>	rv;

	int	pos = 0;
	int	i = 0;
	while( ( i = s.indexOf( q, pos ) ) != UINT ) {
		rv << i;
		pos = i + 1;
	}
	return( rv );
}
QString	StripHTML( const QString& s )
{
	QString	rv = "";
	bool	inTag = false;
	foreach( QChar ch, s ) {
		if( inTag ) {
			if( ch == '>' ) {
				inTag = false;
			}
		} else if( ch == '<' ) {
			inTag = true;
		} else {
			rv += ch;
		}
	}
	return( rv );
}
QString	ChompSuffix( const QString& s )
{
	int	lastDot;

	if( ( lastDot = s.lastIndexOf( '.' ) ) != UINT ) {
		return( s.left( lastDot ) );
	}
	return( s );
}
QString	ChompSlash( const QString& s )
{
	if( s.endsWith( '/' ) ) {
		return( s.left( s.size() - 1 ) );
	} else {
		return( s );
	}
}
QString FileSuffix( const QString& s )
{
	int	lastDot;
	if( ( lastDot = s.lastIndexOf( '.' ) ) != UINT ) {
		return( s.right( s.size() - ( lastDot + 1 ) ) );
	}
	return( s );
}
QString FileSuffix( const QStringList& slist )
{
	QString rv = "";

	foreach( QString s, slist ) {
		QString suffix = FileSuffix( s );
		if( rv.isEmpty() ) {
			rv = suffix;
			continue;
		} else if( rv != suffix ) {
			rv = "";
			break;
		}
	}
	return( rv );
}
QString BasePath( const QString& s )
{
	int	lastSlash;
	if( ( lastSlash = s.lastIndexOf( '/' ) ) != UINT ) {
		return( s.left( lastSlash ) );
	} else {
		return( "" );
	}
	// if there is no slash, do not want to return input string
	//return( s );
}
QString BaseName( const QString& s )
{
	int	lastSlash;
	int	lastDot;
	if( ( lastSlash = s.lastIndexOf( '/' ) ) == UINT ) {
		lastSlash = 0;
	} else {
		++lastSlash;
	}
	if( ( lastDot = s.lastIndexOf( '.' ) ) == UINT ) {
		return( s.left( lastSlash ) );
	} else {
		return( s.mid( lastSlash, lastDot - lastSlash ) );
	}
}
QString FileName( const QString& s )
{
	int	lastSlash;

	if( ( lastSlash = s.lastIndexOf( '/' ) ) != UINT ) {
		return( s.right( s.size() - ( lastSlash + 1 ) ) );
	}
	return( s );
}
void    Recurse( const QString& path, QStringList& data )
{
	QDir    d( path );
	QList<QFileInfo>        l = d.entryInfoList();

	for( int i = 0; i < l.size(); ++i ) {
		if( l.at( i ).fileName().startsWith( "." ) ) {
			continue;
		}
		if( l.at( i ).isDir() ) {
			Recurse( l.at( i ).absoluteFilePath(), data );
		} else {
			data << l.at( i ).absoluteFilePath();
		}
	}
	return;
}
QStringList     Scan( const QString& path,
		 const QStringList& suffixList )
{
	QStringList     rv;
	QDir    d( path );

	foreach( QFileInfo fi, d.entryInfoList( QDir::Files ) ) {
		if( suffixList.size() == 0 ) {
			rv << fi.absoluteFilePath();
		} else if( suffixList.contains( fi.suffix() ) ) {
			rv << fi.absoluteFilePath();
		}
	}
	return( rv );
}
QString	SnarfBetween( const QString& s, const QChar& lo, const QChar& hi )
{
	QString	rv = "";
	int b, e;

	b = e = 0;
	if( ( b = s.indexOf( lo ) ) == UINT ) {
		return( rv );
	}
	++b;
	if( ( e = s.indexOf( hi, b ) ) == UINT ) {
		return( rv );
	}
	rv = s.mid( b, e - b );
	return( rv );
}
void    ParseNameValuePairs( const QString& buf,
         const QStringList& hdr, QMap<QString,QVariant>& data,
         const int& firstOffset )
{
        foreach( QString s, hdr ) {
                if( data.contains( s ) ) {
                        data[ s ] = QVariant();
                } else {
                        data.insert( s, QVariant() );
                }
        }
        if( buf.isEmpty() ) {
                return;
        }
        QStringList tok = buf.split( QRegExp( "[= ]" ) );
        for( int i = firstOffset; i < tok.size() - 1; i += 2 ) {
                if( data.contains( tok[ i ] ) ) {
                        data[ tok[ i ] ] = QVariant( tok[ i + 1 ] );
                }
        }
}
QList<int>	Deblob( const QString& s )
{
	QList<int>	rv;
	QStringList	tokens = s.split( "," );

	foreach( QString tok, tokens ) {
		rv << tok.toInt();
	}
	return( rv );
}
QString		Reblob( const QList<int>& tab )
{
	QString	rv;

	foreach( int i, tab ) {
		rv += QString( "%1," ).arg( i );
	}
	rv.chop( 1 );
	return( rv );
}

}	//	GH namespace
