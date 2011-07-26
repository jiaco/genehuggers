TEMPLATE=app
TGT=ghmake

CONFIG += gh cli

RESOURCES +=\
	../resources/installer.qrc

HEADERS +=\
	myApp.h

SOURCES +=\
	myApp.cpp	\
	_main.cpp

SOURCE_DIR=$$PWD

include( ../ghbase.pri )
