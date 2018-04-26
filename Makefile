include common.mk

.PHONY: all clean submods prepare subdirs

all: prepare patch.done subdirs
check:
	$(MAKE) -C verification check

clean:
	-$(MAKE) -C pbkzlib clean

submods:
	mkdir -p $(NTL_PREFIX)
	$(MAKE) -C submods

EXT_FILES = $(ROOT_DIR)/pbkzlib/external
.PHONY: prepare
prepare:
	mkdir -p $(EXT_FILES)
	cd $(EXT_FILES) && \
		for x in $(NTL_SRC)/LLL_RR.c $(NTL_SRC)/LLL_QP.c $(GEN_SRC)/tools.h $(GEN_SRC)/ideal.h; do \
			[ -f `basename $$x` ] || ln -fs $$x; \
		done

patch.done: fix.patch
	patch -p1 < $<
	touch $@

subdirs:
	$(MAKE) -C pbkzlib

