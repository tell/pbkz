ROOT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
NTL_SRC = $(ROOT_DIR)/submods/ntl-unix/ntl/src
NTL_PREFIX = $(ROOT_DIR)/prefix
GEN_SRC = $(ROOT_DIR)/submods/latticechlgs/generators
