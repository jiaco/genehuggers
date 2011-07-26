/*! \file _docs/GhCore/ghIfp.d */
#ifndef	GH_IFP_H
#define	GH_IFP_H	1
#include "ghDef.h"
#include "ghErrorprone.h"
#include "ghIdf.h"
//
//	Version 5 notes, ghIdf has been brought over
//	 but not even sure what it is used for
//

namespace	GH	{

const	QString	STDIN = "stdin";	//!< QString constant for "stdin"

const	QIODevice::OpenMode	READONLY = QIODevice::ReadOnly;
	//!< shorthand for OpenMode for Input files
const	QIODevice::OpenMode	UNBUFFERED = QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered;
	//!< shorthand for OpenMode for Input files

const	int	EOL_SIZE = 1;		//!< used in \a next()

/*!	\brief	Input QTextStream subclass for stdin and files.

	Ifp serves as a uniform interface for input files
	as well as stdin.
	It is a subclass of QTextStream that also contains a QFile.
	The Ifp class will take care of the QFile operations if it
	is used with a file and provides QTextStream behaviors to both
	standard input as well as input files.

	For examples see \a GH::App.
 */
class	GH_DLL_EXPORT	Ifp	: public QTextStream, public GH::Errorprone
{
public:
	bool	open( const QString& usePath = GH::USTR );
	void	close();
	QString	next();
	QString	getCurrent() const;
	QString	getPath() const;
	QString	getSuffix() const;

	bool	back();
	bool	isStdin() const;
	bool	isSeekable() const;
	bool	fseek( qint64 pos );
	qint64	fpos() const;

	static	QString	load( const QString& path );
private:
	bool	getChar( QChar& ch );
	void	putChar( const QChar& ch );

	//
	//	this is used for .gz support
	//
	QProcess	process;
	QStringList	processArgs;

	QFile	file;		//!< Internal QFile for use with files.
	QString	path;		//!< QString to hold current input file path.
	QString	previous;	//!< QString to hold previous input line.
	QString	current;	//!< QString to hold current input line.
	bool	seekable;	//!< Flag which is false for compressed stdin
	qint64	byteCount;
			//!< Internally keep track of number of bytes read
	bool	backed;
			//!< Internal boolean flag for the back hack. \sa back()
};

//
//	this is the preferred method for reading file line by line
//
inline	bool	nextLine( Ifp& fp, QString& line )
{
	line = fp.next();
	if( line == USTR ) {
		return( false );
	}
	return( true );
}

}	//	GH namespace
#endif	//	GH_IFP_H
