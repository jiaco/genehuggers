#include "ghErrorprone.h"

namespace	GH
{
	/*
	 *	Error class, goes inside Errorprone
	 *	 but can then be transferred around to bubble up
	 * 	 and reach the user
	 */
	Error::Error()
{
	clear();
}
void	Error::clear()
{
	setMessage( QString() );
	setType( None );
}
void	Error::setMessage( const QString& emsg )
{
	_message = emsg;
}
void	Error::setType( const int& type )
{
	_type = type;
}
void	Error::setInfo( const QString& emsg )
{
	_message = emsg;
	_type = Info;
}
void	Error::setWarning( const QString& emsg )
{
	_message = emsg;
	_type = Warn;
}
void	Error::setCritical( const QString& emsg )
{
	_message = emsg;
	_type = Crit;
}
QString	Error::message() const
{
	return( _message );
}
int	Error::type() const
{
	return( _type );
}
bool	Errorprone::hasError() const
{
	return( !_error.state() );
}
bool	Error::state() const
{
	if( type() == None ) {
		return( true ); 
	}
	return( false );
}
	/*
	 *	Errorprone should be subclassed wherever possible
	 */
	Errorprone::Errorprone()
{
	clearError();
}
const Error&	Errorprone::error() const
{
	return( _error );
}
Error*	Errorprone::error()
{
	return( &_error );
}
void	Errorprone::setError( const Error& error )
{
	_error = error;
}
void	Errorprone::setError( Error* error )
{
	_error = (*error);
}
void	Errorprone::clearError()
{
	_error.clear();
}
bool	Errorprone::errorState() const
{
	return( _error.state() );
}
void	Errorprone::setWarning( const QString& emsg )
{
	_error.setMessage( emsg );
	_error.setType( Error::Warn );
}
void	Errorprone::setCritical( const QString& emsg )
{
	_error.setMessage( emsg );
	_error.setType( Error::Crit );
}
void	Errorprone::setError( const QString& emsg, const int& type )
{
	_error.setMessage( emsg );
	_error.setType( type );
}
void	Errorprone::setErrorMessage( const QString& emsg )
{
	_error.setMessage( emsg );
}
void	Errorprone::setErrorType( const int& type )
{
	_error.setType( type );
}
QString	Errorprone::errorMessage() const
{
	return( _error.message() );
}
int	Errorprone::errorType() const
{
	return( _error.type() );
}

}	//	GH namespace
