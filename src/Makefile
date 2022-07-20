CROSS_COMPILE ?=

AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar

RM := rm -f

#for Debuging
#CFLAGS = -std=c90 -Wall -O0 -g3 -fmessage-length=0

#for Release
CFLAGS = -std=c90 -DNDEBUG -Wall -O3 -fmessage-length=0  

all: libContainers.a

libContainers.a: array.o forwardList.o list.o
	$(AR) -r $@ array.o forwardList.o list.o

array.o:array.c include/containers/array.h include/containers/container.h
	$(CC) $(CFLAGS) -c $< -o $@

forwardList.o:forwardList.c include/containers/forwardList.h include/containers/container.h
	$(CC) $(CFLAGS) -c $< -o $@

list.o:list.c include/containers/list.h include/containers/container.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) libContainers.a *.o

.PHONY: all clean
