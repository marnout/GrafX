/*
Simple X11 xwindow class
file : grafX.h
Neads Xlib.h : # apt install libx11-dev
compile:
g++ testGrafX.cpp grafX.cpp -o testGrafX -lX11 -std=c++11 -Wfatal-errors
Source: http://www.nd.edu/~dthain/courses/cse20211/fall2011/gfx
Licence: GNU-GPL
http://grognon.net/grafx.html
20/09/2015
*/

#ifndef MRD_GRFX_H
#define MRD_GRFX_H


#include <iostream>
#include <string.h>
#include <X11/Xlib.h>
using namespace std;

/*
#define AUTHOR "- Mourad Arnout -"
#define PROJECT "GRFX Graphics X11 class -"
#define VERSION "- 0.1 -"
*/

class GrafX 
{
public:
	// Open a new graphics window open(width, height, title)
	void open(int, int, const char *);
	// Flush all previous output to the window
	void flush();
	// Draw a point at (x, y)
	void point( int, int);
	// Draw a line from (x1, y1) to (x2, y2)
	void line( int, int, int, int);
	// Draw a line from previous position of line or point, to (x, y)
	void lineto(int, int);
	// Draw a rectangle(x, y, width, height)
	void rectangle(int, int, unsigned int, unsigned int);
	// Draw a simple text at (x, y)
	void text(int, int, char *);
	// Set the current drawing color
	void color( int, int, int);
	// Clear the window to current background color
	void clear();
	// Set the current background color
	void clear_color( int, int, int);
	// Check to see for an event is waiting
	int event_waiting();
	// wait for the user to press a key or mouse button
	// the result is the key char or 1, 2, 3 button pressed
	// (saved_xpos, saved_ypos) : mouse pointer
	char wait();
	// Change the title of the window
	void nameWindow(string);
	// Retrieve xpos and ypos
	int get_xpos();
	int get_ypos();
protected:
	int x, y; // last position after point, line
	Display 		*display=0; 
	Window  		window;
	GC      		gc;
	Colormap 	colormap;
	int fast_color_mode = 1;
	// mouse pointer coordinate after wait
	int saved_xpos = 0;
	int saved_ypos = 0;
};

#endif
