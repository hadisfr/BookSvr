#include "exc.h"
#include "network_exception.h"
#include <iostream>
#include "tools.h"
#include "client_socket.h"
#include "book.h"
#include "bookClient.h"

using namespace std;

class ui{
public:
	static void help();
	static void cmdErr(string msg = "");
	static string rebuildCmd(stringstream &, string);
	static void procResponse(string);
	static void svrCmdProc(string, bookClient &);
	static void statementProc(string msg, bookClient &);
};

string its(int n){
	stringstream ss;
	ss << n;
	string str;
	ss >> str;
	return str;
}

int sti(string str){
	stringstream ss(str);
	int n;
	ss >> n;
	return n;
}

int main(int argc, char *argv[]){
	if (argc < 3){
		cerr << "Valid operation: " << argv[0] << " server_address server_port [echo]\n";
		exit(-1);
	}
	cout << "BookSvr V 1 :: CLT\n"
		<< "It's recommended to use \"echo\" while feeding program input by file.\n"
		<< "Use \"help\" after logging in to see list of commands.\n\n";
	bool echo = false;
	if(argc > 3 && !strcmp(argv[3], "echo"))
		echo = true;

	string svrAdd = string(argv[1]);
	int portNum = Tools::mystoi(argv[2]);
	if(svrAdd == "localhost")
		svrAdd = "127.0.0.1";

	ClientSocket socket(svrAdd, portNum);
	cout << "SVR: " << svrAdd << ":" << portNum << endl << "Connection established.\n\n";

	do{	//login
		string user;
		do{
			string p;
			char ch;
			cout << "New User? (n/y)\t";
			cin >> ch;
			if(echo)
				cout << ch << endl;
			cout << "Username:\t";
			cin >> user;
			if(echo)
				cout << user << endl;
			cout << "Password:\t";
			cin >> p;
			if(echo)
				cout << endl;
			if(ch == 'n'){
				socket.send("login " + user + " " + p);
				string str = socket.receive();
				if(str != "validated"){
					cout << "Login failed.\n";
					continue;
				}
			}
			else if(ch == 'y'){
				socket.send("signup " + user + " " + p);
				string str = socket.receive();
				if(str != "validated"){
					cout << "Signup failed.\n";
					continue;
				}
			}
			else
				continue;
			break;
		}while(true);
		cout << "\nHello " << user << "!\n";

		cin.ignore();

		bookClient bookClt;

		try{
			while(true){
				cout << "\n> ";
				string str;
				getline(cin, str);
				stringstream ss(str);
				string cmd;
				ss >> cmd;
				if(echo)
					cout << str << endl;
				if(cmd == "logout"){
					socket.send("logout");
					cout << socket.receive() << endl;
					break;
					if(socket.receive() == "ok")
						cout << "Bye " << user << "!\n";
					else
						ui::cmdErr();
					break;
				}
				else if(cmd == "help"){
					ui::help();
				}
				else if(cmd == "newBook"){
					socket.send(ui::rebuildCmd(ss, cmd));
					ui::procResponse(socket.receive());
				}
				else if(cmd == "loadBook"){
					string t;
					ss >> t;
					bookClt.deleteBook(t);
					socket.send(cmd + " " + t);
					/*if(socket.receive() != "ok")
						throw unknownExc();*/
					string str = socket.receive();
					stringstream sss(str);
					getline(sss, str);
					ui::svrCmdProc(str, bookClt); //newBook
					getline(sss, str);
					int n = sti(str), m = n;
					while(m--){
						getline(sss, str);
						ui::svrCmdProc(str, bookClt); //addChapter
					}
					while(n--){
						getline(sss, str);
						m = sti(str);
						while(m--){
							getline(sss, str);
							ui::svrCmdProc(str, bookClt); //addVer
						}
					}
					cout << "Done!\n";
				}
				else if(cmd == "listChapters"){
					string bname;
					ss >> bname;
					cout << bookClt.getBook(bname).listChapters();
				}
				else if(cmd == "changeName"){
					string o, n;
					ss >> o >> n;
					bookClt.deleteBook(o);
					socket.send(ui::rebuildCmd(ss, cmd + " " + o));
					ui::procResponse(socket.receive());
					cout << "Use \"loadBook\".\n";
				}
				else if(cmd == "addChapter"){
					socket.send(ui::rebuildCmd(ss, cmd));
					ui::procResponse(socket.receive());
					cout << "use \"loadBook\"\n";
				}
				else if(cmd == "show"){
					string bname;
					int cnum;
					ss >> bname >> cnum;
					cout << bname << " :: " << bookClt.getBook(bname).chapters[cnum].getName() << " :: v " << bookClt.getBook(bname).chapters[cnum].lastVerNum() << "\n==========" << endl;
					cout << bookClt.getBook(bname).chapters[cnum].show();
				}
				else if(cmd == "showPrev"){
					string bname;
					int cnum, t;
					ss >> bname >> cnum >> t;
					cout << bname << " :: " << bookClt.getBook(bname).chapters[cnum].getName() << " :: v " << t << "\n==========" << endl;
					cout << bookClt.getBook(bname).chapters[cnum].show(t);
				}
				else if(cmd == "newVer"){
					string bname;
					int cnum;
					ss >> bname >> cnum;
					bookClt.getBook(bname).chapters[cnum].newVer();
					cout << "Done!\n";
				}
				else if(cmd == "revVer"){
					string bname;
					int cnum;
					ss >> bname >> cnum;
					bookClt.getBook(bname).chapters[cnum].reverseVer();
					cout << "Done!\n";
				}
				else if(cmd == "remove"){
					string bname;
					int cnum, lnum;
					ss >> bname >> cnum >> lnum;
					bookClt.getBook(bname).chapters[cnum].remove(lnum);
					cout << "Done!\n";
				}
				else if(cmd == "add"){
					string bname, text;
					int cnum, lnum;
					ss >> bname >> cnum >> lnum >> ws;
					getline(ss, text);
					bookClt.getBook(bname).chapters[cnum].add(text, lnum);
					cout << "Done!\n";
				}
				else if(cmd == "addFile"){
					string bname, text;
					int cnum, lnum;
					ss >> bname >> cnum >> lnum;
					getline(ss, text);
					bookClt.getBook(bname).chapters[cnum].addFile(text, lnum);
					cout << "Done!\n";
				}
				else if(cmd == "list"){
					socket.send(cmd);
					cout << socket.receive();
				}
				else if(cmd == "sync"){
					string bname;
					int cnum;
					ss >> bname >> cnum;
					int lsv = bookClt.getBook(bname).chapters[cnum].getLastSyncedVer(), lv = bookClt.getBook(bname).chapters[cnum].lastVerNum(), sv;
					socket.send("getLastVerNum " + bname + " " + Tools::itos(cnum));
					sv = Tools::mystoi(socket.receive().c_str());
					if(sv == -1)
						throw unknownExc();
					cerr << "Local LastSyncedVer = " << lsv << ", bookServerVersion = " << sv << ", LocalVersion = " << lv <<".\n";
					if(lsv + 1 < sv)
						cout << "Local version is out of date. Use \"loadBook\".\n";
					else if(lsv + 1 > sv)
						cout << "Err. Use \"loadBook\".\n";
					//so else, lsv + 1 == sv
					else if(lv == sv)
						cout << "Already synced!\n";
					else{
						for(int i = lsv + 1; i < lv; i++){
							socket.send("addVer " + bookClt.getBook(bname).chapters[cnum].getVerStr(i, bname + " " + its(cnum)));
							if(socket.receive() != "ok")
								throw unknownExc();
							bookClt.getBook(bname).chapters[cnum].increaseLSV();
							cout << "Version " << i << " synced.\n";
							cerr << "Local LastSyncedVer = " << lsv << ", bookServerVersion = " << sv << ", LocalVersion = " << lv <<".\n";
						}
					}
				}
				else if(cmd == "unblock"){
					socket.send(ui::rebuildCmd(ss, cmd));
					ui::procResponse(socket.receive());
				}
				else if(cmd == "publish"){
					socket.send(ui::rebuildCmd(ss, cmd));
					ui::procResponse(socket.receive());
				}
				else if(cmd == "revVerSvr"){
					string bname;
					int cnum;
					ss >> bname >> cnum;
					bookClt.getBook(bname).chapters[cnum].reverseVer();
					socket.send(cmd + " " + bname + " " + Tools::itos(cnum));
					ui::procResponse(socket.receive());
				}
				else
					cout << "invalid command. \"help\" for list of commands.\n";
				//socket.status();
			}
		} catch(...){
			ui::cmdErr();
			socket.send("logout");
			throw;
			//exit(1);
		}

		cout << "\nexit? (y/n)\t";
		char ch;
		cin >> ch;
		if(echo)
			cout << ch << endl;
		cout << endl;
		if(ch == 'y')
			break;
	}while(true);

	return 0;
}

