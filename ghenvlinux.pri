
PREFIX=/usr/local
#
# test the Env GH_HOME and if not set get the GH_PWD from ghbase.pri
#
isEmpty( $$(GH_HOME) ) {
	GH_HOME=$${GH_PWD}
	message( Set GH_HOME to PWD $${GH_HOME} )
	message( While I can build the library, you need to do some things to use it. )
	message( Please see the bash_profile example in $${GH_HOME} )
} else {
	# should check that they are the same thing?
	eval( $$(GH_HOME) != $${GH_PWD} ) {
		error( GH_HOME needs to be this: $${GH_PWD} )
	}
	GH_HOME=$$(GH_HOME)
}
message( Building GeneHuggers $${VERSION} from GH_HOME $${GH_HOME} )

#
#	setup include GH_INC path
#
isEmpty( $$(GH_INC) ) {
	GH_INC=$${PREFIX}/include/gh.$${VERSION}
} else {
	GH_INC=$$(GH_INC)
}
!exists( $${GH_INC} ) {
	system( mkdir -p $${GH_INC} ) : error( Unable to mkdir $${GH_INC} )
}
#
#	setup lib GH_LIB path
#
isEmpty( $$(GH_LIB) ) {
	GH_LIB=$${PREFIX}/lib/gh.$${VERSION}
} else {
	GH_LIB=$$(GH_LIB)
}
!exists( $${GH_LIB} ) {
	system( mkdir -p $${GH_LIB} ) {
		message( $${GH_LIB} created for you )
	} else {
		error( Unable to mkdir $${GH_LIB} )
	}
}
#
#	setup GH_BIN path
#
isEmpty( $$(GH_BIN) ) {
	GH_BIN=$${PREFIX}/bin/gh.$${VERSION}
} else {
	GH_BIN=$$(GH_BIN)
}
!exists( $${GH_BIN} ) {
	system( mkdir -p $${GH_BIN} ) {
		message( $${GH_BIN} created for you )
	} else {
		error( Unable to mkdir $${GH_BIN} )
	}
}
