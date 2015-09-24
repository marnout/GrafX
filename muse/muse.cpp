/*
File : muse.cpp
$ g++ muse.ccpp grafX.cpp -o muse -std=c++11 -lX11
*/
#define VERSION 0.1
#define AUTHOR "Mourad Arnout"
#define PROJECT "Muse"

#include "muse.h"
using namespace std;

//:Score ---------------------------------------------------------------------
Score::Score(string title)
{
	gx.open(W, H, title.c_str());
	draw_staff();
	gx.color(0, 255, 0);
}
//:note ----------------------------------------------------------------------
// The nearest note (<= forth)
int near(int n, int p) {return p - p%12 + n;}
// token a|b|c|d|e|f[is|es]['|,][0-9][0-9][.]
bool Score::note(Note& n, string token)
{
	n.name = token;
	regex re("([a-g](?:es)?(?:is)?)([',]{0,3})([0-9]{0,3})([.]?)");
	smatch m;
	int lift = 0;
	if(!regex_match(token, m, re)) return false;
	string  mlift = m[2], mlen = m[3],  mperiod = m[4];
	map<string,int>::iterator it = y_map.find(m[1]);
	if(it == y_map.end()) return false;
	// n.x
	if(!mlen.empty()) {
		n.x = whole / stoi(mlen);
		if(mperiod == ".") n.x += n.x /2;
	} else {
		if(!notes.empty()) n.x = notes.back().x;
		else n.x = 4;
	}
	// n.y
	n.y = y_map[m[1]]; 
	if(!mlift.empty()) {
		if(mlift.at(0) == ',') lift = -12*mlift.size();
		else lift = 12*mlift.size();
	}
	if(notes.empty()) n.y = n.y + lift;
	else n.y = near(n.y, notes.back().y) + lift;
}
//:parse ---------------------------------------------------------------------
void Score::parse(string line)
{ // lexer
	stringstream flot(line);
	string token;
	while(flot >> token) {
		Note n;
		if(!notes.empty()) {
			n.x = notes.back().x;
			n.y = notes.back().y;
		}
		if(token.at(0) == '%') break;
		if(token == "|") {
			n.cmd = '|';
			notes.push_back(n);
		} else
		if(token == "n") {
			n.cmd = 'n';
			notes.push_back(n);
		} else if(token == "h") {
			n.cmd = 'h';
			notes.push_back(n);
		} else {
			if(note(n, token)) notes.push_back(n);
			else cout << "Bug : " << token << endl;
		}
	} // while token
}
//:print ---------------------------------------------------------------------
void Score::print()
{
	cout << "\nNotes :\n";
	for(auto &n: notes) 
		if(n.cmd == 0)
			cout << n.name << ":(" << n.x << ", " << n.y << ") ";
		else if(n.cmd == '|') cout << "|\n";
		else if(n.cmd == 'n' || n.cmd == 'h') cout << endl << endl;;
	cout << endl;
}
//:draw_staff ----------------------------------------------------------------
void Score::draw_staff()
{
	gx.color(80, 80, 80);
	gx.line(xo/2, proj_y(7), W - xo/2, proj_y(7));  	// E
	gx.text(xo/2, proj_y(8), "F");
	gx.line(xo/2, proj_y(10), W - xo/2, proj_y(10)); 	// G
	gx.text(xo/2, proj_y(12), "A");
	gx.line(xo/2, proj_y(14), W - xo/2, proj_y(14));	// B
	gx.text(xo/2, proj_y(15), "C");
	gx.line(xo/2, proj_y(17), W - xo/2, proj_y(17));	// D
	gx.text(xo/2, proj_y(18), "E");
	gx.line(xo/2, proj_y(20), W - xo/2, proj_y(20));	// F
	gx.color(0, 255, 0);
}
//:darw_bar
void Score::draw_bar()
{
	gx.color(80, 80, 80);
	gx.line(posx, proj_y(7), posx, proj_y(20));
	gx.color(0, 255, 0);
}
//:draw ----------------------------------------------------------------------
void Score::draw()
{
	for(auto &n: notes) {
		if(n.y == int_min) {
			posx += n.x;
		} else if(n.cmd == 'h') posx = xo;
		else if(n.cmd == 'n') {
			YA += deltaY; posx = xo;
			draw_staff();
		}
		else if(n.cmd == '|') draw_bar();
		else {
			int Y = proj_y(n.y);
			int Yo = proj_y(posy);
		//	gx.line(posx, Yo, posx, Y);
			gx.rectangle(posx, Y - 2,  4,  4);
			gx.line(posx, Y, posx + n.x, Y);
			posx += n.x;
			posy = Y;
		}
	}
	gx.flush();
}
//:load-----------------------------------------------------------------------
void Score::load(string filename)
{
	ifstream f(filename);
	if(f.is_open()) {
		string line;
		size_t pos;
		while(getline(f, line)) {
			if((pos = line.find("Title:")) == 0) {
				nameWindow(line.substr(7));
				continue;
			}
			parse(line);
		}
	}
	else cout << "Bug : " << filename << " absent\n";
	f.close();
}
//:wait ----------------------------------------------------------------------
void Score::wait()
{
	while(true) { 
		char c = gx.wait();
		if(c == 'q') break;
	}				
}
//:nameWindow
void Score::nameWindow(string title)
{
	gx.nameWindow(title);
}
// main ======================================================================
int main(int argc, char * argv[])
{
	cout << "\n" << PROJECT << " - " << VERSION << " - ";
	cout << AUTHOR << " - GNU-GPL -\n";
	cout << "Type 'q' to quit\n";
	Score score("MuSe   (q): quitte");
	string filename;
	if(argc == 2) filename = argv[1];
	else filename = "ptitchval.mu";
	score.nameWindow(filename);
	score.load(filename);
	score.print();
	score.draw();
	score.wait();
	return 0;
}

