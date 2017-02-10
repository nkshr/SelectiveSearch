#!/bin/sh

DEBUG_SS = y
TIME_SS = y

SRCS = main.cpp  selective_search selective_search_edge.cpp selective_search_vertex.cpp 
OBJS = $(addsuffix .o, $(basename $(SRCS)))

CAFFE_INC_DIR = -I/home/mg6/caffe/build/include -I/home/mg6/caffe/include -I/home/mg6/caffe/build
CAFFE_LIB_DIR = -L/home/mg6/caffe/build/lib/
CAFFE_LIB = -lcaffe

OPENCV_INC_DIR = `pkg-config --cflags-only-I opencv`
OPENCV_LIB_DIR = `pkg-config --libs-only-L opencv`
OPENCV_LIBS = `pkg-config --libs-only-l opencv`

CXX = /usr/bin/c++

ifeq ($(DEBUG_SS), y)
	DEFINES += -DDEBUG_SS
endif

ifeq ($(TIME_SS), y)
	CXX_DEFINES += -DTIME_SS
endif

INC_DIR = $(OPENCV_INC_DIR)

LDFLAGS = $(OPENCV_LIB_DIR)

LIBS = $(OPENCV_LIBS)

all: $(OBJS)
	$(CXX) $(DEFINES) $(INC_DIR) $(OBJS) $(LDFLAGS) $(LIBS) -o ss

%.o: %.cpp
	$(CXX) $(DEFINES) $(INC_DIR) -o $@ -c $^

.PHONY: clean
clean:
	rm -f *.o
	rm -f ./ss
	rm -f vis_img_*.png
	rm -f adj_table_*.txt
	rm -f edges_*.txt
	rm -f vertexs_*.txt
