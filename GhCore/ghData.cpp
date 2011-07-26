#include "ghData.h"

namespace	GH
{
QList<QVariant>	CollectColumn( const QString& colName, const QList<Row>& data )
{
	QList<QVariant>	rv;

	for( int i = 0; i < data.size(); ++i ) {
		rv << data.at( i )[ colName ];
	}
	return( rv );
}
bool	LoadData( Ifp& fp, QStringList& hdr,
		 QList<Row>& data )
{
	Token	_hdr;
	Row	row;

	data.clear();

	_hdr.next( fp );
	hdr = _hdr;
	row.attachHeader( hdr );
	while( row.next( fp ) ) {
		data << row;
	}
	return( true );
}
bool	ReadData( const QString& path, QStringList& hdr,
		 QList<Row>& data )
{
	bool	rv = false;
	Ifp	fp;

	if( !fp.open( path ) ) {
		return( false );
	}
	rv = LoadData( fp, hdr, data );
	fp.close();
	return( rv );
}
bool	ShowData( Ofp& fp, const QStringList& hdr, const QList<Row>& data )
{
	for( int i = 0; i < hdr.size(); ++i ) {
		if( i != 0 ) {
			fp << "\t";
		}
		fp << hdr.at( i );
	}
	fp << endl;
	for( int r = 0; r < data.size(); ++r ) {
		for( int i = 0; i < hdr.size(); ++i ) {
			if( i != 0 ) {
				fp << "\t";
			}
			fp << S( data.at( r )[ hdr.at( i ) ] );
		}
		fp << endl;
	}
	return( true );
}
bool	WriteData( const QString& path, const QStringList& hdr,
	 const QList<Row>& data )
{
	bool	rv = false;
	Ofp	fp;

	if( !fp.open( path ) ) {
		return( false );
	}
	rv = ShowData( fp, hdr, data );
	fp.close();
	return( rv );
}
void	TransposeData( QStringList& hdr, QList<Row>& data )
{
	QStringList	_hdr = hdr;
	QList<Row>	_data = data;
	
	hdr.clear();
	data.clear();
	
	// repopulate header
	hdr << _hdr.at(0); // ID
	foreach ( Row row, _data ) {
		hdr << S( row.value( _hdr.at(0) ) );
	}
	
	// repopulate rows
	// start at col 1 because col 0 becomes the new header
	for ( int i = 1; i < _hdr.size(); i++ ) {
		QString colName = _hdr.at(i);
		
		Row	row;
		row.insert( hdr.at(0), colName );
		for ( int j = 0; j < _data.size(); j++ ) {
			row.insert( hdr.at(j+1), _data.at(j).value(colName) );
		}
		data << row;
	}
}
}	//	GH namespace
