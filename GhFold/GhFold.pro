TEMPLATE=lib
TGT=GhFold
CONFIG += gh bio
include( headers.pri )
include( sources.pri )
include( ../ghbase.pri )

VIENNA_INC="/usr/local/include/ViennaRNA"
VIENNA_LIB="/usr/local/lib"

INCLUDEPATH += $$VIENNA_INC
LIBS += -L$$VIENNA_LIB -lRNA
INCLUDEPATH+=/usr/local/include
