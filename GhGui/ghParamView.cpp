#include "ghParamView.h"

namespace	GH
{
	ParamView::ParamView( ParamModel *model, QWidget* parent )
	: QObject( parent )
{
	_model = model;
	_parent = parent;
	setObjectName( _model->name() );

	// this->setValue() will emit setSetting(name,value)
	//
	connect( model, SIGNAL( changed(QVariant) ),
	 this, SLOT( setValue(QVariant) ) );

	connect( model, SIGNAL( enabled(bool) ),
	 this, SLOT( setEnabled(bool) ) );
// new
	connect( model, SIGNAL( checked(bool) ),
	 this, SLOT( setChecked(bool) ) );

	connect( this, SIGNAL( changed(QVariant) ),
	 model, SLOT( guiSetValue(QVariant) ) );
	connect( this, SIGNAL( enabled(bool) ),
	 model, SLOT( setEnabled(bool) ) );
// new
	connect( this, SIGNAL( checked(bool) ),
	 model, SLOT( setChecked(bool) ) );

	//_enabled = _model->isEnabled();
}
const char*	ParamView::className()
{
	switch( _modelType ) {
		case	ParamModel::Action:
			return( "GH::ActionView" );
			break;
		case	ParamModel::Boolean:
			return( "GH::BooleanView" );
			break;
		case	ParamModel::Choice:
			return( "GH::ChoiceView" );
			break;
		case	ParamModel::File:
			return( "GH::FileView" );
			break;
		case	ParamModel::Font:
			return( "GH::FontView" );
			break;
		case	ParamModel::Color:
			return( "GH::ColorView" );
			break;
		case	ParamModel::ToolButton:
			return( "GH::ToolButtonView" );
			break;
		default:
			return( "GH::ParamView" );
			break;
	}
}
ParamModel* ParamView::model() const
{
	return( _model );
}
/*
void	ParamView::addToGrid( QGridLayout* layout, const int& row, const int& col, const WidgetType& widgetType )
{
	Q_UNUSED( widgetType );

	addToGrid( layout, row, col );
}
*/
QString	ParamView::nameObject( const QString& subName )
{
	return( QString( "%1_%2" ).arg( _model->name() ).arg( subName ) );
}
void	ParamView::setValue( const QVariant& value )
{
	emit( changed( value ) );
	updateValue();
	emit( setSetting( _model->name(), _model->value() ) );
}
void	ParamView::setEnabled( const bool& enabled )
{
	_model->guiSetEnabled( enabled );
	updateEnabled();
}
void	ParamView::setChecked( const bool& tf )
{
	_model->guiSetChecked( tf );
	updateChecked();
}
void	ParamView::setCheckable( const bool& tf )
{
	_model->guiSetCheckable( tf );
	updateCheckable();
}
bool	ParamView::isEnabled() const
{
	return( _model->isEnabled() );
}
bool	ParamView::isChecked() const
{
	return( _model->isChecked() );
}
bool	ParamView::isCheckable() const
{
	return( _model->isCheckable() );
}
}	//	GH namespace
