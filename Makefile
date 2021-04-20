BUILDDIR=build
OPTIMIZE=-Ofast
ELFNAME=BalashEngine

CFLAGS= $(OPTIMIZE)  -g#remove -g when release
ASFLAGS=-g


SOURCE:=$(wildcard *.c) 
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.c=.o) 
ASMOBJS:=$(ASM:.S=.o)

all:BalashEngine Makefile domestos

getexec:#later for bloatstudio
	@echo $(BUILDDIR)/$(ELFNAME)
domestos:#really. added domestos XD
	rm -f $(OBJS) $(ASMOBJS)
mrproper:
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
BalashEngine: $(OBJS) $(ASMOBJS)
	c++ $(OBJS) $(ASMOBJS) -o $(BUILDDIR)/$(ELFNAME) $(CFLAGS) -lpthread -lSDL2
