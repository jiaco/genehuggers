win32 {
	message( win32 )

	GH_HOME=$$(GH_HOME)
	isEmpty( GH_HOME ) {
		GH_HOME=$${GH_PWD}
	}

	GH_PATH=$$(GH_PATH)
	isEmpty( GH_PATH ) {
		GH_PATH=$${GH_PWD}"/Gh"
	}
#	isEmpty( GH_PATH )  : error( GH_PATH not set )


	GH_LIB=$$(GH_LIB)
	isEmpty( GH_LIB ) : GH_LIB=$${GH_PATH}
	GH_INC=$$(GH_INCLUDE)
	isEmpty( GH_INC ) : GH_INC=$${GH_PATH}
	GH_BIN=$$(GH_BIN)
	isEmpty( GH_BIN ) : GH_BIN=$${GH_PATH}

	CONFIG += release

	INCLUDEPATH += $${GH_INC}

	RESOURCES += $${GH_HOME}/resources/style.qrc

	message( GH INC set to $${GH_INC} )

contains( TEMPLATE, lib ) {
	DEFINES += GH_BUILD_EXPORT
	CONFIG *= shared

	DESTDIR=$${GH_LIB}
	TARGET=$${TGT}.$${VERSION}

	install.path = $${GH_INC}
	install.files = $${HEADERS}
	INSTALLS += install
	QMAKE_EXTRA_TARGETS += install
}
contains( TEMPLATE, app ) {
	TARGET=$${TGT}
	# this is still not right
	
	install.path = $${GH_INC}
	install.files = "release/"$${TARGET}
	INSTALLS += install
	QMAKE_EXTRA_TARGETS += install
}

	GH_CORE_LIB=$${CORE_LIB}.$${VERSION}$${VER}
	GH_BIO_LIB=$${BIO_LIB}.$${VERSION}$${VER}
	GH_GUI_LIB=$${GUI_LIB}.$${VERSION}$${VER}
	GH_SALSA_LIB=$${SALSA_LIB}.$${VERSION}$${VER}
	GH_DB_LIB=$${DB_LIB}.$${VERSION}$${VER}

	contains( CONFIG, gh ) {
		LIBS += -L"'"$${GH_LIB}"'" -l$${GH_CORE_LIB}
	}
	contains( CONFIG, bio ) {
		LIBS += -L"'"$${GH_LIB}"'" -l$${GH_BIO_LIB}
	}
	contains( CONFIG, gui ) {
		LIBS += -L"'"$${GH_LIB}"'" -l$${GH_GUI_LIB}
	}
	contains( CONFIG, salsa ) {
		LIBS += -L"'"$${GH_LIB}"'" -l$${GH_SALSA_LIB}
	}
	contains( CONFIG, db ) {
		QT += sql
		LIBS += -L"'"$${GH_LIB}"'" -l$${GH_DB_LIB}
	}
}
