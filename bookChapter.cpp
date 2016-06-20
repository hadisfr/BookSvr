#include "bookChapter.h"
#include "version.h"
#include <vector>
#include <string>
#include "exc.h"
#include "tools.h"

using namespace std;

bookChapter::bookChapter(string _name) : name(_name), lastSyncedVer(-1) {};

void bookChapter::newVer(){
	ver.push_back(cur);
	cur = version();
}

string bookChapter::show(int _ver, string s){
	if(_ver == -1)
		_ver  = ver.size();
	if(_ver > ver.size() || _ver < 0)
		throw badIdExc();
	vector <string> res;
	for(int i = 0; i < _ver && i < ver.size(); i++)
		ver[i].apply(res);
	if(_ver == ver.size())
		cur.apply(res);
	string str = "";
	for(int i = 0; i < res.size(); i++)
		str += Tools::itos(i) + string("\t") + res[i] + string("\n");
	return str;
}

string bookChapter::getName(){
	return name;
}

void bookChapter::reverseVer(){
	if(ver.size())
		ver.pop_back();
	if(lastSyncedVer > 0 && lastSyncedVer >= ver.size())
		lastSyncedVer--;
	cur = version();
}

void bookChapter::add(string str, int n){
	cur.addState(new class::add(str, n));
}

void bookChapter::remove(int n){
	cur.addState(new class::remove(n));
}

int bookChapter::lastVerNum(){
	return ver.size();
}

int bookChapter::getLastSyncedVer(){
	return lastSyncedVer;
}

version const & bookChapter::getVer(int index){
	if(index < 0 || index >= ver.size())
		throw badIdExc();
	return ver[index];
}

void bookChapter::increaseLSV(){
	lastSyncedVer++;
}

void bookChapter::addVer(version v){
	ver.push_back(v);
}

string bookChapter::getVerStr(int n, string seed){
	if(n < 0 || n >= ver.size())
		throw badIdExc();
	//seed + " " + Tools::itos(n) + "\n"
	return  seed + " " + ver[n].toString(seed);
}
