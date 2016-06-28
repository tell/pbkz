include ../common.mk

ifeq (Darwin,$(shell uname -s))
NTL_SCRIPT = sed '162,164d' ntl-unix/ntl/src/DoConfig > DoConfig && mv -f DoConfig ntl-unix/ntl/src/DoConfig
else
NTL_SCRIPT = true
endif

NTL_CD = cd ntl-unix/ntl/src
NTL_CONFIG = ./configure NATIVE=off CXX=$(CXX) CXXFLAGS='-std=c++11 -g -O3' PREFIX=$(NTL_PREFIX) SHARED=on

.PHONY: all

all: ntl-unix.done

ntl-unix.done:
	$(NTL_SCRIPT)
	$(NTL_CD) && $(NTL_CONFIG) && make && make install
	touch $@

