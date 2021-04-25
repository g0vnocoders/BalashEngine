BUILDDIR=build
OPTIMIZE=-Ofast
ELFNAME=BalashEngine#checkout to branch cpp
CFLAGS= $(OPTIMIZE) -lpthread -lSDL2 -g  #remove -g when release
ASFLAGS=-g

CC=cc
CXX=c++
SOURCE:=$(wildcard *.cpp) 
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.cpp=.o) 
ASMOBJS:=$(ASM:.S=.o)
SOSOURCE=$(filter-out main.cpp,$(SOURCE))
default:all
so:
	c++ -shared -o $(BUILDDIR)/libbalash.so -fPIC $(CFLAGS) $(SOSOURCE) 
run:
	./$(BUILDDIR)/$(ELFNAME)

all:$(BUILDDIR)/$(ELFNAME) Makefile domestos run

getexec:#later for bloatstudio
	@echo $(BUILDDIR)/$(ELFNAME)
domestos:#really. added domestos XD
	rm -f $(OBJS) $(ASMOBJS)
mrproper:
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
$(BUILDDIR)/$(ELFNAME): $(OBJS) $(ASMOBJS)
	c++ $(OBJS) $(ASMOBJS) -o $(BUILDDIR)/$(ELFNAME) $(CFLAGS) #put it in here so cl
