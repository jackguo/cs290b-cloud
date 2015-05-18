/*
multi-fliped
single thread
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>


#define MAXSIZE (541)
#define BIGCOUNT (9999999)




void PrintGraph(int *g, int gsize);
void CopyGraph(int *old_g, int o_gsize, int *new_g, int n_gsize);
int getKey(int i, int j);
int getI(int k);
int getJ(int k);
void clearQ( std::queue<int> &q);
int CliqueCount( int *g, int gsize, bool flip_new_edge_only );
int ReadGraph( char *fname, int **g, int *gsize );
std::string num2str( int num );
int getRandEdge( int gsize );
int sort2( int &n1, int &n2 );
int sort3( int &n1, int &n2, int &n3 );
void put_to_tabu_list(std::queue<int> &ban_q, std::set<int> &ban_s, int tabu_size, int key);

#include "util.h"




/*
flip multiple edges at the same time
*/
void flip_2_edge( int *g, int gsize, std::set<int> &ban_s, int &best_cnt, int *result, bool flip_new_edge_only ){
    
    int i[2]; //1 end of edge
    int j[2]; //1 end of edge
    int e[2]; //edge
    std::set<std::string> checked;
    std::string s;
    int cnt;
    best_cnt = BIGCOUNT;

    int k = 0;

    int goal = (flip_new_edge_only)? gsize/3 : gsize;
    while( k < goal ){
        if(flip_new_edge_only){
            e[0] = getRandNewEdge(gsize);
            while(ban_s.count(e[0]) != 0 ) e[0] = getRandNewEdge(gsize);
            e[1] = getRandNewEdge(gsize);
            while(e[1] == e[0] || ban_s.count(e[1]) != 0) e[1] = getRandNewEdge(gsize);

        }
        else{
            e[0] = getRandEdge(gsize);
            while(ban_s.count(e[0]) != 0 ) e[0] = getRandEdge(gsize);
            e[1] = getRandEdge(gsize);
            while(e[1] == e[0] || ban_s.count(e[1]) != 0) e[1] = getRandEdge(gsize);
        }

        sort2(e[0], e[1]);
        s = num2str(e[0]) + "-" + num2str(e[1]);

        if( checked.count(s) == 0 ){
            /*
            flip 2 edge
            */
            i[0] = getI(e[0]);
            j[0] = getJ(e[0]);
            i[1] = getI(e[1]);
            j[1] = getJ(e[1]);

            g[ i[0]*gsize + j[0] ] = 1 - g[ i[0]*gsize + j[0] ];
            g[ i[1]*gsize + j[1] ] = 1 - g[ i[1]*gsize + j[1] ];

            cnt = CliqueCount( g, gsize, flip_new_edge_only );
            if(cnt < best_cnt){
                best_cnt = cnt;
                result[0] = i[0];
                result[1] = j[0];
                result[2] = i[1];
                result[3] = j[1];
            }


            /*
            unflip
            */
            g[ i[0]*gsize + j[0] ] = 1 - g[ i[0]*gsize + j[0] ];
            g[ i[1]*gsize + j[1] ] = 1 - g[ i[1]*gsize + j[1] ];

            checked.insert(s);
            k++;
        }

    }


    checked.clear();
}





/*
flip 1 edge
*/
void flip_1_edge(int *g, int gsize, int i, int j, std::set<int> &ban_s, std::vector<int> &best_K, int &best_start, int &best_count, bool flip_new_edge_only){
    int key = getKey(i, j);
    int count;
    size_t sz;
    if(ban_s.count(key) == 0){
        /*
        flip
        */
        g[ i*gsize + j ] = 1 - g[ i*gsize + j ];
        count = CliqueCount(g, gsize, flip_new_edge_only );

        if(count <= best_count){

            if(count == best_count){
                best_K.push_back(key);
            }

            else{
                sz = best_K.size();
                if(sz == 0){
                    best_K.push_back(key);
                }
                else{
                    best_K[sz - 1] = key;
                    best_start = sz - 1;
                }

                best_count = count;
            }

        }
        /*
        unflip
        */ 
        g[ i*gsize + j ] = 1 - g[ i*gsize + j ];
    }   
}




