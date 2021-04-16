SOURCE:=$(wildcard *.c) $(wildcard *.cpp)
OBJS:=$(SOURCE:.c=.o)
CPPOBJS:=$(SOURCE:.cpp=.o)

CXXFLAGS=CFLAGS=-Ofast 

all:BalashEngine Makefile 
BalashEngine: $(OBJS) $(CPPOBJS)
	cc $(OBJS) $(CPPOBJS )-o BalashEngine  -lSDL2
