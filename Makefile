CROSS_COMPILE ?=

AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CXX	= $(CROSS_COMPILE)g++
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar

RM := rm -f

#for Debuging
#CFLAGS = -std=c90 -Wall -O0 -g3 -fmessage-length=0

#for Release
CFLAGS = -std=c90 -DNDEBUG -Wall -O3 -fmessage-length=0  

CXXFLAGS := -std=c++0x -O0 -g3 -fmessage-length=0 -Wall

LDFLAGS = -L. -lContainers

all: test lib

test: lib
	$(MAKE) -C test/ all

lib:
	$(MAKE) -C src/ all

clean:
	$(MAKE) -C test/ clean
	$(MAKE) -C src/ clean

.PHONY: all clean
