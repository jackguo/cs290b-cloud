/*
simple scheduler
*/
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <cmath>

#include "../socket/Socket.hpp"
#include "GraphStore.h"


#define PORT 12345
#define CHUNKSIZE 44100 //210 * 210



/*
a helper function
stoi
*/
int getNum( char *buf ){
	int pos = 0;
	while( !(buf[pos] >= '0' && buf[pos] <= '9') ) pos++;
	int num = 0;
	
	do{
		num = 10 * num + (buf[pos] - '0');
		pos++;
	}while(buf[pos] >= '0' && buf[pos] <= '9');


	return num;
}




int main(){

	char chunk[CHUNKSIZE];
	GraphStore gstore;
			
	try{

		Socket::TCP server;
		server.listen_on_port(PORT);

		while(1){
			/*
			handle message
			*/
			memset(chunk, 0, CHUNKSIZE);
			Socket::TCP client = server.accept_client();

			std::cout << "receiving.." << std::endl;

			client.receive<char>(chunk, CHUNKSIZE);

			if(strlen(chunk) < 5){
				/*
				request : GET
				*/
				int sz = getNum(chunk);
				server.connect_to( Socket::Address(client.ip(), client.port()) );

				std::cout << "sending.." << std::endl;
				if( gstore.get(sz, chunk) ){
					server.send<char>(chunk, strlen(chunk));
				}
				else{
					strcpy(chunk, "null");
					server.send<char>(chunk, strlen(chunk));
				}

				std::cout << "done." << std::endl;



			}
			else{
				/*
				request : PUT
				*/

				int sz = (int) std::sqrt(strlen(chunk));
				gstore.put(sz, chunk);



			}


			std::cout << "done.." << std::endl;
		}



	}
	catch(Socket::SocketException &e){
		std::cout << e << std::endl;
	}


	return 0;
}