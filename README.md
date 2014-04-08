###CSV Parser and Such

Takes two .csv files, parses both, then searches the latter in order to update
the former. It's VLOOKUP, but in standalone form. 

* CSV cells can be wrapped in " or not
* CSV cells can contain pretty much any char, including newline
* Doesn't include sorting - the second file must be sorted by whatever
  you're going to be matching on.

####Input: 
  ./sfseparse \[input file\] \[lookup file\] \[flags\]

####Output:
  By default, produces one file - "found.csv", which contains all rows from the input file, with data from the lookup file appended to any lines where a match was found.

####Example:
Given two files, Foo.csv and Bar.csv:
Foo.csv
<table>
  <tr><td>Name</td><td>ID</td></tr>
  <tr><td>Jack</td><td>17</td></tr>
  <tr><td>Liz</td><td>9</td></tr>
  <tr><td>Subhas</td><td>42</td></tr>
</table>

Bar.csv
<table>
  <tr><td>ID</td><td>Department</td><td>Email</td></tr>
  <tr><td>4</td><td>Pages</td><td>Kenneth@email.com</td></tr>
  <tr><td>9</td><td>Writing</td><td>Liz@email.com</td></tr>
  <tr><td>26</td><td>Writing</td><td>Frank@email.com</td></tr>
</table>

./CSVCombiner Foo.csv Bar.csv -c 1 0 2 will attempt to find a match in column 0 of Bar.csv for the data in column 1 of each row of Foo.csv. If a match is found, it will append the data in column 2 of Bar.csv to the output file.

###Flags:
+ -s split the output into two files, "found.csv" for rows with matches and "notFound.csv" for rows without a match

+ -a \[header\] \[data\] append a column to any output files produced. both additional fields are required, but using the -o flag will prevent the display of the header field.

+ -c \[input column\] \[lookup column\] \[output column\] use custom columns for lookup and output. Default is 0 0 1, which will search column 0 of the lookup file for data matching that in column 0 of the input file, and will output the data in column 1 of the lookup file if it finds a match.

+ -o omit header lines on any files output by the program.

+ -h \[header\] use a custom header for the column of appended data in "found.csv."

