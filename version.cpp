#include <vector>
#include "version.h"
#include "exc.h"
#include <iostream>
#include "tools.h"

using namespace std;

statement::statement(int _lineNum) : lineNum(_lineNum) {
	if(lineNum < 0)
		throw badLineNumExc();
}

bool add::isAdd(){return true;}

bool add::isRemove(){return false;}

bool remove::isAdd(){return false;}

bool remove::isRemove(){return true;}

add::add(string _str, int _n) : statement(_n), str(_str) {}

remove::remove(int _n) : statement(_n) {}

void statement::apply(vector <string> &v){
	if(lineNum > v.size())
		throw badLineNumExc();
}

void remove::apply(vector <string> &v){
	statement::apply(v);
	if(lineNum == v.size())
		throw badLineNumExc();
	v.erase(v.begin() + lineNum);
}

void add::apply(vector <string> &v){
	statement::apply(v);
	v.insert(v.begin() + lineNum, str);
}

string statement::toString(string const &seed){
	return seed + " " + Tools::itos(getLineNum());
}

string add::toString(string const &seed){
	return  "add " + statement::toString(seed) + " " + getStr() + "\n";
}

string remove::toString(string const &seed){
	return "remove " + statement::toString(seed) + "\n";
}

void version::addState(statement *s){
	states.push_back(s);
}

void version::apply(vector <string> &str){
	for(int i = 0; i < states.size(); i++)
		states[i]->apply(str);
}

int statement::getLineNum(){return lineNum;}

string add::getStr(){return str;}

statement::~statement(){}

version::~version(){
	for(int i = 0; i < states.size(); i++)
		delete states[i];
}

version::version(){}

version::version(version const &v){
	for(int i = 0; i < v.states.size(); i++){
		if(v.states[i]->isAdd())
			addState(new class::add(((class::add*)(v.states[i]))->getStr(), v.states[i]->getLineNum()));
		else
			addState(new class::remove(v.states[i]->getLineNum()));
	}
}

string version::toString(string seed){
	//string str = seed + " " + Tools::itos(states.size()) + "\n";
	string str = Tools::itos(states.size()) + "\n";
	for(int i = 0; i < states.size(); i++)
		str += states[i]->toString(seed);
	return str;
}
