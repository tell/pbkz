include ../common.mk

NTL_SUBMOD_DIR = ntl-unix
NTL_BUILD_DIR = build_ntl
NTL_CD = cd $(NTL_BUILD_DIR)/ntl/src

ifeq (Darwin,$(shell uname -s))
# TODO: should be fixed.
NTL_SCRIPT = false # sed '162,164d' ntl-unix/ntl/src/DoConfig > DoConfig && mv -f DoConfig ntl-unix/ntl/src/DoConfig
else
NTL_SCRIPT = true
endif

NTL_CONFIG = ./configure CXX=$(CXX) CXXFLAGS='-std=c++11 -g -O3' PREFIX=$(NTL_PREFIX)

.PHONY: all clean

all: ntl-unix.done

clean:
	-$(NTL_CD) && make clean && rm -f all

ntl-unix.done:
	cp -r $(NTL_SUBMOD_DIR) $(NTL_BUILD_DIR)
	$(NTL_CD) && $(NTL_CONFIG) && make && make install
	touch $@

