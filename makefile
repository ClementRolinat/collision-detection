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

collision-detect-fix: collision-detect-fix.o vibes.o tools.o node.o
	$(CXX) $^ -o collision-detect-fix $(CXXFLAGS) $(LIBS)

collision-detect-mov: collision-detect-mov.o vibes.o tools.o node.o
	$(CXX) $^ -o collision-detect-mov $(CXXFLAGS) $(LIBS)

collision-detect-waypts: collision-detect-waypts.o vibes.o tools.o node.o
	$(CXX) $^ -o collision-detect-waypts $(CXXFLAGS) $(LIBS)

path-planning-fix: path-planning-fix.o vibes.o tools.o node.o
	$(CXX) $^ -o path-planning-fix $(CXXFLAGS) $(LIBS)

path-planning-mov: path-planning-mov.o sepProj.o vibes.o tools.o node.o
	$(CXX) $^ -o path-planning-mov $(CXXFLAGS) $(LIBS)

vibes.o: vibes.h

tools.o: tools.h

node.o: node.h

sepProj.o: sepProj.h

%.o: %.c
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(LIBS)

clean:
	rm -rf *.bak rm -rf *.o collision-detect-fix collision-detect-mov collision-detect-waypts path-planning-fix path-planning-mov
	
