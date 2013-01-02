# image2dxf.c Makefile
# By Nathan W Brei
# January 28, 2010
# 6.087 Final Project
#
# This makefile presupposes a MacBook Pro running Mac OS X 10.6.2, 
# with OpenCV installed via the instructions at 
# http://opencv.willowgarage.com/wiki/Mac_OS_X_OpenCV_Port
#
# To hand-compile the OpenCV sample file contours.c:
#   export PKG_CONFIG_PATH=/usr/local/Cellar/opencv/2.3.1a/lib/pkgconfig
#   export LD_LIBRARY_PATH=/usr
#   gcc `pkg-config --cflags opencv` `pkg-config --libs opencv` -arch i386 contours.c

all: main.c brain.c scribe.c toolchest.c
	gcc -g -O0 -Wall \
	`pkg-config --cflags opencv` `pkg-config --libs opencv` \
	-o image2dxf main.c brain.c scribe.c toolchest.c
clean:
	rm -rf *o image2dxf
