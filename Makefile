BUILDDIR=build
OPTIMIZE=-Ofast -msse4 -mfpmath=sse -march=native -mpopcnt -mbmi2  -fno-pie -fno-PIE -fno-PIC -no-pie 
ELFNAME=BalashEngine#checkout to branch cpp

CXX=g++
CC=gcc
LIBS=`pkg-config --libs --cflags libpng`
HEADERS=$(wildcard include/*.hpp) $(wildcard *.hpp)
CFLAGS= $(OPTIMIZE) $(LIBS) -lpthread -lSDL2 -g -Wall #remove -g when release 
CPPFLAGS= $(OPTIMIZE) -g   -Wall #remove -g when release
ASFLAGS=-g

SOURCE:=$(wildcard *.cpp)  
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.cpp=.o) 
ASMOBJS:=$(ASM:.S=.o)

default:$(HEADERS) all run 
so:
	c++ -shared -o $(BUILDDIR)/libbalash.so -fPIC $(CFLAGS) $(SOSOURCE) 
run:
	./$(BUILDDIR)/$(ELFNAME)

all:$(BUILDDIR)/$(ELFNAME) Makefile 

getexec:#later for bloatstudio
	@echo $(BUILDDIR)/$(ELFNAME)
domestos:#really. added domestos XD
	rm -f $(OBJS) $(ASMOBJS)
mrproper:
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
$(BUILDDIR)/$(ELFNAME): softrendr.o $(OBJS) $(ASMOBJS)
	$(CXX) $(OBJS) $(ASMOBJS) -o $(BUILDDIR)/$(ELFNAME) $(CFLAGS)

