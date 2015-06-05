to use sqdb (C++ wrapper for sqlite3), need the following fomat:
g++ -l sqlite3 sqdb.cpp bank.cpp -o bank

to use GraphStore.h (extended initializer), also need the flag:
-std=c++11