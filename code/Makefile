top_builddir = ..
include ${top_builddir}/makeinclude

DIRS = utility library

all:
	for dir in ${DIRS}; do \
		${MAKE} -C $${dir} all; \
	done

install: \
	install_stl
	for dir in ${DIRS}; do \
		${MAKE} -C $${dir} install; \
	done

install_stl:
	mkdir -p $(INSTALL_INCLUDE_DIR)
	cp -r ./stl $(INSTALL_INCLUDE_DIR)

clean:
	for dir in ${DIRS}; do \
		${MAKE} -C $${dir} clean; \
	done
	rm -rfv *~

.PHONY: all clean
