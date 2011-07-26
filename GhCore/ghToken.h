#ifndef	GH_TOKEN_H
#define	GH_TOKEN_H	1
#include "ghDef.h"
#include "ghIfp.h"
#include "ghOfp.h"
#include "ghStrUtils.h"
#include "ghLexer.h"

namespace	GH
{

class	GH_DLL_EXPORT	Token	:	public	QStringList, public Lexer
{
public:
	Token( const Separator& sep = Tab );
	Token( const QString& sep );

	bool	next( Ifp& fp );
	Token	split( const QString& s );
	QString	operator[]( const QString& hkey ) const;

	static	Token	load( const QString& path );
	static	Token	loadHeaderResource( const QString& type );
};
bool	CollectTokenList( Ifp& fp, Token& tok, const QString& key,
	 QString& lastKey, QList<Token>& tab );

}	//	GH namespace
#endif	//	GH_TOKEN_H
