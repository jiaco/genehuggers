#ifndef	GH_DB_H
#define	GH_DB_H	1
#include "GhCore.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>

namespace	GH
{
const	QString	DB_DEFAULT_USER = "salsa";
const	QString	DB_DEFAULT_PASS = "salsa";

class	GH_DLL_EXPORT DB
{
public:
	enum	Driver	{
		Undef, Mysql, Psql, Lite
	};
	enum	FieldType {
		Integer, Double, Text, Blob
	};
		/*
		 * converts from Qt-String to Driver-enum
		 */
	static	Driver	DbDriver( const QString& qtString );
		/*
		 * converts from Driver-enum to Qt-String
		 */
	static	QString	QtDriver( const Driver& ghEnum );

	static QSqlDatabase	Open(
				 const QString& databaseName,
				 const Driver& driverType = DB::Mysql,
				 const QString& userName = DB_DEFAULT_USER,
				 const QString& password = DB_DEFAULT_PASS,
				 const QString& hostName = "localhost",
				 GH::Error *error = 0
				);

	static	QString	Database( QSqlDatabase& db );
	static	bool	ChangeDatabase( QSqlDatabase& db, const QString& databaseName );
	static	QStringList	Tables( QSqlDatabase& db, const QString& databaseName = QString() );

	static	bool	Exec( QSqlDatabase& db, const QString& stmt, GH::Error *error = 0 );

	static	bool	Collect( QSqlDatabase& db,
			 const QString& selectStmt,
			 QStringList& hdr, QList<Row>& data );

	static	QSqlError	Error( const QSqlDatabase& db );
	static	QSqlError	Error( const QSqlQuery& db );
	static	bool		OK( const QSqlDatabase& db );
	static	bool		OK( const QSqlQuery& query );
	static	QString		ErrorText( const QSqlDatabase& db );
	static	QString		ErrorText( const QSqlQuery& query );

	static	DB::FieldType	GetFieldType( const QList<QVariant>& columnData );
	static	QList<DB::FieldType>	GetFieldTypes( const QStringList& hdr,
					 const QList<Row>& data );

	static	QString	MysqlFieldType( const FieldType& ftype );
	static	QString	MysqlCreateTable( const QString& tableName,
		 const QStringList& hdr, const QList<FieldType>& ftype );

	static	QString	MysqlInsertLeftPart( const QString& tableName,
			 const QStringList& hdr );
	static	QString	MysqlInsertRightPart( const QStringList& hdr,
			 const Row& dataRow, const QList<FieldType>& ftype );
/*
	static	bool	CreateDatabase();
	static	bool	CreateTable( QSqlDatabase& db,
			 const QString& databaseName,);
*/
};
/*
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
#endif	//	GH_DB_H
