BUILDDIR=build
OPTIMIZE=-Ofast

CFLAGS= $(OPTIMIZE) -lSDL2 -lpthread
ASFLAGS=-g

SOURCE:=$(wildcard *.c) 
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.c=.o) 
ASMOBJS:=$(ASM:.S=.o)

all:BalashEngine Makefile 
mrproper:
	rm -rf $(BUILDDIR)
BalashEngine: $(OBJS) $(ASMOBJS)
	c++ $(OBJS) $(ASMOBJS) -o BUILDDIR/BalashEngine $(CFLAGS)
