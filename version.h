#ifndef VERSION_H
#define VERSION_H

#include <vector>
#include <string>

class statement{
protected:
	int lineNum;
public:
	virtual void apply(std::vector <std::string> &);
	statement(int);
	virtual bool isAdd() = 0;
	virtual bool isRemove() = 0;
	int getLineNum();
	virtual std::string toString(std::string const &);
	virtual ~statement();
};

class add : public statement{
protected:
	std::string str;
public:
	add(std::string, int = 0);
	void apply(std::vector <std::string> &);
	bool isAdd();
	bool isRemove();
	std::string getStr();
	std::string toString(std::string const &);
};

class remove : public statement{
public:
	remove(int);
	void apply(std::vector <std::string> &);
	bool isAdd();
	bool isRemove();
	std::string toString(std::string const &);
};

class version{
private:
	std::vector <statement *> states;
public:
	void addState(statement *);
	void apply(std::vector <std::string> &);
	~version();
	version();
	version(version const &);
	std::string toString(std::string);
};
#endif