#ifndef	MYAPP_H
#define	MYAPP_H	1
#include <GhCore.h>
#include <GhGui.h>

namespace	GH
{

class	MyApp	:	public	CliApp
{
	Q_OBJECT

public:
	MyApp( int argc, char** argv );

	void	doit();

private:
	Ofp	cout, cerr;
};

}	//	GH namespace
#endif	//	MYAPP_H
