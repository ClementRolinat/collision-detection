SRCS=$(wildcard *.cpp)
BINS=$(SRCS:.cpp=)



CXX = g++
CXXFLAGS = $(shell pkg-config --cflags ibex) 
LIBS	 = $(shell pkg-config --libs  ibex)


ifeq ($(DEBUG), yes)
CXXFLAGS := $(CXXFLAGS) -O0 -g -pg -Wall
else
CXXFLAGS := $(CXXFLAGS) 
endif

step1: step1.o vibes.o tools.o
	$(CXX) $^ -o step1 $(CXXFLAGS) $(LIBS)

step2: step2.o vibes.o tools.o
	$(CXX) $^ -o step2 $(CXXFLAGS) $(LIBS)

step3: step3.o vibes.o tools.o
	$(CXX) $^ -o step3 $(CXXFLAGS) $(LIBS)

vibes.o: vibes.h

tools.o: tools.h

%.o: %.c
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(LIBS)

clean:
	rm -rf *.bak rm -rf *.o step1
	
