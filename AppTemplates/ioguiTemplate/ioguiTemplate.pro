TEMPLATE=app
TGT="ioguiTemplate"

CONFIG += gh gui

#RESOURCES += $$(GH_HOME)/resources/headers.qrc

HEADERS +=\
	window.h	\
	myApp.h

SOURCES +=\
	window.cpp	\
	myApp.cpp	\
	_main.cpp

include( $$(GH_HOME)/ghbase.pri )