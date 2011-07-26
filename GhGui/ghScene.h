#ifndef	GH_SCENE_H
#define	GH_SCENE_H	1
#include <QtGui>
#include "ghDef.h"

namespace	GH
{
class	GH_DLL_EXPORT Scene	:	public QGraphicsScene
{
	Q_OBJECT

public:
	Scene( QObject* parent = 0 );
};
}	//	GH namespace
#endif	//	GH_SCENE_H
