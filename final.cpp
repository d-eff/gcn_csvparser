#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

void parseCSV(fstream &file, vector<vector<string> > &rowList);
int bSearch(string searchItem, vector<vector<string> > &searchVector, int col);
void printCSV(vector<vector<string> > &rowList, ofstream &file);

int main(int argc, char* argv[])
{
  //open the file, if provided
  if(argc > 5)
  {
    //fstream to hold the file
    fstream file;

    //2d vectors for the csvs
    vector<vector<string> > seData;
    vector<vector<string> > sfData;
    
    //open the first file (the one with the data we want to update)
    file.open(argv[1]);
    if(file.good())
    {
      parseCSV(file, seData);
      file.close();
      
      //open the second file 
      file.open(argv[2]);
      if(file.good())
      {
        parseCSV(file, sfData);
        file.close();
        //both files parsed, yay

        int foundIndex;
        int idColumn = atoi(argv[3]);
        int lookupColumn = atoi(argv[4]);
        int locColumn = atoi(argv[5]);
        
        vector<string> foundRow;
        vector<vector<string> > found;
        vector<vector<string> > notFound;

        //loop all through the seData
        for(int row = 0; row < seData.size(); ++row)
        {
          //for each one, bsearch the sfData, get the row it's in back
          foundIndex = bSearch(seData[row][idColumn], sfData, lookupColumn);
          if(foundIndex >= 0)
          { 
            //if we found it, get the data we want from the foundIndex row of SFdata
            //then push it to the original SE Row data, then push that
            //to the found vector
            foundRow = seData[row];
            foundRow.push_back(sfData[foundIndex][locColumn]);
            found.push_back(foundRow);
            foundRow.clear();
          } else {
            notFound.push_back(seData[row]);
          }
        }

        ofstream outFile("found.csv");
        cout << "found: " << found.size() << '\n';
        printCSV(found, outFile);
        outFile.close();

        outFile.open("notFound.csv");
        cout << "not found: " << notFound.size() << '\n';
        printCSV(notFound, outFile);
        outFile.close();
      }
    }

  } else {
    cout << "Syntax is: ./sfse_parse <SE File> <SF File> <SE Col> <SF lookup col> <SF output col>\n";
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

void printCSV(vector<vector<string> > &rowList, ofstream &file)
{
      for(int x = 0; x < rowList.size(); x++)
      {
        for(int y = 0; y < rowList[x].size(); y++)
        {
          file << rowList[x][y];

          if(y < rowList[x].size()-1)
            file << ',';
        }
        file << '\n';
      }
}
