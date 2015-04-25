#include <iostream>
#include <thread>
#include <cstdio>
/*thread demo*/

using namespace std;


void foo(){
	for(int i=0;i<100;i+=2){
		printf("%d\n", i);
	}
}

void bar(){
	for(int i=1;i<100;i+=2){
		printf("%d\n", i);
	}
}

int main(){

	thread even (foo);
	thread odd (bar);

	cout << "foo and bar exec concurrently" << endl;
	
	even.join();
	odd.join();

	cout << "foo and bar finished" << endl;

	return 0;
}
