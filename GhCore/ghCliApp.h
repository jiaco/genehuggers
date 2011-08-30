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

#define	ghApp	CliApp::instance()

QVariant	APP_V( const QString& name );
bool		APP_B( const QString& name );
int		APP_I( const QString& name );
double		APP_D( const QString& name );
QString		APP_S( const QString& name );
QStringList	APP_SL( const QString& name );
QColor		APP_CLR( const QString& name );
QFont		APP_FONT( const QString& name );
QString		APP_FONTSTRING( const QString& name );

class	GH_DLL_EXPORT	CliApp	:	public	QObject, public Errorprone
{
	Q_OBJECT

public:
	CliApp( int argc, char** argv );
	~CliApp();

	static CliApp *instance() { return self; }

	QStringList	paramNames( const QString& subgroup = QString() ) const;
	ParamModel*	param( const QString& name ) const;
	ParamModel*	param( const QString& name );

	void	addParam( const QString& name, ParamModel* param );
	void	setParamValue( const QString& name, const QVariant& value );
	bool	hasParam( const QString& name ) const;

	ParamModel::ParamType	paramType( const QString& name );

	void	addParam( const QString& name,
		 const ParamModel::ParamType& type,
		 const QString& configText );

	void	addParam( const QString& name,
		 const QString& configText );

	void	addParam( const QString& name, const QVariant& value,
		 const ParamModel::ParamType& type = ParamModel::Undef,
		 const QString& displayName = QString(),
		 const QString& configText = QString() );

	bool	isGui() const;
	QString arg0() const;
	QString	arg( const QString& name ) const;
	bool	checkArgs();

	void	passInfo( const QString& text ) const;
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
private:
	static	CliApp	*self;

	Q_DISABLE_COPY(CliApp)
};

}	//	GH namespace
#endif	//	_GH_OBJAPP_H