void ui::svrCmdProc(string str, bookClient &clt){
	if(str == "")
		return;
	cerr << "SVR: " << str << endl;
	stringstream ss(str);
	string cmd;
	ss >> cmd;
	if(cmd == "newBook"){
		int n;
		ss >> n;
		if(n < 1){
			cmdErr("empty vector");
			throw wrongPassExc();
		}
		vector <string> w;
		while(n--){
			string t;
			ss >> t;
			w.push_back(t);
		}
		string t;
		ss >> t;
		clt.addBook(book(t, w));
	}
	else if(cmd == "addChapter"){
		string bname, cname;
		int t;
		ss >> bname >> cname >> t;
		clt.getBook(bname).addChapter(bookChapter(cname), t - 1);
	}
	else if(cmd == "addVer"){
		int n, cnum;
		string bname;
		ss >> bname >> cnum >> n;
		//ss.ignore();
		while(n--){
			string str;
			getline(ss, str);
			ui::statementProc(str, clt);
		}
		clt.getBook(bname).chapters[cnum].newVer();
	}
	else
		throw invalidCmdExc();
	//cerr << cmd << " done.\n";
}

void ui::statementProc(string str, bookClient &clt){
	stringstream ss(str);
	string cmd;
	ss >> cmd;
	if(cmd == "remove"){
		string bname;
		int cnum, lnum;
		ss >> bname >> cnum >> lnum;
		clt.getBook(bname).chapters[cnum].remove(lnum);
	}
	else if(cmd == "add"){
		string bname, text;
		int cnum, lnum;
		ss >> bname >> cnum >> lnum >> ws;
		getline(ss, text);
		clt.getBook(bname).chapters[cnum].add(text, lnum);
	}
	else
		throw invalidCmdExc();
}

