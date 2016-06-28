CC = $(CXX)
CXXFLAGS += -std=c++11 -Wall -Wextra -Ofast
ifeq (1,$(OPTIMIZE))
CXXFLAGS += -march=native -flto
endif
CPPFLAGS += -I ./include -I $(NTL_PREFIX)/include
LDLIBS += $(NTL_PREFIX)/lib/libntl.a -lgmp -lgsl -lgslcblas

ifneq (Darwin,$(shell uname -s))
CXXFLAGS += -fopenmp
LDFLAGS += -fopenmp
endif

.PNOHY: all clean

all: main.out

clean:
	$(RM) *.out

%.out: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

