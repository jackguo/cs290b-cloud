/*
multi-fliped
single thread
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <ctime>
#include <queue>
#include <set>
#include <vector>


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

#include "util.h"



/*
Search.
Hybrid.
start from flipping new edge only.
if stuck.
then flip all.
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
    start with a graph of size 99
    */
    if( !ReadGraph("99.ce", &g, &gsize) ){
        fprintf(stderr, "cannot read\n", );
        fflush(stderr);
        exit(1);
    }


    int tabu_size;
    bool flip_new_edge_only = true;


    /*
    check if stuck at some best_count?
    */
    int stuck_num;
    int stuck_cnt;
    int stuck_threshold = 20;
    int stuck_threshold2 = 20;


    /*
    multiple flips?
    */
    int m;
    int n;
    int best_count2;
    int best_i2;
    int best_j2;
    int key2;
    int best_m;
    int best_n;
    int run_level = 1;

    /*
    init tabu list which is made up by 1 set and 1 queue
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


        /*
        find how we are doing
        */
        count = CliqueCount( g, gsize, flip_new_edge_only );
        

        /*
        reset collector
        */
        best_K.clear();
        best_start = 0;


        /*
        if we get a counter example
        */
        if(count == 0){


            printf("....Euraka! Counter found\n");


            PrintGraph(g, gsize);
            
            /*
            make a new graph one size bigger
            */
            new_g = (int *)malloc((gsize+1)*(gsize+1)*sizeof(int));
            if(new_g == NULL) exit(1);
            
            /*
            copy the old graph into the new graph leaving the last row
            and last column alone
            */
            CopyGraph(g, gsize, new_g, gsize+1);
            
            /*
            radom init the last column and last row
            */
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
            

            /*
            throw away the old graph and make new one
            */
            free(g);
            g = new_g;
            gsize++;
            

            /*
            reset the taboo list for the new graph
            */
            ban_s.clear();
            clearQ(ban_q);

            

            /*
            reset flip bool and stucks
            */
            flip_new_edge_only = true;
            run_level = 1;
            stuck_num = 0;
            stuck_cnt = 0;

            /*
            keep going
            */
            continue;
        
        }
        
        /*
        otherwise, we need to consider flipping an edge
        let's speculative flip each edge, record the new
        count, and unflip the edge. We'll then remember
        the best flip and keep it next time around.
        only need to work with upper triangle of matrix =>
        notice the indices
        */
        
        best_count = BIGCOUNT;
        best_count2 = BIGCOUNT;
        int key;
        size_t sz;


        if( flip_new_edge_only ){
            j = gsize - 1;
            for(i=0; i<gsize-1; i++){
                ra1 = rand() % 2;

                if(ra1 == 0){
                    key = getKey(i, j);
                    if(ban_s.count(key) != 0) continue;

                    g[ i*gsize + j ] = 1 - g[ i*gsize + j ];
                    count = CliqueCount(g, gsize, true);

                    if(count <= best_count ){
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
                        }

                        best_count = count;
                    }

                    g[ i*gsize + j ] = 1 - g[ i*gsize + j ];
                }

            }

        }

        else{

            /*
            flip 1 edge
            */
            for(i=0; i<gsize; i++){
                for(j=i+1; j<gsize; j++){
                    
                    ra1 = rand() % gsize;
                    

                    if( ra1 == 0 ){

                        key = getKey(i, j);
                        if(ban_s.count(key) != 0) continue;

                        g[i*gsize + j] = 1 - g[i*gsize + j];
                        count = CliqueCount(g, gsize, false);
                        
                        
                        if( count <= best_count){

                            if( count == best_count ){
                                best_K.push_back(key);
                            }

                            else{
                                sz = best_K.size();
                                if(sz == 0){
                                    best_K.push_back(key);
                                }
                                else{
                                    best_K[sz-1] = key;
                                    best_start = sz - 1;
                                }
                            }

                            best_count = count;
                        }

                        g[i*gsize + j] = 1 - g[i*gsize + j];      
                    
                    }
              
                }
            
            }

            /*
            flip 2 edges at the same time
            */
            if(run_level > 1){

                for(i=0; i<gsize; i++){
                    for(j=i+1; j<gsize; j++){
                        for(m=i; m<gsize; m++){
                            for(n=m+1; n<gsize; n++){
                                if(i == m && j == n) continue;
                                ra1 = rand() % (gsize + gsize + gsize);
                                if(ra1 == 0){
                                    key = getKey(i, j);
                                    key2 = getKey(m, n);
                                    if(ban_s.count(key) != 0 || ban_s.count(key2) != 0) continue;

                                    g[i*gsize + j] = 1 - g[i*gsize + j];
                                    g[m*gsize + n] = 1 - g[m*gsize + n];
                                    count = CliqueCount(g, gsize, false);

                                    /*
                                    ra2 = rand() % 2;
                                    bool flag = (ra2 == 0)? (count < best_count2) : (count <= best_count2);
                                    */

                                    if( count < best_count2 ){
                                        best_count2 = count;
                                        best_i2 = i;
                                        best_j2 = j;
                                        best_m = m;
                                        best_n = n;
                                    }


                                    g[i*gsize + j] = 1 - g[i*gsize + j];
                                    g[m*gsize + n] = 1 - g[m*gsize + n];
                                }
                            }
                        }
                    }
                }
            }
        
        }



        if(best_count == BIGCOUNT){
            printf("no best edge found, terminating\n");
            exit(1);
        }
        

        tabu_size = (flip_new_edge_only)? gsize/6 : gsize;

        if(run_level > 1 && best_count2 < best_count){
            /*
            flip 2 edges
            */
            g[ best_i2*gsize + best_j2 ] = 1 - g[ best_i2*gsize + best_j2 ];
            g[ best_m*gsize + best_n ] = 1 - g[ best_m*gsize + best_n ];

            sz = ban_q.size();
            if(sz == tabu_size){
                ban_s.erase(ban_q.front());
                ban_q.pop();
                ban_s.erase(ban_q.front());
                ban_q.pop();                
            }
            else if(sz == tabu_size - 1){
                ban_s.erase(ban_q.front());
                ban_q.pop();
            }

            ban_q.push(getKey(best_i2, best_j2));
            ban_s.insert(getKey(best_i2, best_j2));
            ban_q.push(getKey(best_m, best_n));
            ban_s.insert(getKey(best_m, best_n)); 

        }
        else{
            /*
            flip 1 edge
            */
            sz = best_K.size();
            ra1 = rand() % (sz - best_start);
            ra1 += best_start;
            key = best_K[ra1];
            best_i = getI(key);
            best_j = getJ(key);
            g[ best_i*gsize + best_j ] = 1 - g[ best_i*gsize + best_j ];
            


            if(ban_q.size() == tabu_size){
                ban_s.erase(ban_q.front());
                ban_q.pop();
            }
            ban_q.push(key);
            ban_s.insert(key);
        
        }
  

        if(flip_new_edge_only){
            i = stuck_num - best_count;
            if( i < 0) i = -i;

            if( i < 3 ){
                stuck_cnt++;
                if( stuck_cnt == stuck_threshold ){
                    printf("stuckked..\n.\n.\n. go to lvl . 1\n");
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
        else{
            if(run_level == 1){

                i = stuck_num - best_count;
                if( i < 0 ) i = -i;
                if(i < 3){
                    stuck_cnt++;
                    if( stuck_cnt == stuck_threshold2 ){
                        printf("stuckkked..\n.\n.\n go to lvl . 2\n");
                        run_level = 2;
                        stuck_cnt = 0;
                        stuck_num = 0;
                    }
                }
                else{
                    stuck_num = best_count;
                    stuck_cnt = 0;
                }

            }
        }
        

        if(best_count2 != BIGCOUNT && run_level > 1 && best_count2 > best_count){

            printf("ce size: %d, best_count: %d, best edge 1: (%d, %d), best edge 2: (%d, %d), new color 1: %d, new color 2: %d\n", gsize, best_count2, best_i2, best_j2, best_m, best_n, g[best_i2*gsize + best_j2], g[best_m*gsize + best_n]);            

        }        
        else{
            printf("ce size: %d, best_count: %d, best edge: (%d, %d), new color: %d\n", gsize, best_count, best_i, best_j, g[best_i*gsize + best_j]);
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












































