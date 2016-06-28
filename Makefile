ifneq ($(MAKECMDGOALS),clean)
ifeq (1,$(shell test -d "$(NTL_SRC)" || echo 1))
$(error Invalid NTL source directory: NTL_SRC="$(NTL_SRC)")
endif
ifeq (1,$(shell test -d "$(NTL_PREFIX)" || echo 1))
$(error Invalid NTL installation directory: NTL_PREFIX="$(NTL_PREFIX)")
endif
ifeq (1,$(shell test -d "$(GEN_SRC)" || echo 1))
$(error Invalid instance generator source directory: GEN_SRC="$(GEN_SRC)")
endif
endif

.PHONY: all clean links subdirs

all: links subdirs

clean:
	-$(MAKE) -C pbkzlib clean

links:
	mkdir -p pbkzlib/include/external
	cd pbkzlib/include/external && \
		for x in $(NTL_SRC)/LLL_RR.c $(NTL_SRC)/LLL_QP.c $(GEN_SRC)/tools.h $(GEN_SRC)/ideal.h; do \
			[ -f `basename $$x` ] || ln -fs $$x; \
		done

subdirs:
	$(MAKE) -C pbkzlib

