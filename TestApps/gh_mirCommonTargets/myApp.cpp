#include "myApp.h"

namespace	GH
{
class X
{
public:
	int	cnt;
	QString	sid, qids;
};
bool	SortX( const X& e1, const X& e2 )
{
	return( e1.cnt > e2.cnt );
}
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Input", "", Param::FileParam, "Input File" );

	_isGui = !checkArgs();
}
void	MyApp::doit()
{
	QString ipath = S( paramValue( "Input" ) );

	Ifp	ifp;
	Row	row;

	QMap<QString,QStringList>	data;

	if( !ifp.open( ipath ) ) {
		cerr << "error reading " << ipath << endl;
		return;
	}
	row.attachHeader( ifp.next().split( "\t" ) );
	while( row.next( ifp ) ) {
		QString	sid = S( row[ "sid" ] );
		QString	qid = S( row[ "qid" ] );
		if( data.contains( sid ) && !data[ sid ].contains( qid ) ) {
			data[ sid ] << qid;
		} else {
			QStringList t;
			t << qid;
			data.insert( sid, t );
		}
	}
	ifp.close();

	QList<X> sorter;
	foreach( QString sid, data.keys() ) {
		X x;
		x.cnt = data[ sid ].size();
		x.sid = sid;
		x.qids = data[ sid ].join( " " );

		sorter << x;
	}
	qSort( sorter.begin(), sorter.end(), SortX );

	for( int i = 0; i < sorter.size(); ++i ) {
		cout << sorter[ i ].cnt << "\t";
		cout << sorter[ i ].sid << "\t";
		cout << sorter[ i ].qids << endl;
	}
/*

	foreach( QString sid, data.keys() ) {
		if( data[ sid ].size() > 2 ) {
			cout << sid;
			cout << " [ " << data[ sid ].size() << " ] ";
			foreach( QString qid, data[ sid ] ) {
				cout << "\t" << qid;
			}
			cout << endl;
		}
	}
*/
	return;
}
}	//	GH namespace
