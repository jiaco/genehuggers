#ifndef	GH_PHYLOUTILS_H
#define	GH_PHYLOUTILS_H	1
#include "GhCore.h"
namespace	GH
{
class	UniqCount
{
public:
	QString	s;
	int	cnt;
};
QList<UniqCount>	SortUniqCount( const QStringList& input );
QString			BasicTaxonomy( const QString& input );

QHash<QString,int>	SingleHash( const QString& path,
			 const int& keyTok = 0,
			 const QString& sep = "\t", const int& skipLine = 0 );
QHash<QString,QString>	DoubleHash( const QString& path,
			 const int& keyTok = 0, const int& valTok = 1,
			 const QString& sep = "\t", const int& skipLine = 0 );

	/* Mothur names file:
		primary [TAB] primary,secondary1,secondary2,...
	
	 1) load a QHash<QString<QStringList> primary->secondary-list
	 2) load a QHash<QString,QString> secondary->primary
	*/
bool	LoadMothurNames2( const QString& path, QHash<QString,QString>& names );

bool	LoadMothurNames( const QString& path,
	 QHash<QString,int>& primaryIds,
	 QHash<QString,QString>& secondaryIds, const bool& secToPri = false );
bool	LoadMothurGroups( const QString& path,
	 QHash<QString,QString>& groups );
bool	LoadMothurTax( const QString& path, QStringList& tax );

bool	LoadFastaIds( const QString& path, QStringList& ids );

QList<QStringList>	ParseOtuList( const Token& tok );
QList<QStringList>	FilterOtuList( QList<QStringList>& otus, const int& minSize = 2 );
/*
QList<int>		CountGroupOtus( const QList<QStringList>& listLine,
			 const QHash<QString,QString>& groups, const QStringList& groupNames );
QList<int>		CountGroupReads( const QList<QStringList>& listLine,
			 const QHash<QString,QString>& groups, const QStringList& groupNames );
*/
}	//	GH namespace
#endif	//	GH_PHYLOUTILS_H
