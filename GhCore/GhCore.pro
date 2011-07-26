TEMPLATE=lib
#CONFIG += __debug
contains( CONFIG, __debug ) {
	message( $${_FILE_} called from $$PWD )
}
TGT=GhCore

include( headers.pri )
include( sources.pri )

include( ../ghbase.pri )
