

//----------------------------------------------------------------------------------------
// This class contains all the essential functions and methods to calculate Ramsey numbers
//----------------------------------------------------------------------------------------


import java.util.*;
import javax.swing.*;
import java.math.*;

class Ramsey
{

  private static Debugging debugger = new Debugging();  // object to call debugging methods/functions


  //-------------------------------------------------------------------------------
  // Purpose: to initiate and manage the core processes to calculate Ramsey numbers
  //-------------------------------------------------------------------------------
  public static void main(String[] args)
  {
    /* Purpose of program:
     * This program calculates Ramsey numbers in ideally the least amount of time possible
     */

    // initial actions before we start computational processes
    initialAcations();

    // define user-inputted variables

    // number of points in the map; we will be searching for submaps within the map of this size
    int ptsInMap;

    // variables for holding the sizes of the subgraphs that we will be searching for
    int submapSize1;
    int submapSize2;


    //Get the inputs
    ptsInMap = getInput("How many nodes would you like in the graph?\nThis is essentially a hypothesis for what you expect this Ramsey number to be.",
                        -1);
    submapSize1 = getInput("We are calculating R(r,b). What would you like the value of r to be?\n(This is the size of the first subgraph size for which we will search.)",
                           ptsInMap);
    submapSize2 = getInput("We are calculating R(r,b). What would you like the value of b to be?\n(This is the size of the second subgraph size for which we will search.)",
                           ptsInMap);


    // Set submapSize1 to be the larger of the submaps to improve performance later during the while loops
    if (submapSize2 > submapSize1)
    {
      submapSize2 = submapSize2 - submapSize1;
      submapSize1 += submapSize2;
      submapSize2 = submapSize1 - submapSize2;
    }


    // report to user the inputted values
    System.out.println("We will be searching for subgraphs of size " + submapSize1 + " and " + submapSize2 + " in a map of size " + ptsInMap);
    System.out.println("More technically, we are calculating R(" + submapSize1 +", " + submapSize2 + ") with a guess of " + ptsInMap + ".");



    /*************************************************///end of user inputs, start of setting up essential variables for calculations

    // Create the array representing the points themselves
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

    // we are setting all of the connections to last node (index value: ptsInMap -1) to false
    // we can specially rotate the colors of these edges
    for (int i = 0; i < ptsInMap - 1; i++)
    {
      theMap[ptsInMap - 1][i] = false;
    }

    // initialize the object that we will use to get colorings; need to associate it with theMap
    ColoringGenerator colorGen = new ColoringGenerator(theMap, submapSize1, submapSize2);

    // variable to tell us if whether the current coloring has monochromatic graphs or not
    boolean mapColoringHasNoMonochromatics = false;

    // boolean to tell if we have reached the last coloring and need to quit
    boolean notFinished = true;



    /**********************************************************/// start of computation


    // This is the outermost while loop of the program. This loop cycles the different colorings of theMap
    while (notFinished)
    {

      // Fill the map with a possible combination; these commands are put into a
      // method so we can easily change the way we fill the map if we choose to use
      // a different algorithm
      notFinished = fillTheMap(theMap, colorGen);


      if (testColoringForASize(ptsInMap, submapSize2, false, theMap) && testColoringForASize(ptsInMap, submapSize1, true, theMap))
      {
        mapColoringHasNoMonochromatics = true;
        break;
      }

    }// while loop to go through all the different colorings



    /********************************************************///end of computation, determine findings

    // pass necessary parameters to determine the results of our searching and print the necessary statements
    printFindings(mapColoringHasNoMonochromatics, ptsInMap, submapSize1, submapSize2, theMap, colorGen);

  }

  //**************************************************************************************************//
  //********************************** Other methods and functions ***********************************//
  //**************************************************************************************************//


  //-------------------------------------------------------------------------------------------------------------------
  // Purpose: to print the essential information about the program and indicate the beginning of the console i/o window
  //-------------------------------------------------------------------------------------------------------------------
  private static void initialAcations()
  {
    // this method displays essential information about the program to the user
    System.out.println("\n");
    System.out.println("Program to Derive Ramsey Numbers");
    System.out.println("\n");
    System.out.println("BEGINNING OF CONSOLE I/O WINDOW:");
    System.out.println("");
  }


