TEMPLATE=app
TGT="laidOut2"

CONFIG += gh gui

#RESOURCES += $$(GH_HOME)/resources/headers.qrc

HEADERS +=\
	ghParam2.h	\
	ghBaseParam2.h	\
	ghActionParam2.h	\
	ghPui2.h	\
	window.h	\
	myApp.h

SOURCES +=\
	ghParam2.cpp	\
	ghBaseParam2.cpp	\
	ghActionParam2.cpp	\
	window.cpp	\
	myApp.cpp	\
	_main.cpp

include( $$(GH_HOME)/ghbase.pri )
