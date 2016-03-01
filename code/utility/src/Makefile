top_builddir = ../../..
include $(top_builddir)/makeinclude

SRC =

DEP  = $(SRC:%.cxx=%.dep)
OBJ  = $(SRC:%.cxx=%.o)
LOBJ = $(SRC:%.cxx=%.lo)

ifeq ($(MAKECMDGOALS), obj)
	include $(DEP)
endif

dep: $(DEP)

obj: $(LOBJ)

lib: libutility.la

libutility.la: $(LOBJ)
	$(LINK) $(LOBJ) -o $@

clean:
	$(RM) $(DEP) $(OBJ) $(LOBJ) libutility.la .libs *~

.PHONY: dep obj lib clean
