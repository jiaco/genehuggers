#include "ghDac.h"
namespace	GH
{
	DacBase::DacBase()
	: Errorprone()
{
	init();
}
void	DacBase::init()
{
	_params.clear();
	clearError();
}
const QMap<QString,QVariant>	DacBase::params() const
{
	return( _params );
}
void	DacBase::setParam( const QString& pname, const QVariant& pvalue )
{
	if( _params.contains( pname ) ) {
		_params[ pname ] = pvalue;
	} else {
		_params.insert( pname, pvalue );
	}
}
void	DacBase::setParams( const QMap<QString,QVariant>& params )
{
	_params = params;
}
void	DacBase::injectParams( const QMap<QString,QVariant>& params )
{
	foreach( QString pname, params.keys() ) {
		if( _params.contains( pname ) ) {
			_params[ pname ] = params[ pname ];
		} else {
			_params.insert( pname, params[ pname ] );
		}
	}
}
bool		DacBase::hasParam( const QString& pname ) const
{
	return( _params.contains( pname ) );
}
QVariant	DacBase::getParam( const QString& pname ) const
{
	if( _params.contains( pname ) ) {
		return( _params[ pname ] );
	}
	return( QVariant() );
}
}	//	GH namespace
