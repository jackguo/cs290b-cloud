/*
All DB necessary functions
*/

#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>


#include <iostream>
#include <string>
#include <sstream>

#include "sqdb.h"
extern "C" {
  #include "isomorph.h"
}

class GraphStore {
private:
  sqdb::Db db;
  int max_graph = 0;
  bool to_matrix(const std::string &graph, int * matrix, int size);

public:
	GraphStore();
	void put( int, char* );
	bool get( int, char* );
	bool has_size( int );

	std::string num2str( int );	
};


GraphStore::GraphStore() : db("db.sdb") {
	srand( time(0) );
	if( !db.TableExists("graphs") ){
		/*
		init the graphs table
		+------+-------------+
		| size | # of graphs |
		+------+-------------+
		*/
		db.Query("create table graphs(size int primary key, num int);").Next();
		std::cout << "table graphs create done" << std::endl;
	}
	else{

		sqdb::Statement s = db.Query("select max(size) from graphs");
		s.Next();
		max_graph = s.GetField(0);
	
	}

}

std::string GraphStore::num2str( int num ){
	std::stringstream ss;
	ss << num;
	return ss.str();
}


bool GraphStore::has_size( int gsize ){
	return gsize <= max_graph && gsize > 0;
}

bool GraphStore::to_matrix(const std::string & graph, int * matrix, int size) {

  if (size != graph.length()) { // size doesn't match graph size!
    return false; 
  }
  
  for (int i = 0; i < size; i++) {
    if (graph[i] == '0') {
      matrix[i] = 0;
    } else if (graph[i] == '1') {
      matrix[i] = 1;
    } else {
      return false;
    }
  }

  return true;
}

/*
put a graph into DB
*/
void GraphStore::put( int gsize, char *g ){
  std::string sql0;
	if( has_size(gsize) ){
                int *g1 = new int[gsize];
                int *g2 = new int[gsize];

		std::string tb_name = "graph" + num2str(gsize);
		std::string graph_bit(g);
                
		// isomorphic check
                to_matrix(graph_bit, g1, gsize);
                
                sql0 = "select * from " + tb_name;
                sqdb::Statement s = db.Query(sql0.c_str());
                int iso;
                
                while (s.Next()) {
                  std::string graph = s.GetField(1);
                  to_matrix(graph, g2, gsize);
                  iso = IsIsomorph(g1, g2, gsize);
                  if (iso == 1) {
                    return;
                  }
                }
                // check done

		sql0 = "insert into " + tb_name + "(graph) values ( '" + graph_bit + "') ;";
		db.Query( sql0.c_str() ).Next();

	}
	else{

		/*
		insert a row in table Graphs
		*/
		sql0 = "insert into graphs (size, num) values (" + num2str(gsize) + ", 1);";
		db.Query( sql0.c_str() ).Next();


		/*
		create a new table to store the graphs of gsize
		+----+-------+
		| id | graph |
		+----+-------+
		*/
		std::string tb_name = "graph" + num2str(gsize);
		sql0 = "create table " + tb_name + "(id int primary key autoincrement, graph text);";
		db.Query( sql0.c_str() ).Next();


		/*
		insert the graph into the table
		*/
		std::string graph_bit(g);
		sql0 = "insert into " + tb_name + "(graph) values ( '" + graph_bit + "' );";
		db.Query( sql0.c_str() ).Next();		


	}

}




/*
get a graph of gsize from DB
randomly
*/
bool GraphStore::get( int gsize, char *g ){
	if( !has_size(gsize) ) return false;

	std::string sql0 = "select * from graphs where size = " + num2str(gsize) + ";";
	sqdb::Statement s = db.Query(sql0.c_str());
	s.Next();
	int num = s.GetField(1);

	std::string tb_name = "graph" + num2str(gsize);
	if(num == 1){

		sql0 = "select * from " + tb_name + ";";
		s = db.Query(sql0.c_str());
		s.Next();
		std::string gg = s.GetField(1);
		strcpy(g, gg.c_str());

	}
	else{

		/*
		randomly select 1
		*/
		
		int gid = 1 + rand() % num;
		sql0 = "select * from " + tb_name + " where id = " + num2str(gid) + ";";
		s = db.Query(sql0.c_str());
		s.Next();
		std::string gg = s.GetField(1);
		strcpy(g, gg.c_str());

	}	

	return true;
}






