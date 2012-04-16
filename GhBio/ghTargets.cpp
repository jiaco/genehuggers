#include "ghTargets.h"
namespace	GH
{
void	Targets::init()
{
	_format = Targets::ND;
	_hdr.clear();
	_row.clear();
	_data.clear();
	_fwd = _fwdLabel = "";
	_rev = _revLabel = "";
}
bool	Targets::read( Ifp& fp )
{
	bool	rv = false;
	Token	tok;
	
	init();
	if( !tok.next( fp ) ) {
		return( rv );
	}
	if( !tok.size() >= 2 ) {
		return( rv );
	}
	if( tok.at( 0 ).contains( "forward" ) || 
		tok.at( 0 ).contains( "reverse" ) ) {
		_format = Targets::Mothur;
		do {
			parseTokensMothur( tok );
		} while( tok.next( fp ) );
		rv = true;
	} else if( tok.at( 0 ) == "#SampleID" ) {
		_format = Targets::Qiime;
		do {
			parseTokensQiime( tok );
		} while( tok.next( fp ) );
		_fwd = S( _data[ 0 ][ "LinkerPrimerSequence" ] );	
		rv = true;
	} else {
		_format = Targets::Text;
		do {
			parseTokensText( tok );
		} while( tok.next( fp ) );
		rv = true;
	}
	return( rv );
}
bool	Targets::load( const QString& path )
{
	bool	rv = false;
	Ifp	fp;

	if( !fp.open( path ) ) {
		return( rv );
	}
	rv = read( fp );
	fp.close();
	return( rv );
}
void	Targets::show( QTextStream& fp, const Format& format )
{
	switch( format ) {
		case	Targets::Text:
			break;
		case	Targets::Qiime:
			showQiime( fp );
			break;
		case	Targets::Mothur:
			showMothur( fp );
			break;
		default:
			break;
	}
}
void	Targets::showMothur( QTextStream& fp )
{
	if( !_fwd.isEmpty() ) {
		fp << "forward";
		fp << " "  << _fwd;
		if( !_fwdLabel.isEmpty() ) {
			fp << " " << _fwdLabel;
		}
		fp << endl;
	}
	if( !_rev.isEmpty() ) {
		fp << "reverse";
		fp << " "  << _rev;
		if( !_fwdLabel.isEmpty() ) {
			fp << " " << _revLabel;
		}
		fp << endl;
	}
	for( int i = 0; i < _data.size(); ++i ) {
		fp << "barcode ";
		fp << S( _data[i][ "BarcodeSequence" ] );
		fp << " ";
		fp << S( _data[i][ "Description" ] ) << endl;
	}
}
void	Targets::showQiime( QTextStream& fp )
{
	if( _format == Targets::Mothur ) {
		fp << "#SampleID" << "\t" << "BarcodeSequence";
		if( !( _fwd.isEmpty() && _rev.isEmpty() ) ) {
 			fp << "\t" << "LinkerPrimerSequence";
		}
		fp << "\t" << "Description" << endl;
		for( int i = 0; i < _data.size(); ++i ) {
			fp << i + 1;
			fp << "\t";
			fp << S( _data[i][ "BarcodeSequence" ] );
			fp << "\t";
			if( !_fwd.isEmpty() ) {
				fp << _fwd;
				fp << "\t";
			} else {
				fp << _rev;
				fp << "\t";
			}
			fp << S( _data[i][ "Description" ] );
			fp << endl;
		}
	} else {
		fp << "#" << _hdr.join( "\t" ) << endl;
		for( int i = 0; i < _data.size(); ++i ) {
			for( int j = 0; j < _hdr.size(); ++j ) {
				if( j > 0 ) {
					fp << "\t";
				}
				fp << S( _data[i][ _hdr[j] ] );
			}
			fp << endl;
		}
	}
}
void	Targets::parseTokensMothur( Token& tok )
{
	if( tok.at( 0 ).contains( "forward" ) ) {
		_fwd = tok.at( 1 );
		if( tok.size() > 2 ) {
			_fwdLabel = tok.at( 2 );
		}
		return;
	} else if( tok.at( 0 ).contains( "reverse" ) ) {
		_rev = tok.at( 1 );
		if( tok.size() > 2 ) {
			_revLabel = tok.at( 2 );
		}
		return;
	}
	if( _data.size() == 0 ) {
		_hdr = QStringList() << "BarcodeSequence" << "Description";
		_row.attachHeader( _hdr );
	}
	if( tok.at( 0 ) == "barcode" ) {
		_row[ "BarcodeSequence" ] = tok.at( 1 );
		_row[ "Description" ]  = tok.at( 2 );
		_data << _row;
	}
}
void	Targets::parseTokensQiime( Token& tok )
{
	if( _hdr.size() == 0 ) {
		_hdr = tok;
		_hdr[ 0 ].remove( 0, 1 );	// #
		_row.attachHeader( _hdr );
		return;
	}
	_row.clear();
	for( int i = 0; i < tok.size(); ++i ) {
		_row[ _hdr.at( i ) ] = tok.at( i );
	}
	_data << _row;
}
void	Targets::parseTokensText( Token& tok )
{
	if( _data.size() == 0 ) {
		_hdr = tok;
		_row.attachHeader( _hdr );
		return;
	}
	_row.clear();
	for( int i = 0; i < tok.size(); ++i ) {
		_row[ _hdr.at( i ) ] = tok.at( i );
	}
	_data << _row;
}
bool	Targets::changeHeader( const QString& inCol, 
	 const QString& outCol, QStringList& hdr )
{
	QMap<QString,QString>	map;

	for( int i = 0; i < _data.size(); ++i ) {
		map.insert( S( _data[i][ inCol ] ), S( _data[i][ outCol ] ) );
	}
	for( int i = 0; i < hdr.size(); ++i ) {
		if( map.contains( hdr.at( i ) ) ) {
			hdr[ i ] = map[ hdr.at( i ) ];
		}
	}
	return( true );
}
}	//	GH namespace
