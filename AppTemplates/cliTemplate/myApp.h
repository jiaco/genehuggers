#ifndef	MY_APP_H
#define	MY_APP_H	1
#include <GhCore.h>

namespace	GH
{
class	MyApp	: public	CliApp
{
public:
	MyApp( int argc, char** argv );

	bool	doit();

private:
	Ofp	cout, cerr;
};

}	//	GH namespace
#endif	//	MY_APP_H
