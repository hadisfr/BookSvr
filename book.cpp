#include "book.h"
#include <vector>
#include "exc.h"
#include "tools.h"

using namespace std;

book::book(string _name, vector <string> _writers) : name(_name), writers(_writers), blocked(false) {}

void book::addChapter(bookChapter ch, int pos){
	if(pos == -2) //end
		chapters.push_back(ch);
	else
		chapters.insert(chapters.begin() + 1 + pos, ch);
}

vector <string> book::getWriters(){
	return writers;
}

void book::setName(string _name){
	name = _name;
}

void book::block(){
	blocked = true;
}

void book::unblock(){
	blocked = false;
}

bool book::isBlocked(){
	return blocked;
}

string book::getName(){
	return name;
}

bool book::isWriterOf(string uname){
	for(int i = 0; i < writers.size(); i++)
		if(writers[i] == uname)
			return true;
	return false;
}

string book::listChapters(){
	string str = "";
	for(int i = 0; i < chapters.size(); i++)
		str += name + " :: " + chapters[i].getName() + "\n";
	return str;
}

string book::detailToStr(){
	string str = Tools::itos(writers.size());
	for(int i = 0; i < writers.size(); i++)
		str += " " + writers[i];
	str += " " + getName();
	return str;
}
