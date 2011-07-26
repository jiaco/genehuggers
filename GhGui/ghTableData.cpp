#include "ghTableData.h"

namespace	GH
{
void	TableData::setValue( const int& row, const int& col,
	 const QVariant& value )
{
	(*this)[ row ][ col ] = value;
}
}	//	GH namespace
