/*!	\file	ghIfp.cpp
 */
#include "ghIfp.h"

namespace	GH	{

//	STATIC load method
QString	Ifp::load( const QString& path )
{
	Ifp	fp;
	QString	rv = "";
	QString	line;

	if( !fp.open( path ) ) {
		return( USTR );
	}
	while( ( line = fp.next() ) != USTR ) {
		rv += line + "\n";
	}
	fp.close();
	return( rv );
}

/*!	\brief	Open \a usePath (or stdin) as input.

	\param usePath The file path to be opened for reading.
	\note	Ifp.open() with no parameters will use stdin.
		stdin can also be selected by calling
		\a Ifp.open( \a GH::USTR ) and \a Ifp.open( \a GH::STDIN ).
 */
bool	Ifp::open( const QString& usePath )
{
	QChar	ch;

	clearError();
	seekable = false;
	QIODevice::OpenMode oflag = READONLY;

	if( usePath.isEmpty() ) {
		setCritical( "Unable to open: path empty" );
		return( false );
	}
	if( usePath == path ) {
		return( true );
	}
	if( usePath == GH::USTR || usePath == STDIN ) {
		file.open( stdin, oflag );
		setDevice( &file );
		path = STDIN;
	} else if( usePath.endsWith( ".gz" ) ) {
		path = usePath;
		processArgs << "-c";
		processArgs << path;
		process.start( "gunzip", processArgs, READONLY );
		setDevice( &process );
		if( !process.waitForStarted() ) {
			setCritical( "Error duing gunzip process-1" );
			return( false );
		}
		if( !process.waitForFinished() ) {
			setCritical( "Error duing gunzip process-2" );
			return( false );
		}
	} else {
		if( usePath.startsWith( GH_IDF_MARK ) ) {
			QString	idfKey = usePath.mid( GH_IDF_MARK.size() );
			QString	idfPath = IDF::lookup( idfKey );
			if( idfPath == USTR ) {
				setCritical(
				"Unable to lookup idf resource: "
					 + usePath );
				return( false );
			}
			file.setFileName( idfPath );
		} else {
			file.setFileName( usePath );
		}
		if( !file.open( oflag) ) {
			setError( "Unable to open file: " + usePath );
			return( false );
		}
		setDevice( &file );
		path = usePath;
	}
	if( seek( 0 ) ) {
		seekable = true;
	}
	previous = current = USTR;
	backed = false;
	byteCount = 0;
	return( true );
}
/*!	\brief	Close the current file/stream.

	Ifp.close() wraps the close method of the internal QFile.
	This method should be completely safe to call on an un-opened Ifp/QFile.
 */
void	Ifp::close()
{
	if( file.openMode() != QIODevice::NotOpen ) {
		file.close();
	}
	if( processArgs.size() > 0 ) {
		process.terminate();
		processArgs.clear();
	}
}
/*!	\brief	Get the number of bytes read.
	\return	\a byteCount.

	This method returns the number of bytes read, which is managed
	by Ifp, not by the QFile nor the QTextStream.
 */
qint64	Ifp::fpos() const
{
	return( byteCount );
}
/*!	\brief	Read (and return) the next line in the input.
	\return QString containing next line from input.

	If one only uses \a next() and \a back() the Ifp can be used to
	back up a line.

	\sa back()
 */
QString	Ifp::next()
{
	if( backed == true ) {
		backed = false;
		byteCount += ( current.size() + EOL_SIZE );
		return( current );
	}
	previous = current;
	current = readLine();
	if( current.isNull() ) {
		return( GH::USTR );
	} else {
		byteCount += ( current.size() + EOL_SIZE );
		return( current );
	}
}
/*!	\brief	A "quasi"-ungetline method.

	As long as next() is the next method called after a back()
	this will work. It is really only useful to get the bytecount
	before the current line was read.

	\sa next(), fpos()
 */
bool	Ifp::back()
{
	backed = true;
	byteCount -= ( current.size() + EOL_SIZE );
	return( true );
}
/*!	\brief	Wrap the QTextStream seek method.
	\param	pos the position to seek to.

	This function also sets \a backed to false in order to
	disable the use of \a back() after a fseek() call.
 */
bool	Ifp::fseek( qint64 pos )
{
	backed = false;
	return( seek( pos ) );
}
/*!	\brief	Returns a QString containing the current line.
 */
QString	Ifp::getCurrent() const
{
	return( current );
}
/*!	\brief	Returns a QString containing the current file path.
 */
QString	Ifp::getPath() const
{
	return( path );
}
/*!	\brief	Returns a QString containing the current file path's suffix.
 */
QString	Ifp::getSuffix() const
{
	int	x;
	if( ( x = path.lastIndexOf( '.' ) ) == UINT ) {
		return( "" );
	}
	return( path.mid( x + 1 ) );
}
/*!	\brief	Get the last error message.
	\return	QString containing the last logged error.

	In order to see details about why an Ifp method failed, this method
	returns a QString with more descriptive text about what happened.
QString	Ifp::error() const
{
	return( emsg );
}
 */
/*!	\brief	Return true if Ifp is reading from stdin
 */
bool	Ifp::isStdin() const
{
	if( path == STDIN ) {
		return( true );
	} else {
		return( false );
	}
}
/*!	\brief	Return true if Ifp is reading from a seekable source.

	During the open() method, a test is made to seek( 0 ).
	If this test fails, then \a seekable will be set to false.
 */
bool	Ifp::isSeekable() const
{
	return( seekable );
}
}	//	GH namespace
