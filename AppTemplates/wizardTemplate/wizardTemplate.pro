TEMPLATE=app
TGT="wizardTemplate"

CONFIG += gh gui

#RESOURCES += $$(GH_HOME)/resources/headers.qrc

HEADERS +=\
	wizard.h	\
	myApp.h

SOURCES +=\
	wizard.cpp	\
	myApp.cpp	\
	_main.cpp

include( $$(GH_HOME)/ghbase.pri )