  //--------------------------------------------------------------------------------------------------
  // Purpose: to get the users inputs while ensuring that they are legal in the context of the program
  //--------------------------------------------------------------------------------------------------
  private static int getInput(String requestMessage, int maxValue)
  {
    int input;
    input = 0;

    String temp;
    boolean errorMessageDisplayed;

    // have a while loop so we keep asking for value until we get a valid response
    while (input <= 0)
    {
      try
      {
        errorMessageDisplayed = false;

        // Get the input
        temp = JOptionPane.showInputDialog(null, requestMessage, "Deriving Ramsey Numbers :: Input", JOptionPane.QUESTION_MESSAGE );

        // Following expression will throw a NumberFormatException if temp is does not convert to an integer (i.e. temp doesn't have just an integer in it)
        input = Integer.parseInt(temp);
      }
      catch (NumberFormatException error)
      {
        reportInvalidInput (error);
        errorMessageDisplayed = true;
      }

      // integers <= 0 will pass the above test, so just check and catch if the input is valid
      if (input <=0 && !errorMessageDisplayed)
      {
        reportInvalidInput (new NumberFormatException());
      }

      // some of the inputs have a max value (like the submaps cannot be larger than the map itself); this expression ensures this isn't true
      // note: if maxValue = -1, then there is no max value
      if(maxValue != -1 && input >= maxValue)
      {
        reportInvalidInput (new NumberFormatException());
        input = 0;  // resets input so we go continue to go through the while loop
      }

    }

    return input;
  }


  //-----------------------------------------------------------------------
  // Purpose: to report to the user that they have entered an invalid input
  //-----------------------------------------------------------------------
  private static void reportInvalidInput(NumberFormatException error)
  {

    JOptionPane.showMessageDialog(null, "Invalid input. Please enter an integer > 0. \nNote that all subgraph sizes must also be less than the number of nodes in the graph (the guess).", "Invalid input", JOptionPane.ERROR_MESSAGE);

  }


  //----------------------------------------------------------------------------------------------------------
  // Purpose: fills the given map with the next coloring and returns whether we have reached the last coloring
  //----------------------------------------------------------------------------------------------------------
  private static boolean fillTheMap (boolean theMap[][], ColoringGenerator colorGen)
  {
    if (colorGen.hasMore())
    {

        // This statement will place the next coloring into theMap
        colorGen.nextColoring();

        return true; //was false; not all possibilities have been found
      }
      else if (theMap[theMap.length - 1][theMap.length - 2] == false)
      {
        int con = 0;  // con is short for connection number

        // This while loop will find the leftmost false so that we can change it to a true.
        while (theMap[theMap.length - 1][con])
        {
          con++;
        }
        theMap[theMap.length-1][ con ]=true;

        // You must also reset colorGen so that it goes through all of its combinations between the other nodes
        colorGen.reset();

        return true; // Not all possibilities have been found
      }
      else
      {
        return false; // All possibilities have been found
      }

  }


