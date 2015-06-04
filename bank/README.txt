to use sqdb (C++ wrapper for sqlite3), need the following fomat:
g++ -l sqlite3 sqdb.cpp your_code.cpp

to use GraphStore.h (extended initializer), also need the flag:
-std=c++11