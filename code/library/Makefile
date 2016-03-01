top_builddir = ../..
include ${top_builddir}/makeinclude

all:
	${MAKE} -C src dep
	${MAKE} -C src obj
	${MAKE} -C src lib

install: all
	mkdir -p $(INSTALL_INCLUDE_DIR)/library
	cp -r ./include $(INSTALL_INCLUDE_DIR)/library

	mkdir -p $(INSTALL_LIB_DIR)
	cp -r ./src/libhpc.a $(INSTALL_LIB_DIR)

clean:
	${MAKE} -C src clean
	rm -rfv *~ */*~
