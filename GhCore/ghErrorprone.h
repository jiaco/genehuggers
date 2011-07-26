#ifndef	GH_ERRORPRONE_H
#define	GH_ERRORPRONE_H	1
#include "ghDef.h"
namespace	GH
{

class	GH_DLL_EXPORT	Error
{
public:
	enum	ErrorType { None, Info, Warn, Crit };

	Error();

	void	clear();
	void	setMessage( const QString& emsg );
	void	setType( const int& type );
	void	setInfo( const QString& emsg );
	void	setWarning( const QString& emsg );
	void	setCritical( const QString& emsg );

	QString	message() const;
	int	type() const;
	bool	state() const;

private:
	QString	_message;
	int	_type;
};
inline	Error	Warning( const QString& msg )
{
	Error	error;
	error.setType( Error::Warn );
	error.setMessage( msg );
	return( error );
}
inline	Error	Info( const QString& msg )
{
	Error error;
	error.setType( Error::Info );
	error.setMessage( msg );
	return( error );
}
class	GH_DLL_EXPORT	Errorprone
{
public:

	Errorprone();


	const Error&	error() const;
	Error*	error();
	void	setError( const Error& error );
	void	setError( Error* error );

	void	clearError();
	bool	errorState() const;
	bool	hasError() const;

	void	setWarning( const QString& emsg );
	void	setCritical( const QString& emsg );
	void	setError( const QString& emsg, const int& type = Error::Crit );
	void	setErrorMessage( const QString& emsg = QString() );
	void	setErrorType( const int& type = Error::Crit );

	QString	errorMessage() const;
	int	errorType() const;
protected:
	Error	_error;
};

}	//	GH namespace
#endif	//	GH_ERRORPRONE_H
