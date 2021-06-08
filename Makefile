BUILDDIR=build
OPTIMIZE=-Ofast -msse4 -mfpmath=sse -march=native -mavx2 -mpopcnt -mbmi2 -fprofile-use
ELFNAME=BalashEngine#checkout to branch cpp

CXX=c++
LIBS=`pkg-config --libs --cflags libpng`

CFLAGS= $(OPTIMIZE) $(LIBS) -lpthread -lSDL2 -g -fno-pie -fno-PIC#remove -g when release 
CPPFLAGS= $(OPTIMIZE) -g -fno-pie -fno-PIC  #remove -g when release
ASFLAGS=-g

SOURCE:=$(wildcard *.cpp)  
ASM:=$(wildcard *.S) 
OBJS:=$(SOURCE:.cpp=.o) 
ASMOBJS:=$(ASM:.S=.o)

default:all run
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

