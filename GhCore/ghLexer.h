#ifndef	GH_LEXER_H
#define	GH_LEXER_H	1
#include "ghDef.h"
#include "ghIfp.h"
#include "ghOfp.h"
#include "ghStrUtils.h"

namespace	GH
{

class	GH_DLL_EXPORT	Lexer
{
public:
	enum	Separator {
		Tab, Space, WhiteSpace, Comma, SemiColon, Colon, Custom
	};
	Lexer( const Separator& sep = Tab );
	Lexer( const QString& sep );

	void	setSeparator( const QString& sep );
	
	const QString&	line() const;
	bool		isComment() const;
	void		attachHeader( const QStringList& hdr );
	const QStringList	hdr() const;
	QString		hdrAt( const int& idx ) const;
	//int		size() const;

	bool	emptyParts() const;
	void	setEmptyParts( const bool& tf = true );
/*
	bool	next( Ifp& fp );
	Token	split( const QString& s );
	QString	operator[]( const QString& hkey ) const;

	static	Token	load( const QString& path );
	static	Token	loadHeaderResource( const QString& type );
*/
protected:
	Separator	_sep;
	QString		_ss;
	QString		_line;
	QStringList	_hdr;

	QString::SplitBehavior	_splitBehavior;
};

}	//	GH namespace
#endif	//	GH_LEXER_H
