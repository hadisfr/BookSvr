#include "exc.h"
#include "network_exception.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "tools.h"
#include "server.h"
#include "server_socket.h"
#include "book.h"
#include "bookServer.h"

using namespace std;

class server: public Server{
private:
	vector <string> usernames;
	bookServer bookSvr;
	string admin_username;
public:
	server(int port, string user, string pass);
	void on_standard_input(string);
	void on_new_connection(int id);
	void on_new_message(int id, string);
	void on_terminated_connection(int id);
	class ui{
	public:
		static void help();
		static void cmdErr(string = "");
		static void statementProc(string msg, string usr, bookServer &);
	};
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

void server::on_new_message(int id, string msg){
	cout << setw(10) << left;
	usernames[id] != "" ? cout << usernames[id] + " on " + Tools::itos(id) : cout << id;
	cout << " :\t" << msg << endl;
	stringstream ss(msg);
	string cmd;
	ss >> cmd;
	if(cmd == "login" || cmd == "signup"){
		string u, p;
		ss >> u >> p;
		try{
			if(cmd == "signup")
				bookSvr.addUser(u, p);
			bookSvr.login(u, p);
			usernames[id] = u;
		} catch(...){
			send(id, "failed");
			return;
		}
		send(id, "validated");
	}
	else
	try{
		if(usernames[id] == ""){
			send(id, "Invalid user.");
			return;
		}
		if(cmd == "logout"){
			usernames[id] = "";
			send(id, "ok");
		}
		else if(cmd == "newBook"){
			int n;
			ss >> n;
			if(n < 1){
				send(id, "empty vector");
				return;
			}
			vector <string> w(1, usernames[id]);
			while(--n){
				string t;
				ss >> t;
				w.push_back(t);
			}
			string t;
			ss >> t;
			bookSvr.addBook(book(t, w));
			send(id, "ok");
		}
		else if(cmd == "changeName"){
			string o, n;
			ss >> o >> n;
			bookSvr.getBook(o, usernames[id]).setName(n);
			send(id, "ok");
		}
		else if(cmd == "addChapter"){
			string bname, cname;
			int t;
			ss >> bname >> cname >> t;
			bookSvr.getBook(bname, usernames[id]).addChapter(bookChapter(cname), t - 1);
			send(id, "ok");
		}
		else if(cmd == "unblock"){
			if(usernames[id] != admin_username){
				send(id, "ADMIN ONLY");
				return;
			}
			string bname;
			ss >> bname;
			bookSvr.unblockBook(bname);
			send(id, "ok");
		}
		else if(cmd == "publish"){
			if(usernames[id] != admin_username){
				send(id, "ADMIN ONLY");
				return;
			}
			string bname;
			ss >> bname;
			bookSvr.getBook(bname, usernames[id]).exportHTML();
			bookSvr.getBook(bname, usernames[id]).block();
			send(id, "ok");
		}
		else if(cmd == "revVerSvr"){
			if(usernames[id] != admin_username){
				send(id, "ADMIN ONLY");
				return;
			}
			string bname;
			int cnum;
			ss >> bname >> cnum;
			bookSvr.getBook(bname, usernames[id]).chapters[cnum].reverseVer();
			send(id, "ok");
		}
		else if(cmd == "getLastVerNum"){
			string bname;
			int cnum;
			ss >> bname >> cnum;
			try{
				send(id, Tools::itos(bookSvr.getBook(bname, usernames[id]).chapters[cnum].lastVerNum()));
			}catch(...){
				send(id, "-1");
			}
		}
		else if(cmd == "addVer"){
			int n, cnum;
			string bname;
			ss >> bname >> cnum >> n;
			//ss.ignore();
			while(n--){
				string str;
				getline(ss, str);
				try{
					ui::statementProc(str, usernames[id], bookSvr);
				} catch(...){
					cerr << "ERR\n";
					//send(id, "Unknown error.");
					//throw;
					return;
				}
			}
			bookSvr.getBook(bname, usernames[id]).chapters[cnum].newVer();
			send(id, "ok");
		}
		else if(cmd == "loadBook"){//***
			string bname, str = "";
			ss >> bname;
			/*try{
				bookSvr.getBook(bname, usernames[id]);
				send(id, "ok");
			} catch(...){
				send(id, "Err");
				return;
			}*/
			str += "newBook " + bookSvr.getBook(bname, usernames[id]).detailToStr() + "\n";
			//send(id, "newBook " + bookSvr.getBook(bname, usernames[id]).detailToStr());
			int n = bookSvr.getBook(bname, usernames[id]).chapters.size();
			//send(id, its(n));
			str += its(n) + "\n";
			for(int i = 0; i < n; i++)
				str += "addChapter " + bname + " " + bookSvr.getBook(bname, usernames[id]).chapters[i].getName() + " " + its(i) + "\n";
			for(int j = 0; j < n; j++){
				int lv = bookSvr.getBook(bname, usernames[id]).chapters[j].lastVerNum();
				for(int i = 0; i < lv; i++)
					str += "addVer " + bookSvr.getBook(bname, usernames[id]).chapters[j].getVerStr(i, bname + " " + its(j)) + "\n";
			}
			send(id, str);
		}
		/*else if(cmd == "show"){
			string bname;
			int cnum;
			ss >> bname >> cnum;
			send(id, bname + " :: " + bookClt.getBook(bname).chapters[cnum].getName() 
				+ " :: v " + Tools::itos(bookClt.getBook(bname).chapters[cnum].lastVerNum()) + "\n==========\n" 
				+ bookClt.getBook(bname).chapters[cnum].show());
		}
		else if(cmd == "showPrev"){
			string bname;
			int cnum, t;
			ss >> bname >> cnum >> t;
			send(id, bname + " :: " + bookClt.getBook(bname).chapters[cnum].getName() 
				+ " :: v " + Tools::itos(bookClt.getBook(bname).chapters[cnum].lastVerNum()) + "\n==========\n" 
				+ bookClt.getBook(bname).chapters[cnum].show(t));
		}*/
		else if(cmd == "list"){
			send(id, bookSvr.listOfBooks(usernames[id]));
		}
	} catch(...){
		ui::cmdErr();
	}
}

int main(int argc, char *argv[]){
	if (argc != 4){
		cerr << "valid operation: " << argv[0] << " port_number admin_username admin_password\n";
		exit(-1);
	}
	cout << "BookSvr V 1 :: SVR\n"
		<< "Use \"help\" to see list of commands.\n\n";

	server sserver(Tools::mystoi(argv[1]), string(argv[2]), string(argv[3]));
	sserver.run();

	return 0;
}

server::server(int port, string user, string pass) : Server(port), bookSvr(user, pass), admin_username(user){
	cout << "Server started on port " << port << endl;
	cout << "Admin: " << user << endl << endl;
}

void server::on_standard_input(string line){
	if(line == "exit")
		stop();
	else if(line == "help")
		ui::help();
	else
		cout << "invalid command. \"help\" for list of commands.\n";
}

void server::on_new_connection(int id){
	cout << "New connection with ID " << id << ".\n";
	while(id >= usernames.size())
		usernames.push_back("");
}

void server::on_terminated_connection(int id){
	cout << "Connection with ID " << id << " terminated.\n";
	usernames[id] = "";
}

void server::ui::help(){
	cout << "bookSvr V 1\n"
		<< "____________________\n"
		<< "help\n"
		<< "exit\n"
		<< endl;
}

void server::ui::cmdErr(string msg){
	cout << "Err\n";
	if(msg != "")
		cout << msg << endl;
}

void server::ui::statementProc(string str, string usr, bookServer &svr){
	stringstream ss(str);
	string cmd;
	ss >> cmd;
	if(cmd == "")
		return;
	if(cmd == "remove"){
		string bname;
		int cnum, lnum;
		ss >> bname >> cnum >> lnum;
		svr.getBook(bname, usr).chapters[cnum].remove(lnum);
	}
	else if(cmd == "add"){
		string bname, text;
		int cnum, lnum;
		ss >> bname >> cnum >> lnum >> ws;
		getline(ss, text);
		svr.getBook(bname, usr).chapters[cnum].add(text, lnum);
	}
	else
		throw invalidCmdExc();
}
