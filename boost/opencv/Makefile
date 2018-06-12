# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
appname := my_cv
CXX = g++


CXXOPENCV = -I/rong/include/opencv -L/rong/lib/ -lopencv_core -lopencv_ccalib  -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_calib3d -lopencv_ccalib -lopencv_core -lopencv_datasets -lopencv_dnn -lopencv_dpm -lopencv_face -lopencv_features2d -lopencv_flann -lopencv_fuzzy -lopencv_highgui -lopencv_img_hash -lopencv_imgcodecs -lopencv_imgproc -lopencv_line_descriptor -lopencv_ml -lopencv_objdetect -lopencv_optflow -lopencv_phase_unwrapping -lopencv_photo -lopencv_plot -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_stereo -lopencv_stitching -lopencv_structured_light -lopencv_superres -lopencv_surface_matching -lopencv_text -lopencv_tracking -lopencv_video -lopencv_videoio  -lopencv_xfeatures2d -lopencv_ximgproc -lopencv_xobjdetect

CXXFLAGS = -Wall -g  -std=c++14  $(CXXOPENCV) 
# ****************************************************
# Targets needed to bring the executable up to date

srcfiles := $(shell find . -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

$(appname): $(objects)
	$(CXX)   $(CXXFLAGS)  -o $(appname) $(objects)
