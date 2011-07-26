TEMPLATE=app
TGT=cliTemplate

CONFIG += gh

#	should remove app_bundle on macx
CONFIG += cli

#RESOURCES +=\
#	$$(GH_HOME)/resources/headers.qrc

HEADERS +=\
	myApp.h

SOURCES +=\
	myApp.cpp	\
	_main.cpp

include( $$(GH_HOME)/ghbase.pri )
