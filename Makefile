include common.mk

.PHONY: all clean submods links subdirs

all: links subdirs
check:
	$(MAKE) -C verification check

clean:
	-$(MAKE) -C pbkzlib clean

submods:
	mkdir -p $(NTL_PREFIX)
	$(MAKE) -C submods

DIR_0 = pbkzlib/external
links:
	mkdir -p $(DIR_0)
	cd $(DIR_0) && \
		for x in $(NTL_SRC)/LLL_RR.c $(NTL_SRC)/LLL_QP.c $(GEN_SRC)/tools.h $(GEN_SRC)/ideal.h; do \
			[ -f `basename $$x` ] || ln -fs $$x; \
		done

subdirs:
	$(MAKE) -C pbkzlib CFLAGS='-I$(NTL_PREFIX)/include' LOADLIBES='$(NTL_PREFIX)/lib/libntl.a' LDFLAGS='-L$(NTL_PREFIX)/lib'

