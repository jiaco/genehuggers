#ifndef	GH_ROW_H
#define	GH_ROW_H	1
#include "ghLexer.h"

namespace GH
{
class	GH_DLL_EXPORT	Row	: public QMap<QString,QVariant>, public Lexer
{
public:
	Row( const Separator& sep = Tab );
	Row( const QString& sep );

	bool	next( Ifp& fp );
	Row	split( const QString& s, bool* okay = 0 );
/*
	QVariant operator[]( const QString& hkey ) const;
	QVariant operator[]( const QString& hkey );
*/
	QString	join( const QString& sep ) const;

	Row&	absorb( const Row& row );
};
}	//	GH namespace
#endif	//	GH_ROW_H
