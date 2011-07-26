#include "ghDb.h"

namespace	GH
{
DB::Driver	DB::DbDriver( const QString& qtString )
{
	if( qtString == "QMYSQL" ) {
		return( DB::Mysql );
	} else if( qtString == "QPSQL" ) {
		return( DB::Psql );
	} else if( qtString == "QSQLITE" ) {
		return( DB::Lite );
	}
	return( DB::Undef );
}
QString	DB::QtDriver( const Driver& ghEnum )
{
	QString	rv;

	switch( ghEnum ) {
		case	Mysql:
			rv = "QMYSQL";
			break;
		case	Psql:
			rv = "QPSQL";
			break;
		case	Lite:
			rv = "QSQLITE";
			break;
		default:
			rv = "";
			break;
	}
	return( rv );
}
QSqlDatabase	DB::Open( const QString& databaseName,
		 const Driver& driverType,
		 const QString& userName,
		 const QString& password,
		 const QString& hostName,
		 GH::Error *error )
{
	QString	emsgPrefix = "GH::DB::Open Failed: ";

	QString	qtDriver = DB::QtDriver( driverType );
	if( qtDriver.isEmpty() ) {
		if( error ) {
			error->setCritical( emsgPrefix + "Driver Type" );
		}
		return( QSqlDatabase::database() );
	}
	QString	connectionName = QString( "%1.%2.%3.%4" )
		.arg( qtDriver ).arg( databaseName )
		.arg( userName ).arg( hostName );

	if( QSqlDatabase::contains( connectionName ) ) {
		QSqlDatabase::removeDatabase( connectionName );
	}

	QSqlDatabase	db =
	 QSqlDatabase::addDatabase( qtDriver, connectionName );

	db.setHostName( hostName );
	db.setPort( 3306 );
	db.setDatabaseName( databaseName );
	db.setUserName( userName );
	db.setPassword( password );
	if( !db.open() ) {
		if( error ) {
			error->setCritical( emsgPrefix + ErrorText( db ) );
		}
		return( QSqlDatabase::database() );
	}
	return( db );
}
QString		DB::Database( QSqlDatabase& db )
{
	QSqlQuery	q( db );
	if( !DB::OK( q ) ) {
		return( ErrorText( q ) );
	}
	if( !q.exec( "SELECT DATABASE()" ) ) {
		return( ErrorText( q ) );
	}
	if( !q.next() ) {
		return( ErrorText( q ) );
	}
	return( S( q.value( 0 ) ) );
}
bool	DB::ChangeDatabase( QSqlDatabase& db, const QString& databaseName )
{
	if( DB::Database( db ) == databaseName ) {
		return( true );
	}
/*
 * this is not necessary
 *
	QSqlQuery	q( db );
	if( !q.exec( QString( "USE %1" ).arg( databaseName ) ) ) {
		return( false );
	}
*/
	if( DB::Database( db ) != databaseName ) {
		db.setDatabaseName( databaseName );
		if( db.open() ) {
			return( true );
		}
	}
	return( false );
}
QStringList	DB::Tables( QSqlDatabase& db, const QString& databaseName )
{
	QStringList	rv;

	if( !databaseName.isEmpty() && DB::Database( db ) != databaseName ) {
		if( !DB::ChangeDatabase( db, databaseName ) ) {
			return( QStringList() );
		}
	}
/*
 * here, no matter the QSql::TableType flag, was still getting lots of
 * "hidden" tables...
 */
	QSqlQuery	q( db );
	q.exec( "SHOW TABLES" );
	while( q.next() ) {
		rv << S( q.value( 0 ) );
	}
	return( rv );
}
//
//	NOW PASS an Error to each function
//
bool	DB::Exec( QSqlDatabase& db, const QString& stmt, GH::Error *error )
{
	if( !DB::OK( db ) ) {
		if( error ) {
			error->setCritical( ErrorText( db ) );
		}
		return( false );
	}
	QSqlQuery	q( db );
	if( !DB::OK( q ) ) {
		error->setCritical( ErrorText( q ) );
		return( false );
	}
	if( !q.exec( stmt ) ) {
		error->setCritical( ErrorText( q ) );
		return( false );
	}
	return( true );
}
bool	DB::Collect( QSqlDatabase& db,
	 const QString& selectStmt,
	 QStringList &hdr, QList<Row>& data )
{
	hdr.clear();
	data.clear();

	if( !DB::OK( db ) ) {
		return( false );
	}
	QSqlQuery	q( db );
	if( !DB::OK( q ) ) {
		return( false );
	}
	if( !q.exec( selectStmt ) ) {
		return( false );
	}
	if( q.size() > 0 ) {
		for( int i = 0; i < q.record().count(); ++i ) {
			hdr << q.record().fieldName( i );
		}
		while( q.next() ) {
			Row row;
			for( int i = 0; i < hdr.size(); ++i ) {
				row.insert( hdr.at( i ), q.value( i ) );
			}
			data << row;
		}
	}
	return( true );
}



QSqlError	DB::Error( const QSqlDatabase& db )
{
	if( db.isValid() ) {
		return( db.lastError() );
	}
	return( QSqlError() );
}
QSqlError	DB::Error( const QSqlQuery& query )
{
	return( query.lastError() );
}
bool	DB::OK( const QSqlDatabase& db )
{
	if( !db.isValid() ) {
		return( false );
	}
	if( DB::Error( db ).type() == QSqlError::NoError ) {
		return( true );
	}
	return( false );
}
bool	DB::OK( const QSqlQuery& query )
{
	if( query.lastError().type() == QSqlError::NoError ) {
		return( true );
	}
	return( false );
}
QString	DB::ErrorText( const QSqlDatabase& db )
{
	if( DB::OK( db ) ) {
		return( QString() );
	}
	return( DB::Error( db ).text() );
}
QString	DB::ErrorText( const QSqlQuery& query )
{
	return( DB::Error( query ).text() );
}
	// Functions to INJECT TEXT INTO SQL
	//
DB::FieldType	DB::GetFieldType( const QList<QVariant>& columnData )
{
	DB::FieldType     rv;

	bool    okDouble, okInteger;
	int     nDouble, nInteger, nBlob, nText;
	nDouble = nInteger = nBlob = nText = 0;

	foreach( QVariant v, columnData ) {
		v.toDouble( &okDouble );
		v.toInt( &okInteger );
		if( okDouble && !okInteger ) {
			++nDouble;
		} else if( okInteger ) {
			++nInteger;
		} else {
			if( v.toString().size() > 256 ) {
				++nBlob;
			} else {
				++nText;
			}
		}
	}
	if( nBlob > 0 ) {
		rv = DB::Blob;
	} else if( nText > 0 ) {
		rv = DB::Text;
	} else if( nDouble > 0 ) {
		rv = DB::Double;
	} else {
		rv = DB::Integer;
	}
	return( rv );
}
QList<DB::FieldType>	DB::GetFieldTypes( const QStringList& hdr,
			 const QList<Row>& data )
{
	QList<DB::FieldType>      rv;

	for( int i = 0; i < hdr.size(); ++i ) {
		rv << GetFieldType( CollectColumn( hdr.at( i ), data ) );
	}
	return( rv );
}
QString	DB::MysqlFieldType( const FieldType& ftype )
{
	switch( ftype ) {
		case	Integer:
			return( "INTEGER" );
			break;
		case	Double:
			return( "DOUBLE" );
			break;
		case	Text:
			return( "TEXT" );
			break;
		default:
			return( "BLOB" );
			break;
	}
}
QString	DB::MysqlCreateTable( const QString& tableName,
		const QStringList& hdr, const QList<FieldType>& ftype )
{
	QStringList	fieldDefs;
	for( int i = 0; i < hdr.size(); ++i ) {
		fieldDefs << QString( "`%1` %2" ).arg( hdr.at( i ) )
				.arg( MysqlFieldType( ftype.at( i ) ) );
	}
	return( QString( "CREATE TABLE %1 ( %2 )" )
		.arg( tableName )
		.arg( fieldDefs.join( "," ) ) );
}
QString	DB::MysqlInsertLeftPart( const QString& tableName,
	 const QStringList& hdr )
{
	QString	rv = QString( "INSERT INTO %1 ( " ).arg( tableName );
	for( int i = 0; i < hdr.size(); ++i ) {
		if( i > 0 ) {
			rv.append( "," );
		}
		rv.append( QString( "`%1`" ).arg( hdr.at( i ) ) );
	}
	rv.append( " )" );
	return( rv );
/*
	return( QString( "INSERT INTO %1 ( %2 )" )
		 .arg( tableName )
		 .arg( hdr.join( "," ) ) );
*/
}
QString	DB::MysqlInsertRightPart( const QStringList& hdr, const Row& dataRow,
		 const QList<FieldType>& ftype )
{
	QString rv;

	for( int i = 0; i < hdr.size(); ++i ) {
		if( i > 0 ) {
			rv.append( "," );
		}
		switch( ftype.at( i ) ) {
			case	Integer:
			case	Double:
				rv.append( S( dataRow[ hdr.at( i ) ] ) );
				break;
			default:
				rv.append( QString( "\"%1\"" )
				 .arg( S( dataRow[ hdr.at( i ) ] ) ) );
			break;
		}
	}
	return( rv );
}
	

/*
 *	Error related
inline	QSqlError	SqlError( const QSqlDatabase& db )
{
	if( db.isValid() ) {
		return( db.lastError() );
	}
	return( QSqlError() );
}
inline	bool	DB_OK( const QSqlDatabase& db )
{
	if( SqlError( db ).type() == QSqlError::NoError ) {
		return( true );
	}
	return( false );
}
inline	QString ErrorText( const QSqlError& error )
{
	return( error.text() );
}
inline	QString	SqlErrorText( const QSqlDatabase& db )
{
	if( DB_OK( db ) ) {
		return( QString() );
	}
	return( ErrorText( SqlError( db ) ) );
}
inline	QString	SqlErrorText( const QSqlQuery& query )
{
	return( ErrorText( query.lastError() ) );
}
 *	Wrappers to be as cross-driver as possible
	// For the create database though, it is not at all clear what should be done...
	//
inline	bool	CreateDatabase( QSqlDatabase& db, const QString& dbName )
{
	QStringList	currentDatabases;

	if( db.driverName() == "QSQLITE" ) {
		return( true );
	}
	QStringList	currentDatabases = db.tables();

	if( tables.contains( dbName ) ) {
		return( true );
	}
	QSqlQuery	sql( db );
	return( sql.exec( QString( "CREATE DATABASE %1" ).arg( dbName ) ) );
}
 *	More stuff
inline	QString	FieldType( const QVariant::Type& type )
{
	switch( type ) {
		case	QVariant::Invalid:
			return( "Invalid" );
			break;
		case	QVariant::Bool:
			return( "Bool" );
			break;
		case	QVariant::ByteArray:
			return( "ByteArray" );
			break;
		case	QVariant::Char:
			return( "Char" );
			break;
		case	QVariant::Double:
			return( "Double" );
			break;
		case	QVariant::Date:
			return( "Date" );
			break;
		case	QVariant::Image:
			return( "Image" );
			break;
		case	QVariant::Int:
			return( "Int" );
			break;
		case	QVariant::LongLong:
			return( "LongLong" );
			break;
		case	QVariant::String:
			return( "String" );
			break;
		case	QVariant::Time:
			return( "Time" );
			break;
		case	QVariant::ULongLong:
			return( "ULongLong" );
			break;
		default:
			return( "NA" );
			break;
	}
}
inline	bool	QueryDbase( QSqlQuery& query, const QString& stmt,
		 QStringList& hdr, QList<Row>& data )
{
	hdr.clear();
	data.clear();
	if( !query.exec( stmt ) ) {
		return( false );
	}
	if( query.size() > 0 ) {
		for( int i = 0; i < query.record().count(); ++i ) {
			hdr << query.record().fieldName( i );
		}
		while( query.next() ) {
			Row row;
			for( int i = 0; i < hdr.size(); ++i ) {
				row.insert( hdr.at( i ), query.value( i ) );
			}
			data << row;
		}
	}
	return( true );
}
 */
}	//	GH namespace
