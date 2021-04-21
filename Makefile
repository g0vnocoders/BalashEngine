BUILDDIR=build
OPTIMIZE=-Ofast
ELFNAME=BalashEngine

CFLAGS= $(OPTIMIZE) -Wall -Wextra  -lpthread -lSDL2 -g#remove -g when release
ASFLAGS=-g


SOURCE:=$(wildcard *.c) 
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.c=.o) 
ASMOBJS:=$(ASM:.S=.o)

all:$(BUILDDIR)/$(ELFNAME) Makefile domestos

getexec:#later for bloatstudio
	@echo $(BUILDDIR)/$(ELFNAME)
domestos:#really. added domestos XD
	rm -f $(OBJS) $(ASMOBJS)
mrproper:
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
$(BUILDDIR)/$(ELFNAME): $(OBJS) $(ASMOBJS)
	c++ $(OBJS) $(ASMOBJS) -o $(BUILDDIR)/$(ELFNAME) $(CFLAGS)
