#ifndef	GH_BIOSTRUTILS_H
#define	GH_BIOSTRUTILS_H	1
#include <GhCore.h>

namespace	GH
{

GH_DLL_EXPORT	int	SidSplit( const QString& sid, QString* org = 0, const int& max = 22 );
GH_DLL_EXPORT	int	SidSplit( const QVariant& sid, QString* org = 0 );
GH_DLL_EXPORT	bool	SortSidByChr( const QString& e1, const QString& e2 );
GH_DLL_EXPORT	QString	SnarfSid( const QString& s );
GH_DLL_EXPORT	QString	Strand( const int& aux );
GH_DLL_EXPORT	int	StrandToAux( const QString& var );




template<class T>
int	SidSetIndex( const QList<T>& data, const QString& sid )
{
	for( int i = 0; i < data.size(); ++i ) {
		if( sid == data.at( i ).sid ) {
			return( i );
		}
	}
	return( UINT );
}

}	//	GH namespace
#endif	//	GH_BIOSTRUTILS_H
