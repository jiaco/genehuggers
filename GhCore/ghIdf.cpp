#include "ghIdf.h"

namespace	GH
{
QString	IDF::lookup( const QString& key )
{
	IDF	idf;
	QString	rv = USTR;

	if( !idf.load() ) {
		return( rv );
	}
	return( idf.get( key ) );
}
bool	IDF::install( const QString& key, const QString& value,
	 const bool& overwrite )
{
	IDF	idf;
	bool	rv = false;

	if( !idf.load() ) {
		return( rv );
	}
	if( ( rv = idf.set( key, value, overwrite ) ) ) {
		idf.write();
	}
	return( rv );
}
void	IDF::show( QTextStream& fp )
{
	IDF	idf;

	if( !idf.load() ) {
		return;
	}
	idf.showAll( fp );
	return;
}
bool	IDF::remove( const QString& key )
{
	IDF	idf;
	if( !idf.load() ) {
		return( false );
	}
	if( idf.removeKey( key ) ) {
		idf.write();
		return( true );
	}
	return( false );
}
	IDF::IDF()
{
	_env = _path = USTR;
}
bool	IDF::getPath()
{
	if( _path != USTR ) {
		return( true );
	}
	if( ( _env = GH::IDF() ) == USTR ) {
		return( false );
	}
	_path = _env + "/" + ".gh.idf";
	return( true );
}
bool	IDF::set( const QString& key, const QString& value,
	 const bool& overwrite )
{
	if( _map.contains( key ) ) {
		if( overwrite ) {
			_map[ key ] = value;
		} else {
			return( false );
		}
	} else {
		_map.insert( key, value );
	}
	return( true );
}
QString	IDF::get( const QString& key )
{
	if( _map.contains( key ) ) {
		return( _map[ key ] );
	} else {
		return( USTR );
	}
}
bool	IDF::removeKey( const QString& key )
{
	if( _map.remove( key ) == 0 ) {
		return( false );
	} else {
		return( true );
	}
}
bool	IDF::load()
{
	Ifp	ifp;

	if( !getPath() ) {
		return( false );
	}
	_map.clear();
	if( !ifp.open( _path ) ) {
		if( !write() ) {
			return( false );
		}
		// need to be able to at least write
	}
	QString	line;
	while( ( line = ifp.next() ) != USTR ) {
		QStringList	tok = line.split( "\t" );
		if( tok.size() == 2 ) {
			_map.insert( tok[ 0 ], tok[ 1 ] );
		}
	}
	ifp.close();
	return( true );
}
void	IDF::showAll( QTextStream& fp )
{
	foreach( QString s, _map.keys() ) {
		fp << s << "\t" << _map[ s ] << endl;
	}
}
bool	IDF::write()
{
	Ofp	ofp;

	if( !getPath() ) {
		return( false );
	}
	if( !ofp.open( _path ) ) {
		return( false );
	}
	foreach( QString s, _map.keys() ) {
		ofp << s << "\t" << _map[ s ] << endl;
	}
	ofp.close();
	return( true );

}
}	//	GH namespace
