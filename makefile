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

all: step1.o vibes.o
	$(CXX) $^ -o step1 $(CXXFLAGS) $(LIBS)

ibexSLAM: ibexSLAM.o vibes.o tools.o
	$(CXX) $^ -o ibexSLAM $(CXXFLAGS) $(LIBS)

vibes.o: vibes.h

%.o: %.c
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(LIBS)

clean:
	rm -rf *.bak rm -rf *.o step1
	
