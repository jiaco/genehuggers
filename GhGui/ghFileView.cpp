#include "ghFileView.h"

namespace	GH
{
	FileView::FileView( ParamModel *model, QWidget *parent )
	: ToolButtonView( model, parent )
{
	connect( this, SIGNAL( defaultAction() ),
	 this, SLOT( doBrowse() ) );

/*
	if( _model->hasConfig( "checkable" ) ) {
		if( B( _model->config( "checkable" ) ) ) {
			setCheckable( true );
		}
	}
*/
	configure();
}
FileView::DialogType	FileView::StringToDialogType( const QString& text )
{
	QString	s = text.toLower();
	if( s.contains( "open" ) ) {
		if( s.contains( "multi" ) ) {
			return( FileView::OpenMulti );
		} else {
			return( FileView::Open );
		}
	} else if( s.contains( "dir" ) ) {
		if( s.contains( "save" ) ) {
			return( FileView::SaveDir );
		} else {
			return( FileView::Dir );
		}
	} else {
		return( FileView::Save );
	}
}
bool	FileView::SetCheckable( QObject* parent, const QString& name,
	 const bool& tf )
{
	FileView *p;

	if( ( p = GetParam<FileView>( parent, name, className() ) ) == 0 ) {
		return( false );
	}
	p->setCheckable( tf );
	return( true );
}
void	FileView::addToGrid( QGridLayout *layout,
	 const int& row, const int& col,
	 const int& rowSpan, const int& colSpan,
	 Qt::Alignment alignment )
{
	ToolButtonView::addToGrid( layout, row, col,
	 rowSpan, colSpan, alignment );

	_startingDir = SL( _model->value() ).at( 0 );
}
void	FileView::configure( const DialogType& type,
	 const QString& caption, const QString& filter )
{
	if( _model->hasConfig( "dialogtype" ) ) {
		_type =
		 FileView::StringToDialogType(
			 _model->configString( "dialogtype" ) );
	} else {
		_type = type;
	}
	if( _model->hasConfig( "caption" ) ) {
		_caption = _model->configString( "caption" );
	} else if( caption.isEmpty() ) {
		switch( _type ) {
			case	Dir:
				_caption = CAPTION_DEFAULT_DIR;
				break;
			case	SaveDir:
				_caption = CAPTION_DEFAULT_SAVEDIR;
				break;
			case	Open:
				_caption = CAPTION_DEFAULT_OPEN;
				break;
			case	OpenMulti:
				_caption = CAPTION_DEFAULT_OPENMULTI;
				break;
			case	Save:
				_caption = CAPTION_DEFAULT_SAVE;
				break;
		}
	} else {
		_caption = caption;
	}
	if( _model->hasConfig( "filter" ) ) {
		_filter = _model->configString( "filter" );
	} else {
		_filter = filter;
	}
}
bool	FileView::Configure( QObject *parent, const QString& name,
	 const DialogType& type, const QString& caption, const QString& filter )
{
	FileView *p;

	if( ( p = GetParam<FileView>( parent, name, className() ) ) == 0 ) {
		return( false );
	}
	p->configure( type, caption, filter );
	return( true );
}
void	FileView::setSinglePath( const QString& path )
{
	_startingDir = path;
	setValue( V( path ) );
}
void	FileView::setMultiplePath( const QStringList& path )
{
	if( path.size() > 0 ) {
		_startingDir = path.at( 0 );
	}
	setValue( QVariant( path ) );
}
void	FileView::doBrowse()
{
	QString		path;
	QStringList	pathList;

	LoseFocus<QLineEdit>( this, _lineEditName );
	switch( _type ) {
		case	Dir:
		case	SaveDir:
			path = QFileDialog::getExistingDirectory(
			 _parent, _caption, _startingDir );
			if( path.isNull() ) {
				return;
			}
			setSinglePath( path );
			break;
		case	Open:
			path = QFileDialog::getOpenFileName(
			 _parent, _caption, _startingDir, _filter );
			if( path.isNull() ) {
				return;
			}
			setSinglePath( path );
			break;
		case	OpenMulti:
			pathList = QFileDialog::getOpenFileNames(
			 _parent, _caption, _startingDir, _filter );
			if( pathList.isEmpty() ) {
				return;
			}
			setMultiplePath( pathList );
			break;
		case	Save:
			path = QFileDialog::getSaveFileName(
			 _parent, _caption, _startingDir, _filter );
			if( path.isNull() ) {
				return;
			}
			setSinglePath( path );
			break;
	}
}
}	//	GH namespace
