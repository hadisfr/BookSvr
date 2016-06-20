#ifndef BOOK_H
#define BOOK_H

#include <vector>
#include <string>
#include "bookChapter.h"

class book{
private:
	std::string name;
	std::vector <std::string> writers;
	bool blocked;
public:
	std::vector <bookChapter> chapters;
	book(std::string _name, std::vector <std::string> _writers);
	void addChapter(bookChapter, int afterPos = -2);
	void exportHTML();
	std::vector <std::string> getWriters();
	void block();
	void unblock();
	bool isBlocked();
	std::string getName();
	bool isWriterOf(std::string);
	void setName(std::string);
	std::string listChapters();
	std::string detailToStr();
};

#endif