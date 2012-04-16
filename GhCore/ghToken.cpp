#include "ghToken.h"

namespace	GH
{
	Token::Token( const Separator& sep )
	 : Lexer( sep )
{
/*
	_sep = sep;
	switch( _sep ) {
		case	Tab:
			_ss = "\t";
			break;
		case	Space:
			_ss = " ";
			break;
		case	WhiteSpace:
			_ss = "\\s+";
			break;
		case	Comma:
			_ss = ",";
			break;
		case	SemiColon:
			_ss = ";";
			break;
		case	Colon:
			_ss = ":";
			break;
		default:
			_ss = "";
			break;
	}
*/
}
	Token::Token( const QString& sep )
	 : Lexer( sep )
{
/*
	_sep = Custom;
	_ss = sep;
*/
}
bool	Token::next( Ifp& fp )
{
	if( nextLine( fp, _line ) ) {
		split( _line );
		return( true );
	}
	return( false );
}
Token	Token::split( const QString& s )
{
	clear();
	_line = s;
	if( _ss.startsWith( '\\' ) ) {
		(*this) << _line.split( QRegExp( _ss ), _splitBehavior );
	} else {
		(*this) << _line.split( _ss, _splitBehavior );
	}
	return( *this );
}
/*
bool	Token::isComment() const
{
	return( comment( _line ) );
}
const	QString& Token::line() const
{
	return( _line );
}
void	Token::attachHeader( const QStringList& hdr )
{
	_hdr = hdr;
}
*/
QString Token::operator[]( const int& hidx ) const
{
	if( hidx >= 0 && hidx < size() ) {
		return( at( hidx ) );
	}
	return( QString() );
}
QString	Token::operator[]( const QString& hkey ) const
{
	if( !_hdr.isEmpty() ) {
		return( at( _hdr.indexOf( hkey ) ) );
	}
	return( QString() );
}
Token	Token::load( const QString& path )
{
	Ifp	fp;
	Token	rv;
	QString	line;

	if( !fp.open( path ) ) {
		return( rv );
	}
	while( nextLine( fp, line ) ) {
		rv << line;
	}
	fp.close();
	return( rv );
}
Token	Token::loadHeaderResource( const QString& type )
{
	QString	resourcePath = QString( ":/headers/%1.hdr" ).arg( type );
	return( Token::load( resourcePath ) );
}
//
//	This was inline in ghToken.h, moved here until find its purpose
//
bool	CollectTokenList( Ifp& fp, Token& tok, const QString& key,
	 QString& lastKey, QList<Token>& tab )
{
	tab.clear();
	while( tok.next( fp ) ) {
		if( tok.isComment() ) {
			continue;
		}
		if( lastKey == USTR ) {
			lastKey = tok[ key ];
		}
		if( tok[ key ] != lastKey ) {
			fp.back();
			break;
		}
		tab << tok;
		lastKey = tok[ key ];
	}
	lastKey = USTR;
	if( tab.size() > 0 ) {
		return( true );
	}
	return( false );
	//
	//	old Note: try tok.collectTokenList( fp, key, lastKey, tab )
}

}	//	GH namespace
