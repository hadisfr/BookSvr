#include "book.h"
#include "bookChapter.h"
#include "exc.h"
#include <fstream>
#include <string>

using namespace std;

void book::exportHTML(){
	ofstream f(string("published_books/" + name + ".htm").c_str());
	if(!f)
		throw fileOpenExc();
	f << "<!DOCTYPE html>\n<html><head><title>bookSvr :: " << name << "</title>\n"
	  << "<style>@charset \"utf-8\";\n"
	  << "body{background: #113; color: #111;}"
	  << "h1{color: #333; margin: 25px 25px 25px 25px;}"
	  << "h2{color: #000; margin: 25px 10px 0 10px;}"
	  << "div{width: 640px; margin: auto; background: #ddd; padding: 50px; border-bottom-left-radius: 30px; border-top-right-radius: 40px;}"
	  << "</style>"
	  << "</head>\n<body><br><div>\n";
	f << "<h1>" << name << "</h1>\n";
	for(int i = 0; i < chapters.size(); i++){
		f << "<h2>" << chapters[i].getName() << "</h2>\n<p>";
		f << chapters[i].show(-1, "</p>\n<p>") << "</p>\n";
	}
	f << "</div><p style=\"text-align: center; font-size: 70%; color: #999;\">C bookSvr v 1</div><br><br></body>\n</html>\n";
	f.close();
}

void bookChapter::addFile(string address, int n){
	ifstream f(string(address + ".txt").c_str());
	if(!f)
		throw fileOpenExc();
	while(!f.eof()){
		string str;
		getline(f, str);
		add(str, n++);
	}
	f.close();
}
