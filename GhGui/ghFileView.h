#ifndef	GH_FILEVIEW_H
#define	GH_FILEVIEW_H	1
#include "ghToolButtonView.h"
namespace	GH
{

const	QString	CAPTION_DEFAULT_DIR = "Select a directory to open";
const	QString	CAPTION_DEFAULT_SAVEDIR = "Select a directory to save";
const	QString	CAPTION_DEFAULT_OPEN = "Select a file to open";
const	QString	CAPTION_DEFAULT_OPENMULTI = "Select file(s) to open";
const	QString	CAPTION_DEFAULT_SAVE = "Select a file to save";

class	GH_DLL_EXPORT FileView	:	public ToolButtonView
{
	Q_OBJECT

public:
	enum	DialogType {
		Dir, Open, OpenMulti, Save, SaveDir
		};

	static	DialogType	StringToDialogType( const QString& text );

	FileView( ParamModel *model, QWidget *parent );

	void	addToGrid( QGridLayout *layout, const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );

	void	updateCheckable() {};

	void	configure( const DialogType& type = Open,
		 const QString& caption = QString(),
		 const QString& filter = QString() );
	static bool	Configure( QObject *parent, const QString& name,
		 const DialogType& type = Open,
		 const QString& caption = QString(),
		 const QString& filter = QString() );

	void	setSinglePath( const QString& path );
	void	setMultiplePath( const QStringList& path );

	static bool	SetCheckable( QObject* parent, const QString& name,
			 const bool& tf );
public slots:
	void	doBrowse();
signals:
	//void	toggled( const bool& checked );
protected:
	DialogType	_type;
	QString		_filter;
	QString		_caption;
	QString		_startingDir;

	static const ParamModel::Type _modelType = ParamModel::File;
};
}	//	GH namespace
#endif	//	GH_FILEVIEW_H
