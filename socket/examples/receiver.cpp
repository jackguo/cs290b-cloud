#include "../Socket.hpp"
#include <string.h>

using namespace std;

int main(){

	char chunk[100];

	try{
		
		Socket::TCP server;
		server.listen_on_port(10000);
		Socket::TCP client = server.accept_client();

		cout << "receiving ... " << endl;
		client.receive<char>(chunk, 100);
		cout << chunk << endl;
		cout << "done. " << endl;
	}
	catch(Socket::SocketException &e){
		
		cout << e << endl;

	}

}