  //----------------------------------------------------------------------
  // Purpose: to test a coloring to see whether it is monochromatic or not
  //----------------------------------------------------------------------
  private static boolean testColoringForASize (int ptsInMap, int size, boolean color, boolean theMap[][] )
  {
    int twoPoints[] = new int[2];
    int twoPointsLocation[];
    CombinationGenerator twoPointSelector;
    CombinationGenerator comboOfPointsGen = new CombinationGenerator(ptsInMap, size);
    int pointCombination[];



    while (comboOfPointsGen.hasMore())
    {
      // get the combination
      pointCombination = comboOfPointsGen.getNext();

      //setup the twoPointSelector CombinationGenerator to check all edges in the submap
      twoPointSelector = new CombinationGenerator(pointCombination.length, 2);



      // RESET submapIsMonochromatic variables for next set of points/submap
      boolean submapIsMonochromatic = true;

      while (twoPointSelector.hasMore())
      {
        // find the position of the two points' indices in the current pointCombination, find
        // the actual indices and save those to a separate array, and then check whether the
        // line between the indices is false
        // Note that we use different array for the position of the indices in pointCombination
        // and the actual points because twoPointLocation is referenced to the private array twoPointSelector.a

        twoPointsLocation = twoPointSelector.getNext();
        twoPoints[0] = pointCombination[twoPointsLocation[0]];
        twoPoints[1] = pointCombination[twoPointsLocation[1]];


        // if we find edge that is false record that the submap we are analyzing
        // is not monochromatic with trues; do the opposite if edge is true
        // we can skip the rest of this submap's analysis if we already know that the
        // subgraph is not monochromatic; knowing of more edges will not help us
        if (theMap[ twoPoints[1] ][ twoPoints[0] ] != color)
        {
          submapIsMonochromatic = false;
          break;
        }


      } // while (twoPointSelector.hasMore())


      // check to see if we found that the submap was monochromatic (either true OR false); if it was,
      // then this coloration of theMap is not free of monochromatic submaps of the user’s given sizes
      if (submapIsMonochromatic == true)
      {
        return false;  // the function will only return false if there IS a monochromatic submap
      }

    } // while (comboOfPointsGen.hasMore())


    return true;  // the function will only return true if there IS NOT a monochromatic submap

  }


  //-----------------------------------------------------------------------------------
  // Purpose: to print the findings and then indicate the end of the console i/o window
  //-----------------------------------------------------------------------------------
  private static void printFindings(boolean mapColoringHasNoMonochromatics, int ptsInMap, int submapSize1, int submapSize2, boolean theMap[][], ColoringGenerator colorGen)
  {
    System.out.println();

    if (mapColoringHasNoMonochromatics == true)
    {
      System.out.println("This graph of " + ptsInMap + " nodes has a coloring with no monochromatic graphs. Ramsey number is definitely greater than " + ptsInMap + ".");
      System.out.println("More technically: R(" + submapSize1 +", " + submapSize2 + ") > " + ptsInMap + ".");

      //call method to print the Edge Sequence Key
      printEdgeSequenceKey(ptsInMap, submapSize1, submapSize2, theMap, colorGen);

    }
    else
    {
      System.out.println("This graph ONLY has monochromatic colorings. Ramsey number is this number of nodes (" + ptsInMap + ") or lower.");
      System.out.println("More technically: R(" + submapSize1 +", " + submapSize2 + ") <= " + ptsInMap + ".");
    }

    System.out.println("\nEND OF CONSOLE I/O WINDOW");
  }


  //---------------------------------------------------------------------------------------------------------------------------------
  // Purpose: prints  programmer defined code called the "Edge Sequence Key" for use in later graphically displaying a map's coloring
  //---------------------------------------------------------------------------------------------------------------------------------
  private static void printEdgeSequenceKey (int ptsInMap, int submapSize1, int submapSize2, boolean theMap[][], ColoringGenerator colorGen)
  {
    int[] binarySequence = colorGen.getBinarySequence();

    System.out.println();
    System.out.println("The Edge Sequence Key can be used to display a picture of the graph with no monochromatic subgraphs in it that the");
    System.out.println("program discovered. Enter this when requested in the appropriate program. The Edge Sequence Key is:");
    System.out.print(ptsInMap + "X" + submapSize1 + "X" + submapSize2 + "X");

    for (int i = 0; i < binarySequence.length; i++)
    {
      System.out.print(binarySequence[i]);
    }


    // append the information for the last point as that has been left out of colorGen's
    // binarySequence array for performance/calculating time reductions
    for (int i = 0; i < ptsInMap - 1; i++)
    {
      if (theMap[ptsInMap - 1][i] == true)
      {
        System.out.print("1");
      }
      else
      {
        System.out.print("0");
      }

    }


    System.out.println();

  }


} // END OF CLASS RAMSEY
