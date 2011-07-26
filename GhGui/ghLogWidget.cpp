#include "ghLogWidget.h"

namespace	GH
{
LogWidget::LogWidget( QWidget* parent, Qt::WindowFlags f )
	: QWidget( parent, f )
{
	_textEdit = new QTextEdit( this );
	_layout = new QVBoxLayout;

	_layout->addWidget( _textEdit );
	setLayout( _layout );
	setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setWindowModality( Qt::NonModal );
}
void	LogWidget::log( const QString& s )
{
	_textEdit->append( s );
}
void	LogWidget::clear()
{
	_textEdit->clear();
}
void	LogWidget::setMinimumSize( int minw, int minh )
{
	//_textEdit->setMinimumSize( minw, minh - 50 );
	QWidget::setMinimumSize( minw, minh );
}
}	//	GH namespace
