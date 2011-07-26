#ifndef	GH_VARIANT_H
#define	GH_VARIANT_H	1
#include <QtCore>
#include <QtGui>
//#include "Qt/qcolor.h"
#include "ghDef.h"

namespace	GH
{
	//
	//	inline functions to convert _to_ QVariant
	//
inline	QVariant	V( const QString& v )
{
	return( QVariant( v ) );
}
inline	QVariant	V( const int& v )
{
	return( QVariant( v ) );
}
inline	QVariant	V( const double& v )
{
	return( QVariant( v ) );
}
inline	QVariant	V( const QStringList& v )
{
	return( QVariant( v ) );
}
inline	QVariant	V( const QMap<QString,QVariant>& v )
{
	return( QVariant( v ) );
}
inline	QVariant	V( const QList<QVariant>& v )
{
	return( QVariant( v ) );
}
	//
	//	inline functions to convert _from_ QVariant
	//
inline	QString	S( const QVariant& v )
{
	return( v.toString() );
}
inline	bool	B( const QVariant& v )
{
	return( v.toBool() );
}
inline	QStringList	SL( const QVariant& v )
{
	QStringList	rv;
	if( v.type() == QVariant::String ) {
		rv << v.toString();
	} else if( v.type() == QVariant::StringList ) {
		rv = v.toStringList();
	}
	return( rv );
}
inline	QColor	CLR( const QVariant& v )
{
	return( v.value<QColor>() );
}
inline	QFont	FONT( const QVariant& v )
{
	return( v.value<QFont>() );
}
inline	QString	FONTSTRING( const QFont& font )
{
	QString rv = QString( "%1,%2,%3" )
		.arg( font.family() ).arg( font.pointSize() )
		.arg( font.weight() );
	if( font.italic() ) {
		rv.append( ",italic" );
	}
	return( rv );
}
inline	QString	V2S( const QVariant& v, const QString& sep = "," )
{
	if( v.type() == QVariant::Color ) {
		return( CLR( v ).name() );
	} else if( v.type() == QVariant::Font ) {
		return( FONTSTRING( FONT( v  )) );
		//return( FONT( v ).toString() );
	} else if( v.type() == QVariant::StringList ) {
		return( SL( v ).join( sep ) );
	} else {
		return( S( v ) );
	}
}
inline	QStringList	V2SL( const QVariant& v, const QString& sep = "" )
{
	QStringList	rv;
	if( v.type() == QVariant::String ) {
		QString s = v.toString();
		if( s.isEmpty() ) {
			return( rv );
		}
		if( !sep.isEmpty() && s.contains( sep ) ) {
			rv = s.split( sep, QString::SkipEmptyParts );
		} else {
			rv << v.toString();
		}
	} else if( v.type() == QVariant::StringList ) {
		rv = v.toStringList();
		if( rv.size() > 0 && rv.at( 0 ).isEmpty() ) {
			return( QStringList() );
		}
	}
	return( rv );
}
inline	QStringList	SplitVariant( const QVariant& v, const QString& sep )
{
	return( S( v ).split( sep, QString::SkipEmptyParts ) );
}
inline	int	I( const QString& v )
{
	bool	ok = false;
	int	rv = v.toInt( &ok );
	if( !ok ) {
		rv = UINT;
	}
	return( rv );
}
inline	int	I( const QVariant& v )
{
	bool	ok = false;
	int	rv = v.toInt( &ok );
	if( !ok ) {
		rv = UINT;
	}
	return( rv );
}
inline	qint64	I64( const QString& v )
{
	bool	ok = false;
	qint64	rv = v.toLongLong( &ok );
	if( !ok ) {
		rv = UINT;
	}
	return( rv );
}
inline	qint64	I64( const QVariant& v )
{
	bool	ok = false;
	qint64	rv = v.toString().toLongLong( &ok );
	if( !ok ) {
		rv = UINT;
	}
	return( rv );
}
inline	double	D( const QString& v )
{
	bool	ok = false;
	double	rv = v.toDouble( &ok );
	if( !ok ) {
		rv = UDBL;
	}
	return( rv );
}
inline	double	D( const QVariant& v )
{
	bool	ok = false;
	double	rv = v.toDouble( &ok );
	if( !ok ) {
		rv = UDBL;
	}
	return( rv );
}

	//
	//	deprecated functions
	//
inline	QVariant	VAR( const QString& v ) { return( V( v ) ); }
inline	QVariant	VARI( const int& v ) { return( V( v ) ); }
inline	QVariant	VARD( const double& v ) { return( V( v ) ); }

inline	QString	STR( const QVariant& v ) { return( S( v ) ); }
inline	int	INT( const QVariant& v ) { return( I( v ) ); }
inline	double	DBL( const QVariant& v ) { return( D( v ) ); }
}	//	GH namespace
#endif	//	GH_VARIANT_H
