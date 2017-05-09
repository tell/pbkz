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

links:
	mkdir -p pbkzlib/include/external
	cd pbkzlib/include/external && \
		for x in $(NTL_SRC)/LLL_RR.c $(NTL_SRC)/LLL_QP.c $(GEN_SRC)/tools.h $(GEN_SRC)/ideal.h; do \
			[ -f `basename $$x` ] || ln -fs $$x; \
		done

subdirs:
	$(MAKE) -C pbkzlib

