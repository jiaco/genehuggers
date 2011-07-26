#include "ghOfp.h"

namespace	GH	{

//
//	ml stands for "markup language"
//	 this method is used to make html output cleaner for text editor
//
void	Ofp::ml( const QString& buf )
{
	QString	s = buf;
	s = s.replace( "'", "\"" );
	(*this) << s;
	if( s.endsWith( '>' ) ) {
		(*this) << "\n";
	}
}
bool	Ofp::open( const QString& usePath )
{
	clearError();
	if( usePath == GH::USTR || usePath == STDOUT ) {
		file.open( stdout, QIODevice::WriteOnly );
		setDevice( &file );
		path = STDOUT;
		return( true );
	} else if( usePath == STDERR ) {
		file.open( stderr, QIODevice::WriteOnly );
		setDevice( &file );
		path = STDERR;
		return( true );
	} else {
		file.setFileName( usePath );
		if( !file.open( QIODevice::WriteOnly ) ) {
			setCritical(
			 QString( "Unable to write: %1" ).arg( usePath ) );
			return( false );
		}
		setDevice( &file );
		path = usePath;
		return( true );
	}
}
bool	Ofp::append( const QString& usePath )
{
	file.setFileName( usePath );
	if( !file.open( QIODevice::Append ) ) {
		setCritical(
		 QString( "Unable to write: %1" ).arg( usePath ) );
		return( false );
	}
	setDevice( &file );
	path = usePath;
	return( true );
}
void	Ofp::close()
{
	file.close();
}
qint64	Ofp::fpos() const
{
	return( pos() );
}
QString	Ofp::getPath() const
{
	return( path );
}
}	//	GH namespace
