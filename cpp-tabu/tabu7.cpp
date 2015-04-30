/*
CS 290B
tabu search: R(7,7)
Team: Eagle
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <queue>
#include <set>
#include <vector>
#include <thread>
#include <mutex>


#define MAXSIZE (541)
#define TABOOSIZE (500)
#define TABOOSIZE_PART (20)
#define BIGCOUNT (9999999)


/*
function declare
*/
void update( int , int* );
void PrintGraph( int*, int );
void CopyGraph( int*, int, int*, int );
int CliqueCount( int*, int );
void tabu_search_full();
int CliqueCountPart( int*, int );
void tabu_search_part();


/*
storing the largest graph we have found
*/
int found_g[80000] = {0};
int found_size = 8;
std::mutex mtx;

/*
found a larger one? update it.
*/
void putFound(int sz, int *g){
    
    mtx.lock();

    if(sz <= found_size){
        mtx.unlock();
        return;
    }

    found_size = sz;

    CopyGraph(g, sz, found_g, found_size);


    mtx.unlock();
}

/*
find self is lagging? then use the Found
*/
int* getFound(int sz, int *g){
    mtx.lock();

    if(sz >= found_size){
        mtx.unlock();
        return g;
    }

    sz = found_size;
    int *new_g = (int *)malloc(sz*sz*sizeof(int));
    if(new_g == NULL) exit(1);

    CopyGraph(found_g, found_size, new_g, sz);
    free(g);

    mtx.unlock();

    return new_g;
}





/*
Print Graph
*/
void PrintGraph(int *g, int gsize){
    int i, j;
    fprintf(stdout, "%d\n", gsize);
    
    for(i=0; i<gsize; i++){
        for(j=0; j<gsize; j++){
            fprintf(stdout, "%d ", g[i*gsize + j]);
        }
        
        fprintf(stdout, "\n");
    
    }
}


/*
Copy Graph
*/
void CopyGraph(int *old_g, int o_gsize, int *new_g, int n_gsize){
    int i,j;
    
    if(n_gsize < o_gsize) return;
    
    for(i=0; i<o_gsize; i++){
        for(j=0; j<o_gsize; j++){
            new_g[i*n_gsize + j] = old_g[i*o_gsize + j];
        }
    }
    
}




/*
return the number of monochromatic cliques in the graph
presented to it.
graph is stored in row-major order
only checks values above diagonal
*/

int CliqueCount(int *g, int gsize){
    int i,j,k,l,m,n,o,count,sgsize, colr;
    count = 0;
    sgsize = 7;
    
    for(i=0; i<gsize-sgsize+1; i++){
        for(j=i+1; j<gsize-sgsize+2; j++){
            colr = g[i*gsize + j];
            for(k=j+1; k<gsize-sgsize+3; k++){
            
                if(colr == g[i*gsize + k] && colr == g[j*gsize + k]){
                    
                    for(l=k+1; l<gsize-sgsize+4; l++){
                                        
                        if(colr == g[i*gsize + l] && colr == g[j*gsize + l] && colr == g[k*gsize + l]){
                            
                            for(m=l+1; m<gsize-sgsize+5; m++){
                                if(colr == g[i*gsize+ m] && colr == g[j*gsize+ m] && colr == g[k*gsize+ m] && colr == g[l*gsize+ m]){
                                    for(n=m+1; n<gsize-sgsize+6;n++){
                                        if(colr == g[i*gsize + n] && colr == g[j*gsize + n] && colr == g[k*gsize + n] && colr == g[l*gsize + n] && colr == g[m*gsize + n]){
                                            for(o=n+1; o<gsize-sgsize+7; o++){
                                                if(colr == g[i*gsize+ o] && colr == g[j*gsize+ o] && colr == g[k*gsize+ o] && colr == g[l*gsize+ o] && colr == g[m*gsize+ o] && colr == g[n*gsize+ o]){
                                                    count++;
                                                }
                                            }                                        
                                        }
                                    }
                                
                                }
                            }
                        }                   
                    }
                }            
            }
        }    
    }
    
    return count;
}



