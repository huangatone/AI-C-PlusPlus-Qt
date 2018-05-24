# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
appname := my_client
CXX = g++

CXXBOOST =  -L/rong/lib/boost -lboost_thread -lboost_system
CXXOPENCV = -I/rong/lib_github/brew/include -L/rong/lib_github/brew/lib/ -lopencv_core -lopencv_ccalib 

CXXFLAGS = -Wall -g  -std=c++14  $(CXXOPENCV) $(CXXBOOST)
# ****************************************************
# Targets needed to bring the executable up to date

srcfiles := $(shell find . -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

$(appname): $(objects)
	$(CXX)   $(CXXFLAGS)  -o $(appname) $(objects)
