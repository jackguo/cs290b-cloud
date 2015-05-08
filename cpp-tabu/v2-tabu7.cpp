/*
improved
single thread.
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <queue>
#include <set>
#include <vector>
#include <algorithm>


#define MAXSIZE (541)
#define TABOOSIZE (500)
#define BIGCOUNT (9999999)



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
return the number of monochromatic cliques in the graph of size (n+1).
assuming the graph of size n is already a counter example.
We only need 6 loops instead of 7.
We only check the sub-cliques that has the new node.
*/
int CliqueCount( int *g, int gsize ){
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

    int hi;
    int hj;
    int hk;
    int hl;
    int hm;
    int hn;
    
    for(i=0; i<gsize-sgsize+1; i++){
        hi = i*gsize;

        for(j=i+1; j<gsize-sgsize+2; j++){

            hj = j*gsize;
            colr = g[hi + j];
            for(k=j+1; k<gsize-sgsize+3; k++){
            
                hk = k*gsize;
                if(colr == g[hi + k] && colr == g[hj + k]){
                    
                    for(l=k+1; l<gsize-sgsize+4; l++){
                        
                        hl = l*gsize;                
                        if(colr == g[hi + l] && colr == g[hj + l] && colr == g[hk + l]){
                            
                            for(m=l+1; m<gsize-sgsize+5; m++){

                                hm = m*gsize;
                                if(colr == g[hi + m] && colr == g[hj + m] && colr == g[hk + m] && colr == g[hl + m]){

                                    for(n=m+1; n<gsize-sgsize+6; n++){

                                        hn = n*gsize;
                                        if(colr == g[hi + n] && colr == g[hj + n] && colr == g[hk + n] && colr == g[hl + n] && colr == g[hm + n]){

                                            if(colr == g[hi + o] && colr == g[hj + o] && colr == g[hk + o] && colr == g[hl + o] && colr == g[hm + o] && colr == g[hn + o]){
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
return the count difference if we flip an edge.
We only need 5 loops.
We only check the sub-cliques that has this edge.
*/
int CliqueCountDiff( int *g, int gsize, int flip_x, int flip_y, int new_clr ){
    
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int o;
 
    /*
    init a helper array
    */
    int node[gsize - 2];
    k = 0;
    for(i=0; i<gsize; i++){
        if( (i != flip_x) && (i != flip_y) ){
            node[k] = i;
            k++;
        }
    }


    int old_clr = 1 - new_clr;
    int old_cnt = 0;
    int new_cnt = 0;

    int sgsize = 7;
    int colr;

    int hi;
    int hj;
    int hk;
    int hl;


    int clique[ sgsize ];    
    bool monochromatic;

    /*
    counter
    */
    for(i=0; i<gsize-sgsize+1; i++){
        hi = node[i] * gsize;

        for(j=i+1; j<gsize-sgsize+2; j++){

            hj = node[j] * gsize;
            colr = g[ hi + node[j] ];



            for(k=j+1; k<gsize-sgsize+3; k++){
        
                hk = node[k] * gsize;
                if(colr == g[ hi + node[k] ] && colr == g[ hj + node[k] ]){
                
                    for(l=k+1; l<gsize-sgsize+4; l++){
                    
                        hl = node[l]*gsize;                
                        if(colr == g[ hi + node[l] ] && colr == g[ hj + node[l] ] && colr == g[ hk + node[l] ]){
                        
                            for(m=l+1; m<gsize-sgsize+5; m++){

                                if(colr == g[ hi + node[m] ] && colr == g[ hj + node[m] ] && colr == g[ hk + node[m] ] && colr == g[ hl + node[m] ]){

                                    clique[0] = flip_x;
                                    clique[1] = flip_y;
                                    clique[2] = node[i];
                                    clique[3] = node[j];
                                    clique[4] = node[k];
                                    clique[5] = node[l];
                                    clique[6] = node[m];

                                    std::sort(clique, clique + 7);

                                    monochromatic = true;

                                    /*
                                    old count
                                    */
                                    if(colr == old_clr){

                                        for(n=0; n<6; n++){
                                            for(o=n+1; o<7; o++){
                                                if(g[ clique[n]*gsize + clique[o] ] != old_clr ){
                                                    monochromatic = false;
                                                    break;
                                                }
                                            }
                                            if(!monochromatic) break;
                                        }
                                        if(monochromatic) old_cnt++;
                                    }

                                    /*
                                    new count
                                    */
                                    else{

                                        for(n=0; n<6; n++){
                                            for(o=n+1; o<7; o++){
                                                if(g[ clique[n]*gsize + clique[o] ] != new_clr ){
                                                    monochromatic = false;
                                                    break;
                                                }
                                            }
                                            if(!monochromatic) break;
                                        }
                                        if(monochromatic) new_cnt++;                                        
                                    }

                                }
                            }
                        }                   
                    }
                }            
            }



        }    
    }    
    



    
    return new_cnt - old_cnt;   
}





/*
search
*/
void tabu_search(){
    int *g;
    int *new_g;
    int gsize;
    int count;
    int i;
    int j;
    int best_i;
    int best_j;
    int diff;
    int best_diff;
    

    /*
    flip possibilty
    6 -> 60%
    the higher, the faster converge
    but the lower randomness
    */
    int flip_P = 2;

    /*
    hold buffer until get to threshold
    */
    int print_buffer;
    int print_threshold = 20;

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
    start with a graph of size 8
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
        count = (gsize == 8)? 0 : CliqueCount(g, gsize);
        

        /*
        reset collector
        */
        best_k.clear();
        best_start = 0;


        /*
        if we get a counter example
        */
        if(count == 0){

            print_buffer = 0;

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
        
        best_diff = BIGCOUNT;
        int key;
        size_t sz;
        for(i=0; i<gsize; i++){
            for(j=i+1; j<gsize; j++){
                
                ra1 = rand() % 10;
                if(ra1 < flip_P){
                    /*
                    flip it
                    */
                    g[i*gsize + j] = 1 - g[i*gsize + j];
                    diff = CliqueCountDiff( g, gsize, i, j, g[i*gsize + j] );
                    

                    /*
                    is it better and the i,j not tabu?
                    */
                    key = getKey(i, j);
                    

                    /*
                    if we have multiple best counts, which one do we use?
                    we use the first one or last one ?
                    */
                    if(diff <= best_diff && ban_s.count(key) == 0){

                        if(diff == best_diff){
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

                        best_diff = diff;
                    }
                    
                    /*
                    flip it back
                    */
                    g[i*gsize + j] = 1 - g[i*gsize + j];      
                
                }
          
            }
        
        }
        
        if(best_diff == BIGCOUNT){
            printf("no best edge found, terminating\n");
            exit(1);
        }
        
        /*
        best count
        */
        count += best_diff;



        /*
        keep the best flip we saw
        */
        sz = best_k.size();
        ra1 = rand() % (sz - best_start);
        ra1 += best_start;
        key = best_k[ra1];
        best_i = getI(key);
        best_j = getJ(key);
        g[ best_i*gsize + best_j ] = 1 - g[ best_i*gsize + best_j ];
        


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
        

        print_buffer = (print_buffer == BIGCOUNT)? 0 : print_buffer + 1;
        if(print_buffer % print_threshold == 0){

            printf("ce size: %d, best_count: %d, best edge: (%d, %d), new color: %d\n", gsize, count, best_i, best_j, g[best_i*gsize + best_j]);
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
    

    tabu_search();

    return 0;
}












































