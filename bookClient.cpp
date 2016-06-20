#include "bookClient.h"
#include <vector>
#include "book.h"
#include "exc.h"

using namespace std;

void bookClient::addBook(book b){
	books.push_back(b);
}

book& bookClient::getBook(string bname){
		return books[booknameToId(bname)];
}

int bookClient::booknameToId(string name){
	for(int i = 0; i < books.size(); i++)
		if(books[i].getName() == name)
			return i;
	throw invalidNameExc();
}

void bookClient::deleteBook(string name){
	try{
		books.erase(books.begin() + booknameToId(name));
	} catch(invalidNameExc){}
}
