#include "bookServer.h"
#include <vector>
#include "exc.h"
#include "book.h"

using namespace std;

bookServer::user::user(string _user, string _pass) : username(_user), password(_pass), isBlocked(true) {}

bookServer::bookServer(string _user, string _pass){
	users.push_back(user(_user, _pass));
	users[0].isBlocked = false;
}

bool bookServer::isValidUserId(int uid){
	return uid >=0 && uid < users.size();
}

bool bookServer::isBlockedUser(int uid){
	if(!isValidUserId(uid))
		throw badIdExc();
	return users[uid].isBlocked;
}

void bookServer::addBook(book b){
	for(int i = 0; i < books.size(); i++)
		if(books[i].getName() == b.getName())
			throw invalidNameExc();
	for(int i = 0; i < b.getWriters().size(); i++)
		usernameToId(b.getWriters()[i]);
	for(int i = 0; i < b.getWriters().size(); i++)
		users[usernameToId(b.getWriters()[i])].books.push_back(books.size());
	if(isBlockedUser(usernameToId(b.getWriters()[0])))
		b.block();
	books.push_back(b);
}

void bookServer::addUser(string _user, string _pass){
	for(int i = 0; i < users.size(); i++)
		if(_user == users[i].username)
			throw invalidUsernameExc();
	users.push_back(user(_user, _pass));
}

int bookServer::usernameToId(string _user){
	for(int i = 0; i < users.size(); i++)
		if(users[i].username == _user)
			return i;
	throw invalidUsernameExc();
}

int bookServer::booknameToId(string name){
	for(int i = 0; i < books.size(); i++)
		if(books[i].getName() == name)
			return i;
	throw invalidNameExc();
}

int bookServer::login(std::string _user, std::string _pass){
	for(int i = 0; i < users.size(); i++)
		if(users[i].username == _user){
			if(_pass == users[i].password)
				return i;
			else
				throw wrongPassExc();
		}
	throw invalidUsernameExc();
}

void bookServer::unblockBook(string bookname){
	books[booknameToId(bookname)].unblock();
	users[usernameToId(books[booknameToId(bookname)].getWriters()[0])].isBlocked = false;
}

void bookServer::blockBook(string bookname){
	books[booknameToId(bookname)].block();
}

book& bookServer::getBook(string bname, string uname){
	int bid = booknameToId(bname);
	if(uname == "admin" || (books[bid].isWriterOf(uname) && !books[bid].isBlocked()))
		return books[bid];
	throw invalidUsernameExc();
}

string bookServer::listOfBooks(string uname){
	string str = "";
	for(int i = 0; i < books.size(); i++)
		if(uname == "admin" || (books[i].isWriterOf(uname) && !books[i].isBlocked()))
			str += books[i].getName() + "\n";
	return str;
}
