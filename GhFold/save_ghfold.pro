TEMPLATE = lib
CONFIG += dll
VERSION = 2.0.0

TGT="ghfold"

CONFIG += bio

include ( ../qmake_includes/ghEnv.pro )

VIENNA_INC="/usr/local/include/ViennaRNA"
VIENNA_LIB="/usr/local/lib"

INCLUDEPATH += $$VIENNA_INC
LIBS += -L$$VIENNA_LIB -lRNA

TARGET=$$GH_LIB/$$TGT

include ( ../qmake_includes/ghCfg.pro )

INSTALLS += install

install.path = $$GH_INCLUDE
install.files = *.h

INCLUDEPATH+=/usr/local/include

SOURCES +=\
	ghFold.cpp	\
	ghDuplex.cpp
