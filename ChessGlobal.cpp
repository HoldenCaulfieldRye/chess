#include<iostream>
#include<string>

using namespace std;

#include "ChessBoard.hpp"

/*useful global function. it's ridiculous there isn't a way of creating a string from two chars in one statement with C++*/
string concat(char ch1, char ch2) {
  string st;
  st = ch2;   //yes it's confusing to put ch2 first,
  st += ch1;  //but I like to see (row,column) in coordinates, 
  return st;  //and with chess it's the other way around.
}

string whosep(WhosePiece piece) {
  if(piece == NOPIECE)
    return "NOPIECE"; 
  if(piece == FRIEND)
    return "FRIEND"; 
  if(piece == FOE)
    return "FOE";
  return "ERROR"; 
}
