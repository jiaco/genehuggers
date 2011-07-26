#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: ObjApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	addParam( "Annotation", "", Param::FileParam, "Annotation File" );
	addParam( "Input", "", Param::FileParam, "Input File" );

	_isGui = !checkArgs();
}
void	MyApp::doit()
{
	// input is fasta with a knownUcscId somewhere in the idlne
	// output is same fasta with >SYMBOL_UCSC [...Other...] Range= Strand= 
	//
	QString apath = S( paramValue( "Annotation" ) );
	QString ipath = S( paramValue( "Input" ) );

	Ifp	ifp;
	Row	row;
	Fast	fast;

	QString			annoIdxColumn = "#hg18.knownGene.name";
	QMap<QString,Row>	annoMap;

	if( !ifp.open( apath ) ) {
		cerr << "error reading " << apath << endl;
		return;
	}
	row.setEmptyParts( true );
	row.attachHeader( ifp.next().split( "\t" ) );
	while( row.next( ifp ) ) {
		annoMap.insert( S( row[ annoIdxColumn ] ), row );
	}
	ifp.close();

	//
	//	read the fasta input and find the annoIdxKey
	//
	if( !ifp.open( ipath ) ) {
		cerr << "error reading " << ipath << endl;
		return;
	}
	while( fast.next( ifp ) ) {
		QString	s = fast.sid();
		QStringList	tok = s.split( QRegExp( "[_ ]" ) );
		foreach( QString st, tok ) {
			if( !annoMap.contains( st ) ) {
				continue;
			}
			row = annoMap[ st ];
			QString osid = QString( "%1_%2 %3" )
			 .arg( S( row[ "hg18.kgXref.geneSymbol" ] ) )
			 .arg( st )
			 .arg( S( row[ "hg18.kgXref.description" ] ) );
			fast.setSid( osid );
			fast.show( cout );
			break;
		}
	}
	ifp.close();
	return;
}
}	//	GH namespace
