BUILDDIR=build
OPTIMIZE=-Ofast
ELFNAME=BalashEngine#checkout to branch cpp
#kill it with sigkill damnit so do killall BalashEngine -9 thanks
CFLAGS= -Ofast  -g  #remove -g when release
ASFLAGS=-g
#press right arrow to rotate line
CC=cc
CXX=c++
SOURCE:=$(wildcard *.cpp) 
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.cpp=.o) 
ASMOBJS:=$(ASM:.S=.o)

all:$(BUILDDIR)/$(ELFNAME) Makefile 

getexec:#later for bloatstudio
	@echo $(BUILDDIR)/$(ELFNAME)
domestos:#really. added domestos XD
	rm -f $(OBJS) $(ASMOBJS)
mrproper:
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
$(BUILDDIR)/$(ELFNAME): $(OBJS) $(ASMOBJS) #put it in here so clang will shut up
	c++ $(OBJS) $(ASMOBJS) -o $(BUILDDIR)/$(ELFNAME) $(CFLAGS) -lpthread -lSDL2 
