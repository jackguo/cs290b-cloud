// graph storage, using sqlite3
#include <sqlite3.h>

class GraphStore {
public:
  /* constructor, open a sqlite3 database */
  GraphStore();  
  /* deconstructor, close the database */
  ~GraphStore();
  
  /* only non-isomorphic graphs are stored */
  int put(int size, const char* graph);
  /* return id if an isomorhic graph exists, -1 otherwise */
  int get_id(int size, const char* graph);
  bool rm_graph(int size, int id);
  /* check if a graph of a certain size exists */
  bool has_size(int size);
  
private:
  sqlite3 *db;
};
