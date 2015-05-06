import java.util.*;

public class Population
{
    ArrayList<Chromosome> pop;
    public Population(int size, int graphsize)
    {
	pop = new ArrayList<Chromosome>();
	for(int i=0; i<size; i++)
	    {
		//make a new graph
		AdjMatrixGraph G = new AdjMatrixGraph(graphsize);
		//then a new coloring for that graph
		ColorMatrix C = new ColorMatrix(G);
		//then add that coloring to the pop
		pop.add(new Chromosome(C));
	    }
    }
    //blank constructor to use for manual inserting (from file)
    public Population()
    {
	pop = new ArrayList<Chromosome>();
    }

    //makes a pop from a pre formed list of chromosomes
    public Population(ArrayList<Chromosome> list)
    {
	//hopefully this copies everything over, might need to make a manual move
	System.out.println(list);
	pop = list;
    }

    public int getSize()
    {
	return pop.size();
    }
    
    public Chromosome getWorst()
    {
	Chromosome worst = pop.get(0);
	for(Chromosome c:pop)
	    {
		if(c.getFitness(7)>worst.getFitness(7))
		    worst = c;
	    }
	return worst;
    }

    public int getWorstPosition()
    {
	int worst = 0;
	for(int i=0; i<pop.size(); i++)
	    {
		if(pop.get(i).getFitness(7)>pop.get(worst).getFitness(7))
		    worst = i;
	    }
	return worst;
    }

    public Chromosome getBest()
    {
	Chromosome best = pop.get(0);
	for(Chromosome c:pop)
	    {
		if(c.getFitness(7)<best.getFitness(7))
		    best = c;
	    }
	return best;
    }

    public int getBestPosition()
    {
	int best = 0;
	for(int i=0; i<pop.size(); i++)
	    {
		if(pop.get(i).getFitness(7)<pop.get(best).getFitness(7))
		    best = i;
	    }
	return best;
    }


    @Override
    public String toString()
    {
	int i = 0;
	String p="";
	int terms = 49;
	for(Chromosome c:pop)
	    {
		//p+="Fitness for graph "+i+": "+c.getFitness(7);
		p+=c.getFitness(7)+"  ";
		terms--;
		//if(c.getFitness(7)==0)
		    //    c.getColorMatrix().printColoring();
		if(terms==0)
		    {
			p+="\n";
			terms=49;
		    }
		i++;
	    }
	return p;
    }

    public Chromosome getChromosome(int index)
    {
	return pop.get(index);
    }
    public void addChromosome(int index, Chromosome c)
    {
	//if element at index exists replace it
	if(pop.get(index) != null)
	    pop.set(index, c);
	//otherwise just add it
	else
	    pop.add(c);
    }
    public void addChromosome(Chromosome c)
    {
	pop.add(c);
    }
    public ArrayList<Chromosome> getPop()
    {
	return this.pop;
    }
}
