#ifndef BOOKSERVER_H
#define BOOKSERVER_H

#include "book.h"
#include <vector>

class bookServer{
private:
	struct user{
		std::string username;
		std::string password;
		bool isBlocked;
		std::vector <int> books;
		user(std::string _user, std::string _pass);
	};
	std::vector <user> users;
	std::vector <book> books;
public:
	bookServer(std::string _user, std::string _pass);
	bool isValidUserId(int uid);
	bool isBlockedUser(int uid);
	void addBook(book);
	void addUser(std::string _user, std::string _pass);
	int usernameToId(std::string);
	int booknameToId(std::string);
	int login(std::string _user, std::string _pass);
	void unblockBook(std::string);
	void blockBook(std::string);
	book& getBook(std::string bname, std::string uname);
	std::string listOfBooks(std::string);
};

#endif