void ui::help(){
	cout << "bookSvr V 1\n"
		<< "____________________\n"
		<< "help\n"
		<< "logout\n"
		<< "list\n"
		<< "newBook numOfWriters [Writer2 ...] nameOfBook\n"
		<< "loadBook name\n"
		<< "listChapters bookName\n"
		<< "changeName oldBookName newBookName\n"
		<< "addChapter bookName chapterName chapterNum(start from 0)\n"
		<< "show bookName chapterNum(start from 0)\n"
		<< "showPrev bookName chapterNum(start from 0) ver(start from 1)\n"
		<< "newVer bookName chapterNum(start from 0)\n"
		<< "revVer bookName chapterNum(start from 0)\n"
		<< "add bookName chapterNum(start from 0) lineNum(start from 0) text\n"
		<< "addFile bookName chapterNum(start from 0) lineNum(start from 0) fileAddress\n"
		<< "remove bookName chapterNum(start from 0) lineNum(start from 0)\n"
		<< "sync bookname chapterNum(start from 0)\n"
		<< "publish bookName (ADMIN ONLY)\n"
		<< "unblock bookName (ADMIN ONLY)\n"
		<< "revVerSvr bookName chapterNum(start from 0) (ADMIN ONLY)\n"
		<< endl;
}

void ui::cmdErr(string msg){
	cout << "Err. " + msg + " Use \"help\".\n";
}

string ui::rebuildCmd(stringstream &ss, string cmd){
	string str;
	getline(ss, str);
	str = cmd + " " + str;
	return str;
}

void ui::procResponse(string str){
	if(str == "ok")
		cout << "Done!\n";
	else
		cmdErr(str);
}
