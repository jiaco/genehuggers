QMAKE_COPY="cp -fp"
#	## note: contains( TGT, GhCore ) worked

VIENNA_INC="/usr/local/include/ViennaRNA"
VIENNA_LIB="/usr/local/lib"

#contains( CONFIG, __debug ) {
#	unix:!macx {
#		message( $${_FILE_} called from $$PWD )
#		message( Manually setting env in qmake )
#
#		GH_LIB=/usr/local/lib/gh/4.2.0
#		GH_INC=/usr/local/include/gh/4.2.0
#		GH_BIN=/usr/local/bin/gh/4.2.0
#	}
#}
#
# can capture the GH_HOME from the PWD of ghbase.pri
#
GH_PWD=$$PWD

isEmpty( TGT ) : error( TGT must be set before including $${_FILE_} )

include( ghversion.pri )
include( ghlibnames.pri )


macx {
	GH_HOME=$$(GH_HOME)
	GH_LIB=$$(GH_LIB)
	GH_INC=$$(GH_INC)
	GH_BIN=$$(GH_BIN)
	isEmpty( GH_HOME ) : GH_HOME=/Users/jiaco/src/genehuggers/GhLib
	isEmpty( GH_LIB ) : GH_LIB=/Library/Frameworks
	# do not need a GH_INC on mac
	isEmpty( GH_BIN ) : GH_BIN=/Applications

	# would like to not put cli apps in this location...

	contains( TEMPLATE, lib ) {
		# still have no idea what explicitlib actually does...
		CONFIG += lib_bundle explicitlib
		DESTDIR = $${GH_LIB}

		FRAMEWORK_HEADERS.version = Versions
		FRAMEWORK_HEADERS.files = $${HEADERS}
		FRAMEWORK_HEADERS.path = Headers

		QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
		QMAKE_FRAMEWORK_BUNDLE_NAME = $${TGT}
		QMAKE_FRAMEWORK_VERSION = $${VERSION}
	}
	contains( TEMPLATE, app ) {
		DESTDIR = $${GH_BIN}
		contains( CONFIG, cli ) {
			CONFIG -= app_bundle
		#	DESTDIR=$${SOURCE_DIR}
			DESTDIR=$${GH_BIN}
		}
	}
	TARGET=$${TGT}

	RESOURCES += $${GH_HOME}/resources/style.qrc

	macinstall.commands = $${PWD}/scripts/macinstall.sh $${TGT} $${VERSION}
	QMAKE_EXTRA_TARGETS += macinstall

	GH_CORE_LIB=$${CORE_LIB}
	GH_CORE_INC=$${CORE_LIB}".framework/Headers"
	GH_BIO_LIB=$${BIO_LIB}
	GH_BIO_INC=$${BIO_LIB}".framework/Headers"
	GH_FOLD_LIB=$${FOLD_LIB}
	GH_FOLD_INC=$${FOLD_LIB}".framework/Headers"
	GH_GUI_LIB=$${GUI_LIB}
	GH_GUI_INC=$${GUI_LIB}".framework/Headers"
	GH_SALSA_LIB=$${SALSA_LIB}
	GH_SALSA_INC=$${SALSA_LIB}".framework/Headers"

	contains( CONFIG, gh ) {
		INCLUDEPATH += $${GH_LIB}/$${GH_CORE_INC}
		LIBS += -framework $${GH_CORE_LIB}
	}
	contains( CONFIG, bio ) {
		INCLUDEPATH += $${GH_LIB}/$${GH_BIO_INC}
		LIBS += -framework $${GH_BIO_LIB}
	}
	contains( CONFIG, fold ) {
		INCLUDEPATH += $${GH_LIB}/$${GH_FOLD_INC}
		LIBS += -framework $${GH_FOLD_LIB}
	}
	contains( CONFIG, gui ) {
		INCLUDEPATH += $${GH_LIB}/$${GH_GUI_INC}
		LIBS += -framework $${GH_GUI_LIB}
	}
	contains( CONFIG, salsa ) {
		INCLUDEPATH += $${GH_LIB}/$${GH_SALSA_INC}
		LIBS += -framework $${GH_SALSA_LIB}
	}

}	# end of macx build



win32 {
	# how to let Qt figure out where libs should go
	# cannot build libs without installing headers somewhere...
	# windows sucks...
	include( ghwin32.pri )
}	# end of win32 build

unix:!macx {
	include( ghenvlinux.pri )

#	isEmpty( GH_LIB ) : GH_LIB=/usr/local/lib/gh/$${VERSION}
#	isEmpty( GH_INC ) : GH_INC=/usr/local/include/gh/$${VERSION}
#	isEmpty( GH_BIN ) : GH_BIN=/usr/local/bin/gh/$${VERSION}

#message( VERSION $${VERSION} )

#	!exists( $${GH_LIB} ) {
#		error( Unable to proceed without $${GH_LIB} )
#	}
#	!exists( $${GH_INC} ) {
#		error( Unable to proceed without $${GH_INC} )
#	}
#	!exists( $${GH_BIN} ) {
#		error( Unable to proceed without $${GH_BIN} )
#	}
#	
	contains( TEMPLATE, lib ) {
		DESTDIR=$${GH_LIB}
		TARGET=$${TGT}.$${VERSION}
		install.path = $${GH_INC}
		install.files = $${HEADERS}
		INSTALLS += install
		QMAKE_EXTRA_TARGETS += install
	}
	contains( TEMPLATE, app ) {
		DESTDIR=$${GH_BIN}
		TARGET=$${TGT}
	}
	INCLUDEPATH += $${GH_INC}

	RESOURCES += $${GH_HOME}/resources/style.qrc

	contains( CONFIG, gh ) {
		LIBS += -L$${GH_LIB} -l$${CORE_LIB}.$${VERSION}
	}
	contains( CONFIG, bio ) {
		LIBS += -L$${GH_LIB} -l$${BIO_LIB}.$${VERSION}
	}
	contains( CONFIG, fold ) {
		INCLUDEPATH += $$VIENNA_INC
		LIBS += -L$$VIENNA_LIB -lRNA
		LIBS += -L$${GH_LIB} -l$${FOLD_LIB}.$${VERSION}
	}
	contains( CONFIG, gui ) {
		LIBS += -L$${GH_LIB} -l$${GUI_LIB}.$${VERSION}
	}
	contains( CONFIG, db ) {
		QT += sql
		LIBS += -L${GH_LIB} -l$${DB_LIB}.$${VERSION}
	}
	contains( CONFIG, salsa ) {
		LIBS += -L$${GH_LIB} -l$${SALSA_LIB}.$${VERSION}
	}

}	# end of unix build
