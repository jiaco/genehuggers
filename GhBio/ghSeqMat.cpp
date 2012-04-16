#include "ghSeqMat.h"
namespace	GH
{
	SeqMat::SeqMat( const QString& pattern, const QString& label )
{
	IUPAC	iupac;
	QString	bases;

	_score = 0;
	_pattern = pattern;
	_label = label;
	_size = _pattern.size();
	_m = new double*[ _size ];
	_rm = new double*[ _size ];
	for( int i = 0; i < _size; ++i ) {
		_m[ i ] = new double[ 4 ];
		_rm[ i ] = new double[ 4 ];
		for( int j = 0; j < 4; ++j ) {
			_m[ i ][ j ] = 0;
			_rm[ i ][ j ] = 0;
		}
		if( !iupac.contains( _pattern.at( i ) ) ) {
			_m[i][A] = _m[i][C] = _m[i][G] = _m[i][T] = 0.25;
			continue;
		}
		bases = iupac.value( _pattern.at( i ) );
		double v = 1.0 / ( bases.size() * 1.0 );
		foreach( QChar ch, bases ) {
			_m[ i ][ B2I( ch ) ] = v;
		}
	}
	int j = 0;
	for( int i = _size - 1; i >= 0; --i ) {
		_rm[ j ][ A ] = _m[ i ][ T ];
		_rm[ j ][ C ] = _m[ i ][ G ];
		_rm[ j ][ G ] = _m[ i ][ C ];
		_rm[ j ][ T ] = _m[ i ][ A ];
		++j;
	}
}
	SeqMat::~SeqMat()
{
	for( int i = 0; i < _size; ++i ) {
		delete _m[ i ];
		delete _rm[ i ];
	}
	delete _m;
	delete _rm;
}
int	SeqMat::size() const
{
	return( _pattern.size() );
}
QString	SeqMat::pattern() const
{
	return( _pattern );
}
QString SeqMat::label() const
{
	return( _label );
}
int	SeqMat::mismatches() const
{
	return( _mismatches );
}
double	SeqMat::score( const QString& target ) const
{
	double rv = 0;
	for( int i = 0; i < _size; ++i ) {
		rv += _m[ i ][ B2I( target.at( i ) ) ];
	}
	return( rv );
}
double	SeqMat::rscore( const QString& target ) const
{
	double rv = 0;
	for( int i = 0; i < _size; ++i ) {
		rv += _rm[ i ][ B2I( target.at( i ) ) ];
	}
	return( rv );
}
int	SeqMat::bestMatch( const QString& target, const int& mm, double& score )
{
	double	best;
	int	bpos = UINT;

	score = 0;
	best = 0;
	for( int pos = 0; pos <= target.size() - _size; ++pos ) {
		double d = 0;
		for( int i = 0; i < _size; ++i ) {
			d += _m[ i ][ B2I( target.at( pos + i ) ) ];
		}
		if( d >= _size - mm && d > best ) {
			best = d;
			bpos = pos;
		}
	}
	if( bpos != UINT ) {
		score = best;
	}
	return( bpos );
}
int	SeqMat::match( const QString& target, const int& mm )
{
	if( target.contains( _pattern ) ) {
		return( target.indexOf( _pattern ) );
	}
	// this is wrong, pattern can be ambiguous
	if( mm == 0 ) {
		return( UINT );
	}
	_mismatches = UINT;
	// finds the first match
	for( int pos = 0; pos < target.size() - _size; ++pos ) {
		_score = 0;
		for( int i = 0; i < _size; ++i ) {
			_score += _m[ i ][ B2I( target.at( pos + i ) ) ];
		}
		if( _score >= _size - mm ) {
			_mismatches = _size - _score;
			return( pos );
		}
	}
	return( UINT );
}
int	SeqMat::rmatch( const QString& target, const int& mm )
{
	_mismatches = UINT;
	// finds the first match
	for( int pos = 0; pos < target.size() - _size; ++pos ) {
		_score = 0;
		for( int i = 0; i < _size; ++i ) {
			_score += _rm[ i ][ B2I( target.at( pos + i ) ) ];
		}
		if( _score >= _size - mm ) {
			_mismatches = _size - _score;
			return( pos );
		}
	}
	return( UINT );
}
double	SeqMat::lastScore() const
{
	return( _score );
}
}	//	GH namespace
