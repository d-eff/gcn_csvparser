#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

void parseCSV(fstream &file, vector<vector<string> > &rowList);
int bSearch(string searchItem, vector<vector<string> > &searchVector, int col);
void printCSV(vector<vector<string> > &rowList, ofstream &file, bool addCustomData, string customData);

int main(int argc, char* argv[])
{
  //open the file, if provided
  if(argc >= 3)
  {
    //fstream to hold the file
    fstream file;

    //2d vectors for the csvs
    vector<vector<string> > inputCSV;
    vector<vector<string> > lookupCSV;
    
    //open the first file (the one with the data we want to update)
    file.open(argv[1]);
    if(file.good())
    {
      parseCSV(file, inputCSV);
      file.close();
      
      //open the second file 
      file.open(argv[2]);
      if(file.good())
      {
        parseCSV(file, lookupCSV);
        file.close();
        //both files parsed
  
    //deal with additional flags
    int exArgs = 3;

    bool splitOnNotFound = false;
    bool addOutputColumn = false;
    bool omitHeader = false;
    bool useCustomHeader = false;

    int idColumn = 0;
        int lookupColumn = 0;
    int locColumn = 1;
    
    int foundIndex;

    string outputColHeader = "";
    string outputData = "";
    string customHeader = "";
    
    while(exArgs < argc)
    {
      switch(argv[exArgs][1])
      {
        case 's':
          splitOnNotFound = true;
          exArgs++;
        break;
        case 'a':
          addOutputColumn = true; 
          outputColHeader += '\"';
          outputColHeader += argv[exArgs+1];
          outputColHeader += '\"';
          outputData = argv[exArgs+2];
          exArgs += 3;
        break;
        case 'c':
          idColumn = atoi(argv[exArgs+1]);
          lookupColumn = atoi(argv[exArgs+2]);
          locColumn = atoi(argv[exArgs+3]);
          exArgs += 4;
        break;
        case 'o':
          omitHeader = true;
          exArgs++;
        break;
        case 'h':
          useCustomHeader = true;
          customHeader = argv[exArgs+1];
          exArgs++;
        break;
        default:
          exArgs++;
      }
    }

     
        vector<string> foundRow;
        vector<vector<string> > found;
    vector<vector<string> > notFound;
    

    //add the header to the output file(s)    
    if(!omitHeader) {
      found.push_back(inputCSV[0]);

      if(splitOnNotFound)
        notFound.push_back(inputCSV[0]);
    }
  
    //add the custom header to the output file(s)
    if(useCustomHeader)
    {
        found[0].push_back(customHeader);

        if(splitOnNotFound)
          notFound[0].push_back(customHeader);
    } else {
        found[0].push_back(lookupCSV[0][locColumn]);

        if(splitOnNotFound)
          notFound[0].push_back(lookupCSV[0][locColumn]);
    }


    if(addOutputColumn)
    {
      found[0].push_back(outputColHeader);

      if(splitOnNotFound)
        notFound[0].push_back(outputColHeader);
    }



        //loop all through the inputCSV
        for(unsigned int row = 1; row < inputCSV.size(); ++row)
        {
          //for each one, bsearch the lookupCSV, get the row it's in back
          foundIndex = bSearch(inputCSV[row][idColumn], lookupCSV, lookupColumn);
          if(foundIndex >= 0)
          { 
            //if we found it, get the data we want from the foundIndex row of lookupCSV
            //then push it to the original SE Row data, then push that
            //to the found vector
            foundRow = inputCSV[row];

            //loop through any remaining args and push the specified columns
            //should really add error checking to spots like this
            //for(int argCount = 5; argCount < argc; argCount++)
            //{
             //int locColumn = atoi(argv[5]);
              foundRow.push_back(lookupCSV[foundIndex][locColumn]);
            //}
            found.push_back(foundRow);
            foundRow.clear();
          } else {
      vector<string> unFound = inputCSV[row];            
      //for(int argCount = 5; argCount < argc; argCount++)
      //{
        unFound.push_back("");
      //}
      if(splitOnNotFound)
      {
        notFound.push_back(unFound);
      } else
      {
        found.push_back(unFound);
      }
          }
        }

        ofstream outFile("found.csv");
        cout << "found: " << found.size() << '\n';
        printCSV(found, outFile, addOutputColumn, outputData);
        outFile.close();

    if(splitOnNotFound) {
      outFile.open("insert.csv");
      cout << "not found: " << notFound.size() << '\n';
      printCSV(notFound, outFile, addOutputColumn, outputData);
      outFile.close();
    }
      }
    }

  } else {
    cout << "Syntax is: ./sfse_parse <SE File> <SF File> <SE Col> <SF lookup col> <one or more SF output cols>\n";
  }
}

void parseCSV(fstream &file, vector<vector<string> > &rowList) 
{
  vector<string> row;
  string cell;
  char c;
  
  //if the file isn't empty or failed or something, grab the first char
  c = file.get();

  //now until we hit the end, keep peeling and alayzing each char in fstream
  while(file.good())
  {
    switch(c)
    {
      case ',':
        //if it's a comma, we're between cells. push cell to row.
        if(cell[0] == '"' && cell[cell.length()-1] != '"'){
          cell += '"';
        }
        row.push_back(cell);
        cell.clear();
        break;
      case '\n':
        //if it's a newline, we're at the end of the line. push cell to row and
        //row to rowList
        if(cell[0] == '"' && cell[cell.length()-1] != '"'){
          cell += '"';
        }
        row.push_back(cell);
        rowList.push_back(row);

        cell.clear();
        row.clear();
        break;
      case '"':
        //this is where it gets a tad messy - if it's a double quote,
        //loop until we hit the next double quote. ignore commas, newlines, and
        //anything else in between. sould probably add the ability to \"
        cell += c;
        do {
          c = file.get();
          cell += c;
        } while(c != '"');
        break;
      default:
        //if we made it this far, just add it to the cell
        if(cell.empty()) {
          cell += '"';
        }
        cell += c;
    }  
    c = file.get();
  }
}

//boilerplate binary search
int bSearch(string searchItem, vector<vector<string> > &searchVector, int col)
{
  int max = searchVector.size();
  int min = 0;
  while(max >= min)
  {
    int mid = (max + min)/2;
    if(searchVector[mid][col].compare(searchItem) < 0){
      min = mid + 1;
    } else if(searchVector[mid][col].compare(searchItem) > 0){
      max = mid - 1;
    } else {
      return mid;
    }
  }
  return -1; 
}

void printCSV(vector<vector<string> > &rowList, ofstream &file, bool addCustomData, string customData)
{
      for(unsigned int x = 0; x < rowList.size(); x++)
      {
        for(unsigned int y = 0; y < rowList[x].size(); y++)
        {
          file << rowList[x][y];

          if(y < rowList[x].size()-1)
            file << ',';
        }
    if(x > 0 && addCustomData)
      file << ',' << '\"' << customData << '\"';
        file << '\n';
      }
}
