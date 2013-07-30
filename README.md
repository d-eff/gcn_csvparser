###CSV Parser and Such

Takes two .csv files, parses both, then searches the latter in order to update
the former. It's VLOOKUP, but in standalone form. 

* CSV cells can be wrapped in " or not
* CSV cells can contain pretty much any char, including newline
* Doesn't include sorting - the second file must be sorted by whatever
  you're going to be matching on.

####Input: 
 - ./sfseparse \[file1\] \[lookup file\] \[file1 col\] \[lookup col\] \[output col\]
 - columns are 0-indexed.

####Output:
 - Produces two files, found.csv and notFound.csv, which contain the lines that
were found with the output column data appended, and the lines that were not
found, respectively.


