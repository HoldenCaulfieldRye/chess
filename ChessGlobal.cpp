#include<iostream>
#include<string>

using namespace std;

/*useful global function. it's ridiculous there isn't a way of creating a string from two chars in one statement with C++*/
string concat(char ch1, char ch2) {
  string st;
  st = ch2;   //yes it's confusing to put ch2 first,
  st += ch1;  //but I like to see (row,column) in coordinates, 
  return st;  //and with chess it's the other way around.
}
