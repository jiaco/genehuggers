#ifndef	MYAPP_H
#define	MYAPP_H	1
#include <GhCore.h>
#include <GhGui.h>

namespace	GH
{

class	MyApp	:	public	ObjApp
{
	Q_OBJECT

public:
	MyApp( int argc, char** argv );

	void	doit();

public slots:
private:
	Ofp	cout, cerr;
};

}	//	GH namespace
#endif	//	MYAPP_H
