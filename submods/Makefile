include ../common.mk

NTL_CD = cd ntl-unix/ntl/src

ifeq (Darwin,$(shell uname -s))
# TODO: should be fixed.
NTL_SCRIPT = false # sed '162,164d' ntl-unix/ntl/src/DoConfig > DoConfig && mv -f DoConfig ntl-unix/ntl/src/DoConfig
else
NTL_SCRIPT = true
endif

NTL_CONFIG = ./configure NATIVE=off CXX=$(CXX) CXXFLAGS='-std=c++11 -g -O3' PREFIX=$(NTL_PREFIX)

.PHONY: all clean

all: ntl-unix.done

clean:
	-$(NTL_CD) && make clean && rm -f all

ntl-unix.done:
	$(NTL_CD) && $(NTL_CONFIG) && make >/dev/null 2>&1 && make install
	touch $@

