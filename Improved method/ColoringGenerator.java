
//---------------------------------------------
// This class generates colorings for the graph
//---------------------------------------------

import java.math.*;

public class ColoringGenerator
{
  private boolean theMap[][];  // once object created, this IS the exact same variable as theMap in calling class Ramsey
  private long totalColorings;
  private long coloringsLeft;
  private int a[], f[];
  private int j;
  private int edges; // edges is the number of edges in a map with one less node
  private int realEdgeCount;
  private int submapSize1;
  private int submapSize2;
  private int ptsInMap; // this really equals (the real number of points) - 1

  private static Debugging debugger= new Debugging(); // for debugging


  //------------------------------------
  // Constructor that sets up the object
  //------------------------------------
  public ColoringGenerator(boolean map[][], int SubmapSize1, int SubmapSize2)
  {
    theMap = map;  // pass map here to ensure that the new coloring as being applied to same map at all times and we don't get out of bound errors later
    submapSize1 = SubmapSize1;
    submapSize2 = SubmapSize2;
    ptsInMap = theMap.length - 1;

    //find number of edges
    for (int i = 0; i < ptsInMap; i++)
    {
      edges += theMap[i].length;
    }

    realEdgeCount = edges + ptsInMap;  // adds the number of edges that are connected to the "hidden" node

    totalColorings = (long)Math.pow(2.0, (double)edges);

    a = new int[edges];
    f = new int[edges + 1];

    reset();

  }


  //------
  // Reset
  //------
  public void reset ()
  {
    for (int i = 0; i < edges; i++)
    {
      a[i] = 0;
      f[i] = i;
    }

    f[edges] = edges;

    coloringsLeft = totalColorings - 1; // subtract one because we always skip the first, all 0 coloring

  }


  //--------------------------------------------------
  // Generate next permutation (algorithm cited above)
  //--------------------------------------------------
  private int[] getNext ()
  {
    j = f[0];

    if (j == edges)  //breaks the function with last output for last possible and all subsequent permutation requests
    {
      System.out.println("Last coloring being sent");
      return a;
    }

    //System.out.print("j: " + j + "   ");

    f[0] = 0;
    f[j] = f[j + 1];
    f[j + 1] = j + 1;

    a[j] = 1 - a[j];

    coloringsLeft--;

    //debugger.printInt1DArray(f);

    return a;
  }


  //------------------------------------
  // Place the next coloring into theMap
  //------------------------------------
  public void nextColoring()
  {
    //System.out.println("nextColoring called.");

    int coloringListIndex;
    coloringListIndex = 0;

    getNext();

    while (validateColoring())
    {
      getNext();
    }


    for (int i = 0; i < ptsInMap; i++)
    {
      for(int k = 0; k < theMap[i].length; k++)
      {

        if(a[coloringListIndex] == 1)
        {
          theMap[i][k] = true;
        }
        else
        {
          theMap[i][k] = false;
        }

        coloringListIndex++;
      }
    }

    //debugger.printBool2DArray(theMap);
    //System.out.println("\n\n");

  }


  //----------------------------------------------------------------------------------
  // Counts the number of 0s and 1s in the coloring to determine whether there is even
  // a change for the mapping to have no monochromatic submaps
  //----------------------------------------------------------------------------------
  private boolean validateColoring()
  {
    // returns true if the coloring will definitely have monochromatic submaps based on the number of zeros and ones.
    // returns false if the coloring should be tested more thoroughly

    if (coloringsLeft == 0)
    {
      return false; // so if this is the last graph we do not get into an infinite loop from the calling method
    }

    int num0;
    int num1;

    num0 = 0;
    num1 = 0;

    for (int i = 0; i < a.length; i++)
    {
      if (a[i] == 0)
      {
        num0++;
      }
    }

    num1 = edges - num0;  // calculate num1 this way to usually save time over counting the number of 1s in a[]

    int min0;
    int min1;

    // submapSize1 is of color true/1; submapSize2 is of color false/0

    min0 = ptsInMap - submapSize1 + 2; // should be +1, but ptsInMap is one less than what it really is
    min1 = ptsInMap - submapSize2 + 2;

    if (num0 >= min0 && num1 >= min1)
    {
      // both submaps have enough edges for this graph coloring to be one that we need to test
      return false;
    }

    // the coloring does not have the minimum number of 1s or 0s to be a coloring we need to test more thoroughly
    return true;
  }

  //-----------------------------------------------
  // Indicates whether there are more coloring left
  //-----------------------------------------------
  public boolean hasMore ()
  {
    return coloringsLeft > 0;
  }

  //------------------------------------------------
  // Return number of combinations not yet generated
  //------------------------------------------------
  public long getNumLeft ()
  {
    return coloringsLeft;
  }

  //------------------------------------
  // Return total number of combinations
  //------------------------------------
  public long getTotal ()
  {
    return totalColorings;
  }

  //------------------------------------------------------
  // Return the binary sequence representing the coloring;
  // useful for printing edge sequence key
  //------------------------------------------------------
  public int[] getBinarySequence()
  {
    return a;
  }


  //code used to test the class

  /* public static void main(String[] args)
   {
     int ptsInMap = 5;

     boolean theMap[][];
     theMap = new boolean[ptsInMap][];

     // Create the individual boolean arrays that represent the type of connection with other points (true/false);
     // the arrays will have as many cells as its index so there is not repeated
     // connection information between the two cells involved
     // NOTE: this will be creating unbalanced arrays

     for (int i = 0; i < ptsInMap; i++)
     {
       theMap[i] = new boolean[i];
     }

     ColoringGenerator x = new ColoringGenerator(theMap);

     for (int k = 0; k<x.totalColorings; k++)
     {
       //get output
       int output[];
       output = x.getNext();

       System.out.print("     " + (k+1) +"  ");


       //print output
       debugger.printInt1DArray(output);

       System.out.println();
     }

   }*/


}  // end of class
