#include<iostream>
#include<string>

using namespace std;

/*useful global function. it's ridiculous there isn't a way of creating a string from two chars in one statement with C++*/
string concat(char ch1, char ch2) {
  string st;
  st = ch1;
  st += ch2;
  return st;
}
