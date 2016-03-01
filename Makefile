top_builddir = .
include ${top_builddir}/makeinclude

all:
	@echo "Please specify target: " \
	"lib, install, uninstall, doc, or clean."

lib:
	${MAKE} -C code all

install: uninstall install_lib

install_lib: lib
	${MAKE} -C code install

# install_udev_rules:
# 	@if [ ! -f $(UDEV_RULES_FOLDER)/99-libftdi.rules ]; then \
# 		touch $(UDEV_RULES_FOLDER)/99-libftdi.rules; \
# 	fi
# 
# 	@cat $(UDEV_RULES_FOLDER)/99-libftdi.rules | \
# 		grep $(subst ",\",ATTRS{idVendor}=="0403") | \
# 		grep -q $(subst ",\",ATTRS{idProduct}=="6010"); \
# 	status=$$?; \
# 	if [ $$status != 0 ]; then \
# 		cat $(top_builddir)/resources/99-libftdi.rules >> \
# 			$(UDEV_RULES_FOLDER)/99-libftdi.rules; \
# 	fi

uninstall:
	${RM} \
		$(INSTALL_INCLUDE_DIR) \
		$(INSTALL_LIB_DIR)/libquantum.a

doc:
	doxygen doxy.conf

clean:
	${RM} *~
	${RM} doc
	${RM} main
	${MAKE} -C code clean

.PHONY: all lib install install_lib doc clean
