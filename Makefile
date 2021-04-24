BUILDDIR=build
OPTIMIZE=-Ofast
ELFNAME=BalashEngine#checkout to branch cpp
#kill it with sigkill damnit so do killall BalashEngine -9 thanks
CFLAGS= $(OPTIMIZE)   -g  #remove -g when release
ASFLAGS=-g

CC=cc
CXX=c++
SOURCE:=$(wildcard *.cpp) 
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.cpp=.o) 
ASMOBJS:=$(ASM:.S=.o)

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
	c++ $(OBJS) $(ASMOBJS) -o $(BUILDDIR)/$(ELFNAME) $(CFLAGS) -lpthread -lSDL2 #put it in here so cl
