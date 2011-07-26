#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Query", "", Param::FileParam, "Input File" );
	//
	//	QueryType can be id or seq
	//
	addParam( "QueryType", "ID", Param::FileParam, "Input File" );
	addParam( "Input", "", Param::FileParam, "Input File" );

	_isGui = !checkArgs();
}
void	MyApp::doit()
{
	QString qpath = S( paramValue( "Query" ) );
	QString	qtype = S( paramValue( "QueryType" ) ).toLower();
	QString ipath = S( paramValue( "Input" ) );

	QString	queryColumn = "mir";
	QStringList	queryList;
	QList<int>	queryMatched;
	QString	matchedQuery;
	

	QString	qId = "mir";
	QString	qSeq = "seq";
	QString iSeq = "Mature sequence";

	QStringList	qids, qseqs;

	Ifp	ifp;
	Row	row;
	Fast	fast;

	//
	//	collect all queries
	//
	if( !ifp.open( qpath ) ) {
		cerr << "error reading " << qpath << endl;
		return;
	}
	row.attachHeader( ifp.next().split( "\t" ) );
	while( row.next( ifp ) ) {
		QString s = S( row[ queryColumn ] );
		s = s.toLower();
		queryList << s;
		//queryList << S( row[ queryColumn ] );
		queryMatched << 0;
	}
	ifp.close();

	//
	//	read the fasta input (mature.fa for example)
	//	 and select the mirs
	//
	if( !ifp.open( ipath ) ) {
		cerr << "error reading " << ipath << endl;
		return;
	}
	while( fast.next( ifp ) ) {
		QString	s = fast.sid();
		s = s.toLower();
		s = s.replace( '*', '#' );

		if( queryList.contains( s ) ) {
			int idx = queryList.indexOf( s );
			matchedQuery = queryList.at( idx );
			s = QString( "%1 [ %2 ]" )
				.arg( fast.idline() )
				.arg( matchedQuery );
			s = s.remove( 0, 1 );
			fast.setSid( s );
			Fast::Show( cout, fast );
			queryMatched[ idx ] = 1;
		} 
	}
	ifp.fseek( 0 );

	while( fast.next( ifp ) ) {
		QString s = fast.sid();
		s = s.toLower();
		s = s.replace( '*', '#' );
		for( int idx = 0; idx < queryList.size(); ++idx ) {
			if( queryMatched.at( idx ) == 1 ) {
				continue;
			}
			if( s.startsWith( queryList.at( idx ) ) ) {
// || queryList.at( idx ).startsWith( s ) ) {
				s = QString( "%1 [ %2 ] CHECK_ME" )
					.arg( fast.idline() )
					.arg( queryList.at( idx ) );
				s = s.remove( 0, 1 );
				fast.setSid( s );
				Fast::Show( cout, fast );
				queryMatched[ idx ] = 1;
			}
		}
	}

	for( int idx = 0; idx < queryList.size(); ++idx ) {
		if( queryMatched.at( idx ) == 1 ) {
			continue;
		}
		cerr << "lost " << queryList.at( idx ) << endl;
	}
/*
	row.attachHeader( ifp.next().split( "\t" ) );
	while( row.next( ifp ) ) {
		for( int q = 0; q < qids.size(); ++q ) {
			QString qs = qseqs.at( q );
			if( qs.mid( 2, 6 ) == S( row[ iSeq ] ).mid( 2, 6 ) ) {
				cout << qids.at( q ) << "\t" << row.line() << endl;
			}
		}
	}
*/	

	ifp.close();
	return;
}
}	//	GH namespace
