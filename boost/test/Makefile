# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
appname := my_test
CXX = g++


CXXFLAGS = -Wall -g  -std=c++14  
# ****************************************************
# Targets needed to bring the executable up to date

srcfiles := $(shell find . -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

$(appname): $(objects)
	$(CXX)   $(CXXFLAGS)  -o $(appname) $(objects)
