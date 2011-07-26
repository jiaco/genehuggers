TEMPLATE=lib
#CONFIG += __debug
contains( CONFIG, __debug ) {
	message( $${_FILE_} called from $$PWD )
}
TGT=GhGui
CONFIG += gh

include( headers.pri )
include( sources.pri )

include( ../ghbase.pri )
