#ifndef BOOKCHAPTER_H
#define BOOKCHAPTER_H

#include "version.h"
#include <string>

class bookChapter{
private:
	std::string name;
	std::vector <version> ver;
	int lastSyncedVer;
	version cur;
public:
	std::string getName();
	version const & getVer(int);
	void addVer(version);
	int lastVerNum();
	bookChapter(std::string _name);
	void newVer();
	int getLastSyncedVer();
	std::string show(int _ver = -1, std::string s = "\n");
	void reverseVer();
	void add(std::string, int = 0);
	void addFile(std::string, int = 0);
	void remove(int);
	void increaseLSV();
	std::string getVerStr(int, std::string);
};
#endif