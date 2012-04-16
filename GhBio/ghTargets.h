#ifndef	GH_TARGETS_H
#define	GH_TARGETS_H	1
#include <GhCore.h>
namespace	GH
{
/*
class	QiimeMap	{
public:
	bool	read( QTextStream& fp );
	boool	load( const QString& path );
};
class	MothurOligos	{
public:
	bool	read( QTextStream& fp );
	boool	load( const QString& path );
};
*/
class	GH_DLL_EXPORT	Targets	{
public:
	enum	Format	{
		ND, Text, Mothur, Qiime
	};
	void	init();
	bool	read( Ifp& fp );
	bool	load( const QString& path );

	void	parseTokensMothur( Token& tok );
	void	parseTokensQiime( Token& tok );
	void	parseTokensText( Token& tok );
	void	show( QTextStream& fp, const Format& format = Targets::ND );

	void	showMothur( QTextStream& fp );
	void	showQiime( QTextStream& fp );

	Format	_format;
	QStringList	_hdr;
	QList<Row>	_data;
	Row		_row;
	QString		_fwd, _fwdLabel;
	QString		_rev, _revLabel;
/*
	bool		fromQiimeMap( const QiimeMap& qiimeMap );
	bool		fromMothurOligos( const MothurOligos& mothurOligos );
	QiimeMap	toQiimeMap();
	MothurOligos	toMothurOligos();
*/
	bool		changeHeader(
			 const QString& inCol,
			 const QString& outCol, QStringList& hdr );
};

}	//	GH namespace
#endif	//	GH_TARGETS_H
