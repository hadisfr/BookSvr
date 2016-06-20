#ifndef BOOKCLIENT_H
#define BOOKCLIENT_H

#include <vector>
#include "book.h"

class bookClient{
private:
	std::vector <book> books;
public:
	void addBook(book);
	int booknameToId(std::string);
	book& getBook(std::string bname);
	void deleteBook(std::string);
};
#endif