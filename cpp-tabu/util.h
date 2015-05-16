
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
Read Graph
*/
int ReadGraph( char *fname, int **g, int *gsize ){
    int i;
    int j;
    FILE *fd;
    int lsize;
    int *lg;
    char line_buff[255];
    char *curr;
    char *err;
    char *tempc;
    int lcount;

    fd = fopen(fname,"r");
    if(fd == 0)
    {
        fprintf(stderr,"ReadGraph cannot open file %s\n",fname);
        fflush(stderr);
        return(0);
    }

    fgets(line_buff,254,fd);
    if(feof(fd))
    {
        fprintf(stderr,"ReadGraph eof on size\n");
        fflush(stderr);
        fclose(fd);
        return(0);
    }   
    i = 0;
    while((i < 254) && !isdigit(line_buff[i]))
        i++;
    
    /*
     * first line of the file must contain a size
     */
    if(!isdigit(line_buff[i]))
    {
        fprintf(stderr,"ReadGraph format error on size\n");
        fflush(stderr);
        fclose(fd);
        return(0);
    }
    tempc = line_buff;
    lsize = (int)strtol(tempc,&tempc,10);
    if((lsize < 0) || (lsize > MAXSIZE))
    {
        fprintf(stderr,"ReadGraph size bad, read: %d, max: %d\n", lsize, MAXSIZE);
        fflush(stderr);
        fclose(fd);
        return(0);
    }

    lg = (int *)malloc(lsize*lsize*sizeof(int));
    if(lg == NULL)
    {
        fprintf(stderr,"ReadGraph: no space\n");
        fflush(stderr);
        return(0);
    }

    memset(lg,0,lsize*lsize*sizeof(int));

        for(i=0; i < lsize; i++)
        {
        if(feof(fd))
        {
            break;
        }
        err = fgets(line_buff,254,fd);
        if(err == NULL)
        {
            break;
        }
        curr = line_buff;
        
        for(j=0; j < lsize; j++)
        {
            sscanf(curr,"%d ",&(lg[i*lsize+j]));
            if((lg[i*lsize+j] != 1) && (lg[i*lsize+j] != 0))
            {
                fprintf(stderr, "ReadGraph: non-boolean value read: %d\n", lg[i*lsize+j]);
                fflush(stderr);
                fclose(fd);
                return(0);
            }
            while(isdigit(*curr))
                curr++;
            while(isspace(*curr))
                curr++;
        }
    }

    if(i < lsize)
    {
        fprintf(stderr,"ReadGraph file too short, lsize: %d\n",lsize);
        fflush(stderr);
        fclose(fd);
        return(0);
    }

    fclose(fd);

    *g = lg;
    *gsize = lsize;
    return(1);
}





/*
helper functions
assuming i, j <= 540
*/
int getKey(int i, int j){
    return i*1000 + j;
}
int getI(int k){
    return k / 1000;
}
int getJ(int k){
    return k % 1000;
}
void clearQ( std::queue<int> &q){
    std::queue<int> empty;
    std::swap( q, empty );
}

/*
convert a number to a string
*/
std::string num2str( int num ){
    std::stringstream ss;
    ss << num;
    return ss.str();
}

/*
get a random edge when given 
a graph's size.
*/
int getRandEdge( int gsize ){
    int i = rand() % (gsize - 1);
    int j = (i == gsize - 2)? gsize - 1 : i + 1 + rand() % (gsize - i - 1);

    return getKey(i,j);
}




/*
return the number of monochromatic cliques in the graph.
flip_new_edge_only = false : O(n^7)
flip_new_edge_only = true  : O(n^6)
*/
int CliqueCount( int *g, int gsize, bool flip_new_edge_only ){
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

                                            if(flip_new_edge_only){
                                                if(colr == g[hi + o] && colr == g[hj + o] && colr == g[hk + o] && colr == g[hl + o] && colr == g[hm + o] && colr == g[hn + o]){
                                                    count++;
                                                }
                                            }
                                            else{

                                                for(o=n+1; o<gsize-sgsize+7; o++){
                                                    if(colr == g[hi + o] && colr == g[hj + o] && colr == g[hk + o] && colr == g[hl + o] && colr == g[hm + o] && colr == g[hn + o] ){
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
    }    
    


    return count;
}

