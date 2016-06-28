include ../common.mk

ifeq (Darwin,$(shell uname -s))
NTL_CONFIG.cmd = sed '162,164d' ntl-unix/ntl/src/DoConfig > DoConfig && mv -f DoConfig ntl-unix/ntl/src/DoConfig
else
NTL_CONFIG.cmd = true
endif

.PHONY: all

all: ntl-unix.done

ntl-unix.done:
	$(NTL_CONFIG.cmd)
	cd ntl-unix/ntl/src && ./configure CXX=$(CXX) PREFIX=$(NTL_PREFIX) SHARED=on && make && make install
	touch $@

