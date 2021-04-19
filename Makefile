BUILDDIR=build
OPTIMIZE=-Ofast
ELFNAME=BalashEngine

CFLAGS= $(OPTIMIZE) -lSDL2 -lpthread -std=c++17 -g
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
	g++ $(OBJS) $(ASMOBJS) -o $(BUILDDIR)/$(ELFNAME) $(CFLAGS)
