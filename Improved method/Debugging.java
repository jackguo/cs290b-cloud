/*
 Punit Shah and Jack Ingalls
 2007 New Mexico Supercomputing Challenge
 School: Albuquerque Academy
 Team: 7
 Project Name: Deriving Ramsey Numbers
*/

//------------------------------------------------------------------------------------------
// These methods help to debug aspects of the program by printing information to the console
// window or modifying variables to test cases
//------------------------------------------------------------------------------------------

class Debugging
{
  //private debugging debugger = new debugging();;

  public Debugging()
  {

  }

  public void printInt1DArray (int array[])
  {
    for (int i = 0; i < array.length; i++)
    {
      System.out.print(array[i] + " ");
    }
    System.out.println();
  }

  public void printInt2DArray (int array[][])
  {
    for (int i = 0; i < array.length; i++)
    {
      printInt1DArray(array[i]);
    }
  }

  public void printBool1DArray (boolean array[])
  {
    for (int i = 0; i < array.length; i++)
    {
      System.out.print(array[i] + " ");
    }
    System.out.println();
  }

  public void printBool2DArray (boolean array[][])
  {
    for (int i = 0; i < array.length; i++)
    {
      printBool1DArray(array[i]);
    }
  }


  public void setWholeArrayToTrue (boolean theMap[][])
  {
    // this method sets all of the values of the array to true
    for (int i = 0; i < theMap.length; i++)
    {
      for (int k = 0; k < theMap[i].length; k++)
      {
        theMap[i][k] = true;
      }
    }
  }

  public void set5NodeMapToNonmonochromaticConfig(boolean theMap[][])
  {
    //all outside painted true; all inside painted false (this is a 5 node map with no monochromatic 3 node submaps
    theMap[1][0] = true;
    theMap[2][0] = false;
    theMap[2][1] = true;
    theMap[3][0] = false;
    theMap[3][2] = true;
    theMap[3][1] = false;
    theMap[3][0] = false;
    theMap[4][0] = true;
    theMap[4][1] = false;
    theMap[4][2] = false;
    theMap[4][3] = true;
  }

}