/*
search
flip new edge first
if stuck
flip all
*/
void tabu_search(){
    int *g;
    int *new_g;
    int gsize;
    int count;
    int i;
    int j;
    int best_count;
    int best_i;
    int best_j;

    /*
    some vars for storing result of flip_2_edge
    */
    int best_count_2;
    int node[4];    


    /*
    start with a graph of size 8
    */
    if( !ReadGraph("99.ce", &g, &gsize) ){
        fprintf(stderr, "cannot read\n" );
        fflush(stderr);
        exit(1);
    }




    bool flip_new_edge_only = true;
    int tabu_size;
    int stuck_num;
    int stuck_cnt;
    int stuck_threshold = 10;


    /*
    tabu list
    */
    std::set<int> ban_s;
    std::queue<int> ban_q;


    /*
    best_counts collector
    */
    std::vector<int> best_K;
    int best_start = 0;


    /*
    search
    */
    int ra1;
    int ra2;
    while(gsize < MAXSIZE){

        count = CliqueCount( g, gsize, flip_new_edge_only );

        best_K.clear();
        best_start = 0;


        if( count == 0 ){
            printf("...Euraka! Counter example FOUND!\n");
            PrintGraph(g, gsize);

            new_g = (int *)malloc((gsize+1)*(gsize+1)*sizeof(int));
            if(new_g == NULL) exit(1);

            CopyGraph( g, gsize, new_g, gsize + 1 );
 
            for(i=0; i<gsize+1; i++){
                ra1 = rand() % 2;
                if(ra1 == 0){
                    new_g[i*(gsize+1) + gsize] = 0;
                    new_g[gsize*(gsize+1) + i] = 0;
                }
                else{
                    new_g[i*(gsize+1) + gsize] = 1;
                    new_g[gsize*(gsize+1) + i] = 1;
                }
            }

            free(g);
            g = new_g;
            gsize++;

            ban_s.clear();
            clearQ(ban_q);


            flip_new_edge_only = true;
            stuck_num = 0;
            stuck_cnt = 0;

            continue;
        }


        best_count = BIGCOUNT;
        int key;
        size_t sz;

        best_count_2 = BIGCOUNT;

        if( flip_new_edge_only ){
            j = gsize - 1;
            for(i=0; i<gsize-1; i++){
                ra1 = rand() % 2;
                if(ra1 == 0){

                    flip_1_edge(g, gsize, i, j, ban_s, best_K, best_start, best_count, true);
                }
            }


            flip_2_edge(g, gsize, ban_s, best_count_2, node, true);
        }

        else{

            /*
            flip 1 edge
            */
            for(i=0; i<gsize; i++){
                for(j=i+1; j<gsize; j++){
                    ra1 = rand() % 30;
                    if(ra1 == 0){
                        flip_1_edge(g, gsize, i, j, ban_s, best_K, best_start, best_count, false);
                    }
                }

            }

            /*
            flip 2 edge
            */
            flip_2_edge(g, gsize, ban_s, best_count_2, node, false);

        }


        if(best_count == BIGCOUNT){
            printf("no best found, terminating..\n");
            exit(1);
        }


        tabu_size = (flip_new_edge_only)? gsize/4 : gsize + gsize;
        if(best_count <= best_count_2){
            /*
            flip 1 edge
            */

            sz = best_K.size();
            ra1 = (best_start == sz - 1)? best_start : best_start + rand() % (sz - best_start);
            key = best_K[ra1];
            best_i = getI(key);
            best_j = getJ(key);
            g[ best_i*gsize + best_j ] = 1 - g[ best_i*gsize + best_j ];


            put_to_tabu_list(ban_q, ban_s, tabu_size, key);

            /*
            stuck?
            */
            if( flip_new_edge_only ){
                i = stuck_num - best_count;
                if( i < 0 ) i = -i;
                
                if( i < 3 ){
                    stuck_cnt++;
                    
                    if(stuck_cnt == stuck_threshold){
                        printf("stucked..\n.\n");
                        flip_new_edge_only = false;
                        stuck_cnt = 0;
                        stuck_num = 0;
                    }
                }
                else{
                    stuck_num = best_count;
                    stuck_cnt = 0;
                }
            }

            printf("ce size: %d, best_count: %d, best edge: (%d, %d), new color: %d\n", gsize, best_count, best_i, best_j, g[best_i*gsize + best_j]);

        }
        else{
            /*
            flip 2 edge
            */

            g[ node[0]*gsize + node[1] ] = 1 - g[ node[0]*gsize + node[1] ];
            g[ node[2]*gsize + node[3] ] = 1 - g[ node[2]*gsize + node[3] ];

            put_to_tabu_list(ban_q, ban_s, tabu_size, getKey(node[0], node[1]));
            put_to_tabu_list(ban_q, ban_s, tabu_size, getKey(node[0], node[1]));

            printf("ce size: %d, best_count: %d, best edge: (%d, %d), (%d, %d)\n", gsize, best_count_2, node[0], node[1], node[2], node[3]);

        }


        /*
        rinse and repeat
        */
    }


}







/*
main
*/
int main(void){
    
    srand(time(0));

    tabu_search();

    return 0;
}












































