top_builddir = ../../..
include ${top_builddir}/makeinclude

CPP_SRC = inf_well.cxx \

DEP  = ${CPP_SRC:%.cxx=%.dep}
OBJ  = ${CPP_SRC:%.cxx=%.o}
LOBJ = ${CPP_SRC:%.cxx=%.lo}

ifeq (${MAKECMDGOALS}, obj)
include $(DEP)
endif

dep: $(DEP)

obj: $(LOBJ)

lib: liblibrary.la libhpc.a

liblibrary.la: $(LOBJ)
	$(LINK) $(LOBJ) -o $@

libhpc.a:
	$(LINK) $(LIBS) -o $@

clean:
	$(RM) $(DEP) $(OBJ) $(LOBJ) liblibrary.la libhpc.a .libs *~

.PHONY: dep obj lib clean
