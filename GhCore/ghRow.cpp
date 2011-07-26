#include "ghRow.h"

namespace	GH
{
	Row::Row( const Separator& sep )
	 : Lexer( sep )
{
}
	Row::Row( const QString& sep )
	 : Lexer( sep )
{
}
bool	Row::next( Ifp& fp )
{
	bool	rv = false;
	if( nextLine( fp, _line ) ) {
		 split( _line, &rv );
	}
	return( rv );
}
Row	Row::split( const QString& s, bool* okay )
{
	int		maxTokens;
	QStringList	tok;

	_line = s;
	if( _ss.startsWith( '\\' ) ) {
		tok = _line.split( QRegExp( _ss ), _splitBehavior );
	} else {
		tok = _line.split( _ss, _splitBehavior );
	}

	if( okay ) {
		*okay = true;
	}
	maxTokens = _hdr.size();
	if( emptyParts() ) {
		maxTokens = qMin( tok.size(), _hdr.size() );
	} else if( tok.size() != _hdr.size() ) {
		if( okay ) {
			*okay = false;
		}
		maxTokens = qMin( tok.size(), _hdr.size() );
	}
	for( int i = 0; i < maxTokens; ++i ) {
		(*this).insert( _hdr[ i ], tok[ i ] );
	}
	return( *this );
}
/*
QVariant	Row::operator[]( const QString& hkey ) const
{
	if( !_hdr.isEmpty() && _hdr.contains( hkey ) ) {
		return( QMap<QString,QVariant>::operator[]( hkey ) );
		//return( (*this)[ hkey ] );
	}
	return( QVariant() );
}
QVariant	Row::operator[]( const QString& hkey )
{
	return( QMap<QString,QVariant>::operator[]( hkey ) );
	if( !_hdr.isEmpty() && _hdr.contains( hkey ) ) {
		return( QMap<QString,QVariant>::operator[]( hkey ) );
		//return( (*this)[ hkey ] );
	}
	return( QVariant() );
}
*/
QString	Row::join( const QString& sep ) const
{
	QString	rv;

	if( _hdr.size() > 0 ) {
		rv = S( value( _hdr.at( 0 ) ) );
	}
	for( int i = 1; i < _hdr.size(); ++i ) {
		rv += sep;
		rv += S( value( _hdr.at( i ) ) );
	}
	return( rv );
}
Row&	Row::absorb( const Row& row )
{
	foreach( QString s, row.keys() ) {
		if( !(*this).contains( s ) ) {
			(*this).insert( s, row[ s ] );
		}
	}
	return( (*this) );
}
}	//	GH namespace
