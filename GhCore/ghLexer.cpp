#include "ghLexer.h"

namespace	GH
{
	Lexer::Lexer( const Separator& sep )
{
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
	setEmptyParts( false );
}
	Lexer::Lexer( const QString& sep )
{
	_sep = Custom;
	_ss = sep;
	setEmptyParts( false );
}
void	Lexer::setSeparator( const QString& sep )
{
	_sep = Custom;
	_ss = sep;
}
const	QString& Lexer::line() const
{
	return( _line );
}
bool	Lexer::isComment() const
{
	return( comment( _line ) );
}
/*
 * subclasses may have this method

int	Lexer::size() const
{
	return( 
}
*/
void	Lexer::attachHeader( const QStringList& hdr )
{
	_hdr = hdr;
}
const QStringList	Lexer::hdr() const
{
	return( _hdr );
}
QString	Lexer::hdrAt( const int& idx ) const
{
	if( idx >= 0 && idx < _hdr.size() ) {
		return( _hdr.at( idx ) );
	}
	return( QString() );
}
bool	Lexer::emptyParts() const
{
	if( _splitBehavior == QString::KeepEmptyParts ) {
		return( true );
	} else {
		return( false );
	}
}
void	Lexer::setEmptyParts( const bool& tf )
{
	if( tf ) {
		_splitBehavior = QString::KeepEmptyParts;
	} else {
		_splitBehavior = QString::SkipEmptyParts;
	}
}

/*



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
		(*this) << _line.split( QRegExp( _ss ) );
	} else {
		(*this) << _line.split( _ss );
	}
	return( *this );
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
*/
}	//	GH namespace
