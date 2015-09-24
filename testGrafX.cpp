/*
Simple X11 xwindow class
file : testGrafX.cpp
Neads Xlib.h : # apt install libx11-dev
compile: 
g++ testGrafX.cpp grafX.cpp -o testGrafX -lX11 -std=c++11 -Wfatal-errors
Source: http://www.nd.edu/~dthain/courses/cse20211/fall2011/gfx
Licence: GNU-GPL
20/09/2015
*/
#include "grafX.h"
using namespace std;
#define AUTHOR "- Mourad Arnout -"
#define PROJECT "GRFX Graphics X11 class -"
#define VERSION "- 0.1 -"
//:main ======================================================================
int main()
{
	cout << "\n" << PROJECT << VERSION << AUTHOR << "- GNU GPL.\n";
	GrafX g; // instance de la classe GrafX
	// crée une fenêtre(width, height, title)
	g.open(240, 150, "GrafX  q: to quit");
	g.color(0, 255, 200); // couleur de tracé
	g.rectangle(10, 50, 160, 60); // trace un rectangle(x, y, w, h)
	g.line(10, 50, 85, 10); // pente du toit
	g.lineto(170, 50); // l'autre pente
	g.line(20, 100, 120, 100); // sol
	for(int step = 0; step < 4; step++) { // marches
		g.lineto(120 + 30*step, 100 + 18*step);
		g.lineto(150 + 30*step, 100 + 18*step);
	}
	// conversion de const string en cstring sous peine de warning 
	char text[] = "Watch your step!";
	g.color(255, 0, 0); // change la couleur du crayon
	g.text(20, 90, text); // ecrit un texte 
	while(true) { // attend qu'on appuie sur 'q'
		char c = g.wait();
		if(c == 'q') break;
		if(c == 'n') g.nameWindow("Watch your step");
		else cout << "pos: (" << g.get_xpos() << ", " << g.get_ypos() << ")\n";
	}				// il n'est pas besoin de tortiller du cul
	return 0; 	// pour chier droit
}
