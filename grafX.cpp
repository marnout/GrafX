/*
Simple X11 xwindow class
file : grafX.cpp
Neads Xlib.h : # apt install libx11-dev
compile:
g++ testGrafX.cpp grafX.cpp -o testGrafX -lX11 -std=c++11 -Wfatal-errors
Source: http://www.nd.edu/~dthain/courses/cse20211/fall2011/gfx
Licence: GNU-GPL
http://arad.free.fr/grafx.html
20/09/2015
*/
#include "grafX.h"
using namespace std;
//:open--------------------------------------------------------------------
void GrafX::open(int width, int height, const char *title)
{
	display = XOpenDisplay(0);
	if(!display) {
		cerr <<"open: unable to open the graphics window.\n";
		exit(1);
	}
	/*
		Visual *visual = DefaultVisual(display,0);
		if(visual && visual->class==TrueColor) {
		fast_color_mode = 1;
		} else {
		fast_color_mode = 0;
		}
	 */
	fast_color_mode = 1;
	int blackColor = BlackPixel(display, DefaultScreen(display));
	int whiteColor = WhitePixel(display, DefaultScreen(display));
	window = XCreateSimpleWindow(display, DefaultRootWindow(display), 
			0, 0, width, height, 0, blackColor, blackColor);
	XSetWindowAttributes attr;
	attr.backing_store = Always;
	XChangeWindowAttributes(display,window,CWBackingStore,&attr);
	XStoreName(display,window,title);
	XSelectInput(display, window, 
			StructureNotifyMask|KeyPressMask|ButtonPressMask);
	XMapWindow(display,window);
	gc = XCreateGC(display, window, 0, 0);
	colormap = DefaultColormap(display,0);
	XSetForeground(display, gc, whiteColor);
	// Wait for the MapNotify event
	for(;;) {
		XEvent e;
		XNextEvent(display, &e);
		if (e.type == MapNotify)
			break;
	}
}
//:point ---------------------------------------------------------------------
// Draw a single point at (x,y)
void GrafX::point(int x, int y)
{
	XDrawPoint(display, window, gc, x, y);
	this->x = x; this->y = y;
}
//:line-----------------------------------------------------------------------
// Draw a line from (x1,y1) to (x2,y2)
void GrafX::line(int x1, int y1, int x2, int y2)
{
	XDrawLine(display, window, gc, x1, y1, x2, y2);
	x = x2; y = y2;
}
//:lineto --------------------------------------------------------------------
// Draw a line from the last pos of line or point to (x2, y2)
void GrafX::lineto(int x2, int y2)
{
	XDrawLine(display, window, gc, x, y, x2, y2);
	x = x2; y = y2;
}
//:rectangle -----------------------------------------------------------------
// Draw a rectangle (x, y, with, height)
void GrafX::rectangle(int x, int y, unsigned int w, unsigned int h)
{
	XDrawRectangle(display, window, gc, x, y, w, h);
}
//: text
void GrafX::text(int x, int y, char *text)
{
	XDrawString(display, window, gc, x, y, text, strlen(text));
}
//:color ---------------------------------------------------------------------
/* Change the current drawing color. */
void GrafX::color(int r, int g, int b)
{
	XColor color;
	if(fast_color_mode) {
		// If this is a truecolor display, we can just pick the color directly. 
		color.pixel = ((b&0xff) | ((g&0xff)<<8) | ((r&0xff)<<16));
	} else {
		// Otherwise, we have to allocate it from the colormap of the display.
		color.pixel = 0;
		color.red = r<<8;
		color.green = g<<8;
		color.blue = b<<8;
		XAllocColor(display,colormap,&color);
	}
	XSetForeground(display, gc, color.pixel);
}
//:clear ---------------------------------------------------------------------
// Clear the graphics window to the background color
void GrafX::clear()
{
	XClearWindow(display,window);
}
//:clear_color ---------------------------------------------------------------
// Change the current background color
void GrafX::clear_color(int r, int g, int b)
{
	XColor color;
	color.pixel = 0;
	color.red = r<<8;
	color.green = g<<8;
	color.blue = b<<8;
	XAllocColor(display,colormap,&color);
	XSetWindowAttributes attr;
	attr.background_pixel = color.pixel;
	XChangeWindowAttributes(display,window,CWBackPixel,&attr);
}
//:event_waiting()
//
int GrafX::event_waiting()
{
	XEvent event;
	flush();
	while (1) {
		if(XCheckMaskEvent(display,-1,&event)) {
			if(event.type==KeyPress) {
				XPutBackEvent(display,&event);
				return 1;
			} else if (event.type==ButtonPress) {
				XPutBackEvent(display,&event);
				return 1;
			} else {
				return 0;
			}
		} else {
			return 0;
		}
	}
}
//:wait-----------------------------------------------------------------------
// Wait for the user to press a key or mouse button
char GrafX::wait()
{
	XEvent event;
	flush();
	while(1) {
		XNextEvent(display,&event);
		if(event.type==KeyPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return XLookupKeysym(&event.xkey,0);
		} else if(event.type==ButtonPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return event.xbutton.button;
		}
	}
}
//:get_xpos, get_ypos --------------------------------------------------------
int GrafX::get_xpos()
{
	return saved_xpos;
}
//
int GrafX::get_ypos()
{
	return saved_ypos;
}
//:flush ---------------------------------------------------------------------
// Flush all previous output to the window 
void GrafX::flush()
{
	XFlush(display);
}
//:nameWindow-----------------------------------------------------------------
// Change the windows' title
void GrafX::nameWindow(string title)
{
	XStoreName(display,window,title.c_str());
}
	
