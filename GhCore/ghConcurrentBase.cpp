#include "ghConcurrentBase.h"

namespace	GH
{
	ConcurrentBase::ConcurrentBase()
{
	init();
}
void	ConcurrentBase::init()
{
	_params.clear();
}
void	ConcurrentBase::setParams( const QMap<QString,QVariant>& params )
{
	_params = params;
}
void	ConcurrentBase::injectParams( const QMap<QString,QVariant>& params )
{
	foreach( QString pname, params.keys() ) {
		if( _params.contains( pname ) ) {
			_params[ pname ] = params[ pname ];
		} else {
			_params.insert( pname, params[ pname ] );
		}
	}
}
const QMap<QString,QVariant>& ConcurrentBase::params() const
{
	return( _params );
}
}	//	GH namespace
