#CROSS_COMPILE ?=

AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
#CC	= $(CROSS_COMPILE)gcc
CXX	= $(CROSS_COMPILE)g++
#CPP	= $(CC) -E
#AR	= $(CROSS_COMPILE)ar

RM := rm -f

#for Debuging
#CFLAGS = -std=c90 -Wall -O0 -g3 -fmessage-length=0

#for Release
#CFLAGS = -std=c90 -DNDEBUG -Wall -O3 -fmessage-length=0  

CXXFLAGS := -std=c++0x -O0 -g3 -fmessage-length=0 -Wall -I../src/include
 
LDFLAGS = -L../src -lContainers

all: test_array test_forwardList test_list

test_array: array_test.o container_test.o
	$(CXX) -o test_array array_test.o container_test.o $(LDFLAGS)

test_forwardList: forwardList_test.o container_test.o
	$(CXX) -o test_forwardList forwardList_test.o container_test.o $(LDFLAGS)

test_list: list_test.o container_test.o
	$(CXX) -o test_list list_test.o container_test.o $(LDFLAGS)

array_test.o: array_test.cpp container_test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

forwardList_test.o: forwardList_test.cpp container_test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

list_test.o: list_test.cpp container_test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

container_test.o: container_test.cpp container_test.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) test_* *.o

.PHONY: all clean
