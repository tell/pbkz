include ../common.mk

NTL_SUBMOD_DIR = ntl-unix
NTL_BUILD_DIR = build_ntl
NTL_CD = cd $(NTL_BUILD_DIR)

ifeq (Darwin,$(shell uname -s))
# TODO: should be fixed.
NTL_SCRIPT = false # sed '162,164d' ntl-unix/ntl/src/DoConfig > DoConfig && mv -f DoConfig ntl-unix/ntl/src/DoConfig
else
NTL_SCRIPT = true
endif

.PHONY: all clean

all: ntl-unix.done

clean:
	-$(NTL_CD) && make clean

ntl-unix.done:
	mkdir -p $(NTL_BUILD_DIR)
	cp -r $(NTL_SUBMOD_DIR)/* $(NTL_BUILD_DIR)/
	$(NTL_CD) \
		&& make PREFIX=$(NTL_PREFIX) 2>&1 > build.log \
		&& make PREFIX=$(NTL_PREFIX) install
	touch $@

