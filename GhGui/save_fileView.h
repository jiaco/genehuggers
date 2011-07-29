#ifndef	GH_FILEVIEW_H
#define	GH_FILEVIEW_H	1
#include "ghParamView.h"
// May 16, 2011 changed FileView to use a QToolButton(QMenu) and lost
// the parent of EditView
namespace	GH
{

const	QString	CAPTION_DEFAULT_DIR = "Select a directory to open";
const	QString	CAPTION_DEFAULT_SAVEDIR = "Select a directory to save";
const	QString	CAPTION_DEFAULT_OPEN = "Select a file to open";
const	QString	CAPTION_DEFAULT_OPENMULTI = "Select file(s) to open";
const	QString	CAPTION_DEFAULT_SAVE = "Select a file to save";

class	GH_DLL_EXPORT FileView	:	public ParamView
{
	Q_OBJECT

public:
	enum	DialogType {
		Dir, Open, OpenMulti, Save, SaveDir
		};

	FileView( ParamModel *model, QWidget *parent );

	void	addToGrid( QGridLayout *layout, const int& row, const int& col,
		 const int& rowSpan = 1, const int& colSpan = 1,
		 Qt::Alignment alignment = 0 );

	void	updateValue();
	void	updateEnabled();
	void	updateChecked();
	void	updateCheckable() {};

	void	configure( const DialogType& type = Open,
		 const QString& caption = QString(),
		 const QString& filter = QString() );
	static bool	Configure( QObject *parent, const QString& name,
		 const DialogType& type = Open,
		 const QString& caption = QString(),
		 const QString& filter = QString() );

	void		addAction( QAction* action );
	static bool	AddAction( QObject *parent, const QString& name,
		 	 QAction* action );

	void	setSinglePath( const QString& path );
	void	setMultiplePath( const QStringList& path );

	// can just use enabled?
//static bool	Checked( const QObject *parent, const QString& name );
	//bool	checked() const;
	//void	setChecked( const bool& tf = true );
	//void	setCheckable( const bool& tf = true );
	static bool	SetCheckable( QObject* parent, const QString& name,
			 const bool& tf );
/*
	static bool	SetChecked( QObject* parent, const QString& name,
			 const bool& tf );
*/
public slots:
	void	doBrowse();
	void	textEdited( const QString& value );
	//void	toggle( const bool& value );
signals:
	//void	toggled( const bool& checked );
protected:
	QString	_widgetName;
	QMenu	*_menu;
	QString	_browseBtnName;
	QAction	*_browseAction;
	QString	_checkBoxName;

	DialogType	_type;
	QString		_filter;
	QString		_caption;
	QString		_startingDir;

	//bool	_checkable;
};
}	//	GH namespace
#endif	//	GH_FILEVIEW_H
