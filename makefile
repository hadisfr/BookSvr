CC = g++
CFLAG = -g
OBJ = $(CC) $(CFLAG) -c

all: server.out client.out
	clear && mkdir published_books && echo "\nDone!\nUse \"./server.out\" or \"./cient.out\".\nAdd \"echo\" argument when feeding client from file.\n"

clean:
	echo "\n" && ls -a -l *.o && echo "\n" && rm *.o ~* .DS_Store
cleanAll:
	rm *.out && rm -R published_books && make clean

server.out: serverMain.o server.o server_socket.o tools.o book.o bookChapter.o version.o fileStream.o bookServer.o
	$(CC) $(CFLAG) serverMain.o server.o server_socket.o tools.o book.o bookChapter.o version.o fileStream.o bookServer.o -o server.out
client.out : clientMain.o client_socket.o tools.o book.o bookChapter.o version.o fileStream.o bookClient.o
	$(CC) $(CFLAG) clientMain.o client_socket.o tools.o book.o bookChapter.o version.o fileStream.o bookClient.o -o client.out

server.o: server.cpp server.h
	$(CC) -c server.cpp
server_socket.o: server_socket.cpp server_socket.h tools.h network_exception.h
	$(CC) -c server_socket.cpp
client_socket.o: client_socket.cpp client_socket.h tools.h network_exception.h
	$(CC) -c client_socket.cpp
tools.o: tools.cpp tools.h
	$(CC) -c tools.cpp

serverMain.o: serverMain.cpp exc.h network_exception.h server_socket.h server.h  book.h bookChapter.h version.h bookServer.h
	$(OBJ) serverMain.cpp
clientMain.o: clientMain.cpp exc.h network_exception.h client_socket.h book.h bookChapter.h version.h bookClient.h
	$(OBJ) clientMain.cpp

book.o: book.cpp book.h exc.h bookChapter.h
	$(OBJ) book.cpp
bookChapter.o: bookChapter.cpp bookChapter.h exc.h version.h
	$(OBJ) bookChapter.cpp
version.o: version.cpp version.h exc.h
	$(OBJ) version.cpp
bookServer.o: bookServer.cpp bookServer.h exc.h book.h
	$(OBJ) bookServer.cpp
bookClient.o: bookClient.cpp bookClient.h exc.h
	$(OBJ) bookClient.cpp
fileStream.o: fileStream.cpp book.h bookChapter.h exc.h
	$(OBJ) fileStream.cpp