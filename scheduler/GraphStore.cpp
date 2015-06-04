#include "GraphStore.h"
#include <iostream>
#include <stdlib.h>

GraphStore::GraphStore() {
  int rc;
  
  rc = sqlite3_open("graph.db", &db);
  
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    exit(0);
  } else {
    std::cout << "successfully opened database" << std::endl;
  }
}

GraphStore::~GraphStore() {
  sqlite3_close(db);
}

  
