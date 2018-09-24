SRC	= main.cpp
BOOST_DIR = ./boost_1_66_0

CXX = $(CC)
CFLAGS	+= -std=c++11 -Ofast -funroll-loops -flto -fomit-frame-pointer -fprefetch-loop-arrays -msse4 -fpermissive
LDFLAGS	+= -lgmp -lmpfr -lgsl -lntl
CPPFLAGS	+= -I. -I${BOOST_DIR}
CXXFLAGS = $(CFLAGS)

main: main.cpp
