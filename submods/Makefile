include ../common.mk

.PHONY: all

all: ntl-unix.done

ntl-unix.done:
	cd ntl-unix/ntl/src && ./configure PREFIX=$(NTL_PREFIX) SHARED=on && make && make install
	touch $@

