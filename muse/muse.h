/*
File : muse.h
$ g++ muse.ccpp grafX.cpp -o muse -std=c++11 -lX11
*/

#ifndef MRD_MUSE_H
#define MRD_MUSE_H

//
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <map>
#include <fstream>
#include "grafX.h"
//
using namespace std;
const int int_min = -32767;
//:map_note ------------------------------------------------------------------
map<string,int>y_map = {
	{"aes", 11}, {"a", 0}, {"ais", 1},
	{"bes", 1}, {"b", 2}, {"bis", 3},
	{"ces", 2}, {"c", 3}, {"cis", 4},
	{"des", 4}, {"d", 5}, {"dis", 6},
	{"ees", 6}, {"e", 7}, {"eis", 8},
	{"fes", 7}, {"f", 8}, {"fis", 9},
	{"ges", 9}, {"g", 10}, {"gis", 11},
	{"r", int_min}
};
//:Note ======================================================================
struct Note {
	int x; // 1=whole, 2=half, 4=quater, 8=eight, ...
	int y; // 0=a, 1=ais, 2=b, .., 12=gis
	string name;
	char cmd = 0;
	Note(int a=4, int b=0, string s="") {x = a; y = b; name = s;}
};
//:score =====================================================================
// like \score in lilypond
class Score
{
public:
	Score(string);
	bool note(Note&, string);
	void parse(string);
	void print();
	void draw_staff();
	void draw_bar();
	void draw();
	void load(string);
	void wait();
	void nameWindow(string);
protected:
	vector<Note>notes;
	GrafX gx;
	int W = 800; 	int H = 600;
	int dy = 8; int YA = 24*dy ; int deltaY = 24*dy;
	int whole = 96;
	int xo = whole / 4;
	int posx = xo; 	int posy = 7;
	int proj_y(int y) {return YA - y*dy;}
};

#endif

