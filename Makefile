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

.PHONY: all links subdirs

all: links subdirs

links:
	cd pbkzlib/include/external && \
		ln -fs $(NTL_SRC)/LLL_RR.c && \
		ln -fs $(NTL_SRC)/LLL_QP.c && \
		ln -fs $(GEN_SRC)/tools.h && \
		ln -fs $(GEN_SRC)/ideal.h

subdirs:
	$(MAKE) -C pbkzlib

