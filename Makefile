SOURCE:=$(wildcard *.c) 
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.c=.o) 
ASMOBJS:=$(ASM:.S=.o)

CXXFLAGS=-g 
CFLAGS=$(CXXFLAGS)
ASFLAGS=-g
all:BalashEngine Makefile 
BalashEngine: $(OBJS) $(ASMOBJS)
	c++ $(OBJS) $(ASMOBJS) -o BalashEngine  -lSDL2 -lpthread
