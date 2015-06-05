#include <string.h>
#include <stdio.h>


#include <iostream>

#include "../socket/Socket.hpp"




#define BANK_IP1 "128.111.43.37" //bart
#define BANK_IP2 "128.111.43.70" //boris


#define SEND_PORT 12345
#define RECEIVE_PORT 13333
#define MAXCHUNKSIZE 44100


Socket::TCP server;


void put_into_bank( char *g ){
	try{
		server.connect_to(Socket::Address(BANK_IP1, SEND_PORT));
		std::cout << "sending.." << std::endl;
		server.send<char>(g, strlen(g));
		std::cout << "done.." << std::endl;
	}
	catch(Socket::SocketException &e){
		std::cout << e << std::endl;
		std::cout << "bank #1 failed" << std::endl;
		std::cout << "try connecting bank #2..." << std::endl;
		
		try{
			server.connect_to(Socket::Address(BANK_IP2, SEND_PORT));
			std::cout << "sending.." << std::endl;
			server.send<char>(g, strlen(g));
			std::cout << "done.." << std::endl;
		}
		catch(Socket::SocketException &e){
			std::cout << e << std::endl;
			std::cout << "bank #2 failed" << std::endl;
		} 

	}
}




/*
graph will be stored into <result>
*/
void get_from_bank( int gsize, char *result ){

	char chunk[10] = {0};
	sprintf(chunk, "%d", gsize);
	try{
		server.connect_to(Socket::Address(BANK_IP1, SEND_PORT));
		std::cout << "sending.." << std::endl;
		server.send<char>( chunk, strlen(chunk) );
		std::cout << "done.." << std::endl;

		std::cout << "waiting for response" << std::endl;
		/*
		get response
		*/
		server.listen_on_port(RECEIVE_PORT);
		Socket::TCP client = server.accept_client();
		std::cout << "receiving.." << endl;
		client.receive<char>(result, MAXCHUNKSIZE);
		std::cout << "done.." << std::endl;


	}
	catch(Socket::SocketException &e){
		std::cout << e << std::endl;
		std::cout << "bank #1 failed" << std::endl;
		std::cout << "try connecting bank #2..." << std::endl;
		
		try{
			server.connect_to(Socket::Address(BANK_IP2, SEND_PORT));
			std::cout << "sending.." << std::endl;
			server.send<char>( chunk, strlen(chunk) );
			std::cout << "done.." << std::endl;

			std::cout << "waiting for response" << std::endl;
			/*
			get response
			*/
			server.listen_on_port(RECEIVE_PORT);
			Socket::TCP client = server.accept_client();
			std::cout << "receiving.." << endl;
			client.receive<char>(result, MAXCHUNKSIZE);
			std::cout << "done.." << std::endl;


		}
		catch(Socket::SocketException &e){
			std::cout << e << std::endl;
			std::cout << "bank #2 failed" << std::endl;
		} 

	}


}

