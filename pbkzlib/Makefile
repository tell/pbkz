include ../common.mk

CC = $(CXX)
CXXFLAGS += -std=c++11 -Wall -Wextra -Ofast
ifeq (1,$(OPTIMIZE))
CXXFLAGS += -march=native -flto
endif
CPPFLAGS += -I . -I $(NTL_PREFIX)/include

# LDLIBS += $(NTL_PREFIX)/lib/libntl.a -lgmp -lgsl -lgslcblas
LDLIBS += -lntl -lgmp -lgsl -lgslcblas
LDFLAGS += -L $(NTL_PREFIX)/lib

ifneq (Darwin,$(shell uname -s))
# CXXFLAGS += -fopenmp
# LDFLAGS += -fopenmp
endif

.PNOHY: all clean

all: main.out

clean:
	$(RM) *.out

%.out: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

