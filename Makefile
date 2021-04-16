SOURCE:=$(wildcard *.c) $(wildcard *.cpp)
OBJS:=$(SOURCE:.c=.o)
CPPOBJS:=$(SOURCE:.cpp=.o)

CXXFLAGS=-g 
CFLAGS=$(CXXFLAGS)

all:BalashEngine Makefile 
BalashEngine: $(OBJS) $(CPPOBJS)
	c++ $(OBJS) $(CPPOBJS )-o BalashEngine  -lSDL2 -lpthread
