#include "ghFast.h"

namespace	GH
{
void	Qual::init()
{
	sid = "";
	clear();
}
void	Qual::show( QTextStream& fp )
{
	fp << ">" << sid << endl;
	int i = 0;
	fp << at( i );
	for( ++i; i < size(); ++i ) {
		fp << " " << at( i );
	}
	fp << endl;
}
bool	nextQual( Ifp& fp, Qual& qual )
{
	QString	line;
	Token	tok( Lexer::WhiteSpace );

	qual.init();

	if( !nextLine( fp, line ) ) {
		return( false );
	}
	if( !line.startsWith( '>' ) ) {
		return( false );
	}
	qual.sid = line.mid( 1 );
	while( tok.next( fp ) ) {
		if( tok.line().startsWith( '>' ) ) {
			fp.back();
			break;
		}
		foreach( QString t, tok ) {
			qual << t.toInt();
		}
	}
/*	changed to handle multi-line qual values
	if( !tok.next( fp ) ) {
		return( false );
	}
	foreach( QString t, tok ) {
		qual << t.toInt();
	}
*/
	return( true );
}
	Fast::Fast( const QString& seq, const QString& sid )
{
	init();
	_seq = seq;
	_sid = sid;
}
void	Fast::init()
{
	_seq = _sid = _idline = QString();
}
void	Fast::setSeq( const QString& seq )
{
	_seq = seq;
}
void	Fast::setSid( const QString& sid )
{
	_sid = sid;
}
QString	Fast::seq() const
{
	return( _seq );
}
QString	Fast::sid() const
{
	return( _sid );
}
QString	Fast::idline() const
{
	return( _idline );
}
bool	Fast::next( Ifp& fp )
{
	bool	okay;
	QString	s;

	init();
	if( !nextLine( fp, _idline ) ) {
		return( false );
	}
	_sid = SnarfSid( _idline, &okay );
	if( !okay ) {
		return( false );
	}
	while( nextLine( fp, s ) ) {
		if( s.startsWith( IdDelimiter() ) ) {
			fp.back();
			break;
		}
		_seq += s;
	}
	//
	//	really do not know if empty seqs should be fail or not...
	//
	if( _seq.size() > 0 ) {
		return( true );
	}
	return( false );
}
bool	Fast::Next( Ifp& fp, Fast& fast )
{
	return( fast.next( fp ) );
}
void	Fast::show( QTextStream& fp ) const
{
	fp << IdDelimiter() << _sid << endl;
	for( int i = 0; i < _seq.size(); i += SeqLineSize ) {
		fp << _seq.mid( i, SeqLineSize ) << endl;
	}
}
void	Fast::Show( QTextStream& fp, const Fast& fast )
{
	fast.show( fp );
}
QChar	Fast::IdDelimiter()
{
	return( QChar( _IdDelimiter ) );
}
QString	Fast::SnarfSid( const QString& idline, bool* okay )
{
	QString	rv;
	QString	endChars = " |";


	if( okay ) {
		*okay = true;
	}

	if( idline.size() > 0 && idline.at( 0 ) == IdDelimiter() ) {
		int end = 1;
		for( ; end < idline.size(); ++end ) {
			if( endChars.contains( idline.at( end ) ) ) {
				--end;
				break;
			}
		}
		rv = idline.mid( 1, end );
		return( rv );
	}
	if( okay ) {
		*okay = false;
	}
	return( rv );
}
}	//	GH namespace
