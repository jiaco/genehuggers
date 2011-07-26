#ifndef	GH_CLIAPP_H
#define	GH_CLIAPP_H	1
#include <QObject>
#include <QMessageBox>
#include "ghErrorprone.h"
#include "ghParamModel.h"


namespace	GH
{
QMap<QString,QString>	MapStringArgs( const int& argc, char** argv );

const	QString	StyleParam = "_Style";

class	GH_DLL_EXPORT	CliApp	:	public	QObject, public Errorprone
{
	Q_OBJECT

public:
	CliApp( int argc, char** argv );

	QStringList	paramNames( const QString& subgroup = QString() ) const;
	ParamModel*	param( const QString& name ) const;
	ParamModel*	param( const QString& name );
	void	addParam( const QString& name, ParamModel* param );
	void	setParamValue( const QString& name, const QVariant& value );

	ParamModel::ParamType	paramType( const QString& name );

	void	addParam( const QString& name, const QVariant& value,
		 const ParamModel::ParamType& type = ParamModel::Undef,
		 const QString& displayName = QString() );

	bool	isGui() const;
	QString arg0() const;
	QString	arg( const QString& name ) const;
	bool	checkArgs();

	void	showError() const;
	void	setRequiredParamNames( const QStringList& names );
public slots:
	void	resetDefaults();
signals:
	void	emitError( const Error& error ) const;
	void	emitError( Error* error ) const;

protected:
	QStringList			_requiredParamNames;
	QStringList			_paramNames;
	QMap<QString,ParamModel*>	_param;

	bool				_isGui;
	QString				_arg0;
	QMap<QString,QString>		_arg;
};

}	//	GH namespace
#endif	//	_GH_OBJAPP_H
