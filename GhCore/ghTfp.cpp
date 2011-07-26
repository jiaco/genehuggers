#include "ghTfp.h"

namespace	GH	{

bool	Tfp::init()
{
	path = "";
	if( !file.open() ) {
		setCritical( "Error occured with Tfp::init() file.open()" );
		return( false );
	}
	path = file.fileName();
	close();
	return( true );
}
bool	Tfp::open( const QString& fileTemplate )
{
	if( !fileTemplate.isEmpty() ) {
		file.setFileTemplate( fileTemplate );
	}
	if( !file.open() ) {
		setCritical( "Error occured with Tfp::open() file.open()" );
		return( false );
	}
	file.setAutoRemove( false );
	setDevice( &file );
	path = file.fileName();
	return( true );
}
const	QString&	Tfp::getPath() const
{
	return( path );
}
void	Tfp::close()
{
	file.close();
}
void	Tfp::setTmp( const QString& tpath )
{
	file.setFileTemplate( tpath );
	return;
}
qint64	Tfp::fpos() const
{
	return( pos() );
}
QString	Tfp::TempPath( const QString& name )
{
	Tfp	fp;
	QString	rv;

	fp.setTmp( name );
	fp.open();
	rv = fp.getPath();
	fp.close();
	return( rv );
}
}	//	GH namespace
