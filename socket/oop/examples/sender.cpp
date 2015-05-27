#include "../Socket.hpp"
#include <string.h>

using namespace std;

#define IP "127.0.0.1"
#define PORT 10000


int main(){

	char chunk[] = "hello, w000000t";		

	try{
		Socket::TCP server;
		server.connect_to(Socket::Address(IP, PORT));
		cout << "sending ..." << endl;
		server.send<char>(chunk, strlen(chunk));
		cout << "done .. " << endl;
	
	}
	catch(Socket::SocketException &e){
		cout << e << endl;
	}

}