/*
helper functions
assuming i, j <= 540
*/
int getKey(int i, int j){
    return i*10000 + j;
}
int getI(int k){
    return k / 10000;
}
int getJ(int k){
    return k % 10000;
}
void clearQ( std::queue<int> &q){
    std::queue<int> empty;
    std::swap( q, empty );
}



/*
original version
*/
void tabu_search_full(){
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
    init tabu list which is made up by 1 set and 1 queue
    */
    std::set<int> ban_s;
    std::queue<int> ban_q;
    
    /*
    best_counts collector
    */
    std::vector<int> best_k;
    int best_start = 0;

    /*
    start with graph of size 8
    */
    gsize = 8;
    g = (int *)malloc(gsize*gsize*sizeof(int));
    if(g == NULL) exit(1);
    
    /*
    start out with all zeros
    */
    memset(g, 0, gsize*gsize*sizeof(int));
    
    /*
    search
    */
    int ra1;
    while(gsize < MAXSIZE){
        /*
        find how we are doing
        */
        count = CliqueCount(g, gsize);
        
        /*
        reset collector
        */
        best_k.clear();
        best_start = 0;

        /*
        if we get a counter example
        */
        if(count == 0){
            printf("11111....Euraka! Counter found\n");
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
            zero out the last column and last row
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
        int key;
        size_t sz;
        for(i=0; i<gsize; i++){
            for(j=i+1; j<gsize; j++){
                
                ra1 = rand() % 2;
                if(ra1 == 0){
                    /*
                    flip it
                    */
                    g[i*gsize + j] = 1 - g[i*gsize + j];
                    count = CliqueCount(g, gsize);
                    
                    /*
                    is it better and the i,j,count not tabu?
                    */
                    key = getKey(i, j);
                    
                    /*
                    if we have multiple best counts, which one do we use?
                    we use the first one or last one ?
                    */
                    if(count <= best_count && ban_s.count(key) == 0){

                        if(count == best_count){
                            best_k.push_back(key);
                        }
                        else{
                            sz = best_k.size();
                            if(sz == 0){
                                best_k.push_back(key);
                            }
                            else{
                                best_k[sz-1] = key;
                                best_start = sz - 1;
                            }
                        }

                        best_count = count;
                    }
                    
                    /*
                    flip it back
                    */
                    g[i*gsize + j] = 1 - g[i*gsize + j];      
                
                }
          
            }
        
        }
        
        if(best_count == BIGCOUNT){
            printf("no best edge found, terminating\n");
            exit(1);
        }
        
        /*
        keep the best flip we saw
        */
        sz = best_k.size();
        ra1 = rand() % (sz - best_start);
        ra1 += best_start;
        key = best_k[ra1];
        best_i = getI(key);
        best_j = getJ(key);
        g[best_i*gsize + best_j] = 1 - g[best_i*gsize + best_j];
        
        /*
        tabu this graph configuration so that we do not 
        visit it again
        */
        //key = getKey(best_i, best_j);
        if(ban_q.size() == TABOOSIZE){
            ban_s.erase(ban_q.front());
            ban_q.pop();
        }
        ban_q.push(key);
        ban_s.insert(key);
        
        printf("ce size: %d, best_count: %d, best edge: (%d, %d), new color: %d\n",
        gsize, best_count, best_i, best_j, g[best_i*gsize + best_j]);
        
        /*
        rinse and repeat
        */   
    
    }
        
}



/*
return the number of monochromatic cliques in the graph.
A little difference with CliqueCount is:
We only need 6 loops instead of 7.
We only check the sub-cliques that has the new node.
*/
int CliqueCountPart(int *g, int gsize){
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int o = gsize - 1;
    int count = 0;
    int sgsize = 7;
    int colr;
    
    for(i=0; i<gsize-sgsize+1; i++){
        for(j=i+1; j<gsize-sgsize+2; j++){
            colr = g[i*gsize + j];
            for(k=j+1; k<gsize-sgsize+3; k++){
            
                if(colr == g[i*gsize + k] && colr == g[j*gsize + k]){
                    
                    for(l=k+1; l<gsize-sgsize+4; l++){
                                        
                        if(colr == g[i*gsize + l] && colr == g[j*gsize + l] && colr == g[k*gsize + l]){
                            
                            for(m=l+1; m<gsize-sgsize+5; m++){
                                if(colr == g[i*gsize+ m] && colr == g[j*gsize+ m] && colr == g[k*gsize+ m] && colr == g[l*gsize+ m]){
                                    for(n=m+1; n<gsize-sgsize+6;n++){
                                        if(colr == g[i*gsize + n] && colr == g[j*gsize + n] && colr == g[k*gsize + n] && colr == g[l*gsize + n] && colr == g[m*gsize + n]){

                                            if(colr == g[i*gsize+ o] && colr == g[j*gsize+ o] && colr == g[k*gsize+ o] && colr == g[l*gsize+ o] && colr == g[m*gsize+ o] && colr == g[n*gsize+ o]){
                                                count++;
                                            }
                                      
                                        }
                                    }
                                
                                }
                            }
                        }                   
                    }
                }            
            }
        }    
    }    
    

    return count;    
}



/*
partial version:
assuming the counter example is embeded
in the graph one size bigger
*/
void tabu_search_part(){
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
    init tabu list which is made up by 1 set and 1 queue
    */
    std::set<int> ban_s;
    std::queue<int> ban_q;
    

    /*
    best_counts collector
    */
    std::vector<int> best_k;
    int best_start = 0;

    /*
    start with graph of size 8
    */
    gsize = 8;
    g = (int *)malloc(gsize*gsize*sizeof(int));
    if(g == NULL) exit(1);
    
    /*
    start out with a counter example
    */
    memset(g, 0, gsize*gsize*sizeof(int));
    g[0*gsize + 2] = 1;
    g[1*gsize + 4] = 1;
    
    /*
    search
    */
    int ra1;
    while(gsize < MAXSIZE){
        /*
        find how we are doing
        */
        count = CliqueCountPart(g, gsize);
        
        /*
        reset collecor
        */
        best_k.clear();
        best_start = 0;

        /*
        if we get a counter example
        */
        if(count == 0){
            printf("22222....Euraka! Counter found\n");
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
            zero out the last column and last row
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
            keep going
            */
            continue;
        
        }

        /*
        otherwise, random flip
        */
        best_count = BIGCOUNT;
        int key;
        size_t sz;

        /*
        unlike tabu_search_full, here we only
        flip the new edge, which is the last row
        amd last column
        */
        j = gsize - 1;
        for(i=0; i<gsize-1; i++){
            ra1 = rand() % 2;
            if(ra1 == 0){

                /*
                flip it
                */
                g[i*gsize + j] = 1 - g[i*gsize + j];
                count = CliqueCountPart(g, gsize);

                /*
                is it better and the i,j,count not tabu?
                */
                key = getKey(i, j);

                if(count <= best_count && ban_s.count(key) == 0){

                    if(count == best_count){
                        best_k.push_back(key);
                    }
                    else{
                        sz = best_k.size();
                        if(sz == 0){
                            best_k.push_back(key);
                        }
                        else{
                            best_k[sz-1] = key;
                            best_start = sz - 1;
                        }
                    }

                    best_count = count;

                }

                /*
                flip it back
                */
                g[i*gsize + j] = 1 - g[i*gsize + j];

            }
        }


        if(best_count == BIGCOUNT){
            printf("no best edge found, terminating\n");
            exit(1);
        }

        /*
        keep the best flip we saw
        */
        sz = best_k.size();
        ra1 = rand() % (sz - best_start);
        ra1 += best_start;
        key = best_k[ra1];
        best_i = getI(key);
        best_j = getJ(key);
        g[best_i*gsize + best_j] = 1 - g[best_i*gsize + best_j];


        /*
        tabu this graph configuration so that we do not visit
        it again
        */
        if(ban_q.size() == TABOOSIZE_PART){
            ban_s.erase(ban_q.front());
            ban_q.pop();
        }

        ban_q.push(key);
        ban_s.insert(key);

    
        printf("ce size: %d, best_count: %d, best edge: (%d, %d), new color: %d\n",
        gsize, best_count, best_i, best_j, g[best_i*gsize + best_j]);
        
        /*
        rinse and repeat
        */          
    
    }

}



/*
main
*/
int main(void){
    
    tabu_search_full();
    
    return 0;
}












































