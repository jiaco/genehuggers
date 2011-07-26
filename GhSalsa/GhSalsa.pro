TEMPLATE=lib
#CONFIG += __debug
contains( CONFIG, __debug ) {
	message( $${_FILE_} called from $$PWD )
}
TGT=GhSalsa
CONFIG += gh bio gui

include( headers.pri )
include( sources.pri )

include( ../ghbase.pri )
