#include "ghPhyloUtils.h"

namespace	GH
{
bool	UniqCountSort( const UniqCount& e1, const UniqCount& e2 )
{
	if( e1.cnt == e2.cnt ) {
		return( e1.s < e2.s );
	} else {
		return( e1.cnt > e2.cnt );
	}
}
QList<UniqCount>	SortUniqCount( const QStringList& input )
{
	QList<UniqCount>	rv;
	UniqCount		uc;

	QStringList	tab = input;
	if( tab.isEmpty() ) {
		return( rv );
	}
	qSort( tab.begin(), tab.end() );

	uc.s = tab.at( 0 );
	uc.cnt = 1;
	for( int i = 1; i < tab.size(); ++i ) {
		if( tab.at( i ) == uc.s ) {
			++uc.cnt;
		} else {
			rv << uc;
			uc.s = tab.at( i );
			uc.cnt = 1;
		}
	}
	if( uc.cnt > 0 ) {
		rv << uc;
	}
	qSort( rv.begin(), rv.end(), UniqCountSort );
	return( rv );
}
QString	BasicTaxonomy( const QString& input )
{
	QString rv = input;
	rv = rv.remove( QRegExp( "\\(\\d*\\)" ) );
	rv = rv.remove( "unclassified;" );
	return( rv );
}
QHash<QString,int>	SingleHash( const QString& path,
			 const int& keyTok,
			 const QString& sep, const int& skipLine )
{
	Ifp	fp;
	Token	tok;
	QHash<QString,int>	rv;

	if( !fp.open( path ) ) {
		return( rv );
	}
	tok.setSeparator( sep );
	for( int nLine = 0; tok.next( fp ); ++nLine ) {
		if( nLine < skipLine ) {
			continue;
		}
		rv.insert( tok.at( keyTok ), 1 );
	}
	fp.close();
	return( rv );
}
QHash<QString,QString>	DoubleHash( const QString& path,
			 const int& keyTok, const int& valTok,
			 const QString& sep, const int& skipLine )
{
	Ifp	fp;
	Token	tok;
	QHash<QString,QString>	rv;

	if( !fp.open( path ) ) {
		return( rv );
	}
	tok.setSeparator( sep );
	for( int nLine = 0; tok.next( fp ); ++nLine ) {
		if( nLine < skipLine ) {
			continue;
		}
		rv.insert( tok.at( keyTok ), tok.at( valTok ) );
	}
	fp.close();
	return( rv );
}
bool	LoadMothurTax( const QString& path,
	 QStringList& tax )
{
	Ifp	fp;
	Token	tok;

	tax.clear();
	if( !fp.open( path ) ) {
		return( false );
	}
	// skip 1 line header
	tok.next( fp );

	while( tok.next( fp ) ) {
		tax << tok.at( 2 );
	}
	fp.close();
	return( true );
}
bool	LoadMothurNames2( const QString& path, QHash<QString,QString>& names )
{
	Ifp	fp;
	Token	tok;
	QStringList	stab;

	if( !fp.open( path ) ) {
		return( false );
	}
	while( tok.next( fp ) ) {
		stab = tok.at( 1 ).split( "," );
		for( int i = 1; i < stab.size(); ++i ) {
			names.insert( stab.at( i ), tok.at( 0 ) );
		}
	}
	fp.close();
	return( true );
}
bool	LoadMothurNames( const QString& path, 
	 QHash<QString,int>& primaryIds,
	 QHash<QString,QString>& secondaryIds,
	 const bool& secToPri )
{
	Ifp	fp;
	Token	tok;
	QStringList	stab;

	if( !fp.open( path ) ) {
		return( false );
	}
	while( tok.next( fp ) ) {
		stab = tok.at( 1 ).split( "," );
		primaryIds.insert( tok.at( 0 ), stab.size() );
		if( secToPri ) {
			for( int i = 1; i < stab.size(); ++i ) {
				secondaryIds.insert( stab.at( i ), tok.at( 0 ) );
			}
		} else {
			for( int i = 1; i < stab.size(); ++i ) {
				secondaryIds.insert( tok.at( 0 ), stab.at( i ) );
			}
		}
	}
	fp.close();
	return( true );
}
bool	LoadMothurGroups( const QString& path,
	 QHash<QString,QString>& groups )
{
	groups = DoubleHash( path );
	if( groups.isEmpty() ) {
		return( false );
	}
	return( true );
}
bool	LoadFastaIds( const QString& path, QStringList& ids )
{
	Ifp	fp;
	QString	line;
	if( !fp.open( path ) ) {
		return( false );
	}
	while( nextLine( fp, line ) ) {
		if( !line.startsWith( '>' ) ) {
			continue;
		}
		ids << line.mid( 1 );
	}
	fp.close();
	return( true );
}
bool	ListLineSort( const QStringList& e1, const QStringList& e2 )
{
	return( e1.size() > e2.size() );
}
QList<QStringList>	ParseOtuList( const Token& tok )
{
	QList<QStringList>	rv;

	for( int i = 2; i < tok.size(); ++i ) {
		QStringList t = tok.at( i ).split( "," );
		rv << t;
	}
	//qSort( rv.begin(), rv.end(), ListLineSort );
	return( rv );
}
QList<QStringList>	FilterOtuList( QList<QStringList>& otus, const int& minSize )
{
	for( int i = otus.size() - 1; i >= 0; --i ) {
		if( otus.at( i ).size() < minSize ) {
			otus.removeAt( i );
		}
	}
	return( otus );
}
/*
QList<int>	CountGroupOtus( const QList<QStringList>& listLine,
		 const QHash<QString,QString>& groups, const QStringList& groupNames )
{
	QList<int>	rv;
	for( int i = 0; i < groupNames.size(); ++i ) {
		rv << 0;
	}
	for( int i = 0; i < listLine.size(); ++i ) {
		for( int j = 0; j < listLine.at( i ).size(); ++j ) {
			if( groups.contains( listLine.at( i ).at( j ) ) ) {
				++rv[ groupNames.indexOf( groups[ listLine.at( i ).at( j ) ] ) ];
			}
		}
	}
	return( rv );
}
QList<int>	CountGroupReads( const QList<QStringList>& listLine,
		 const QHash<QString,QString>& groups, const QStringList& groupNames )
{
	QList<int>	rv;
	for( int i = 0; i < groupNames.size(); ++i ) {
		rv << 0;
	}
	for( int i = 0; i < listLine.size(); ++i ) {
		for( int j = 0; j < listLine.at( i ).size(); ++j ) {
			if( groups.contains( listLine.at( i ).at( j ) ) ) {
				rv[ groupNames.indexOf( groups[ listLine.at( i ).at( j ) ] ) ] += otuList.at(i );
			}
		}
	}
	return( rv );
}
*/
}	//	GH namespace
