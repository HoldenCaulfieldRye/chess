#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

#include "tube.h"

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  ifstream input(filename);
  height = width = 0;
  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }
  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {
  bool success = get_map_dimensions(filename, height, width);  
  if (!success)
    return NULL;
  char **m = allocate_2D_array(height, width);  
  ifstream input(filename);
  char line[512];
  char space[] = " ";
  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;
  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n]))
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}


bool get_symbol_position(char** map, int height, int width, char target, int& r, int& c)
{
  map = load_map("map.txt",height,width);  //load ASCII map in appropriate dimensions
  
  for(r=0; r<height; r++)                  //r indexed starting from 0
    {
      for(c=0; c<width; c++)               //c indexed starting from 0
	{                                  //scan in row-by-row fashion
	  if (map[r][c]==target)
	    {
	      return true; 
	    } 
	}
    }
  r = -1;                                  //reached iif target cannot be found
  c = -1;
  return false;
}


char get_symbol_for_station_or_line(const char name[]) 
{
  int name_length = get_length(name);
  int chars_found=0;                 //number of (consecutive) discovered char's which match name[]
  char ch[3];
  ifstream in[2];
	
  in[0].open("stations.txt");
  in[1].open("lines.txt");

  for(int file=0; file<2; file++) {  //look in stations.txt; if fail then look in lines.txt 

    for(int i=0; i<3; i++) {
      in[file].get(ch[i]);           //ch[] is an array of 3 consecutive characters from lines.txt
    }
  
    while(!in[file].fail()) 
      {
	for (chars_found=0; ch[2] == name[chars_found] && name[chars_found] != '\0'; chars_found++) {
	  in[file].get(ch[2]);
	}
	
	if (chars_found == name_length) {
	  in[0].close(); in[1].close(); //name[] has just been covered by ch[2], while ch[0] and ch[1] stayed back
	  return ch[0];                 //so ch[1] is a ' ', and ch[0] contains The Symbol
	}

	ch[0]=ch[1];                    //ch[] takes 1 step forward for each loop of while()
	ch[1]=ch[2];
	in[file].get(ch[2]);
      }
  }
  in[0].close(); in[1].close();         //name[] is nowhere to be found
  return ' ';
}


int get_length(const char name[])
{
  int count=1;
  for(; name[count] != '\0'; count++);
  return count;
}


int validate_route(char** map, int height, int width, const char start_station[50], char route[512], char destination[512])
{ 
  /*declaration*/ 
  char previous, current, next;
  int cur_r, cur_c, prv_r, prv_c, nxt_r, nxt_c; //coordinates for current,previous,next location
  int n_directions, line_changes;               //n_directions aka number of directions
  Direction directions[50];

  /*initialisation*/
  current = get_symbol_for_station_or_line(start_station);        //find out what station's symbol is
  get_symbol_position(map, height, width, current, cur_r, cur_c); //find out its coordinates on map
  cut_up(route, n_directions, directions);                        //from route, create a Direction array of directions
  prv_r = -1;
  prv_c = -1;
  nxt_r = -1;
  nxt_c = -1;                                                     //default values
  line_changes = 0;

  /*evaluate starting point*/
  if (current==' ') {
    return ERROR_START_STATION_INVALID;
  }

  /*evaluate landing point*/
  for (int i=0; i<n_directions; i++) {                 
    next = get_next_location(map, height, width, directions[i], cur_r, cur_c, nxt_r, nxt_c); //get nxt_r and nxt_c
      switch(next) {
      case '!': return ERROR_OUT_OF_BOUNDS;
      case '?': return ERROR_INVALID_DIRECTION;
      case ' ': return ERROR_OFF_TRACK;
      default:
	if (isLine(current, next) && current != next)
	  return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
	else if (isLine(current) && prv_r==nxt_r && prv_c==nxt_c)
	  return ERROR_BACKTRACKING_BETWEEN_STATIONS;
	else {
	  if (isLine(previous, next) && previous != next) //necessary & sufficient conditions for line change
	    line_changes++;
	  previous = current;                             //this and following 5 lines make up the updating
	  current = next;
	  prv_r = cur_r;
	  prv_c = cur_c;
	  cur_r = nxt_r;
	  cur_c = nxt_c;
	}
	break;
      }
    }
  get_name_for_station(current, destination);             //reach here iif a valid destination has been reached
  return line_changes;
}


/*from route, creates a Direction array of directions*/
void cut_up(char route[512], int &n_directions, Direction result[50])
{
  /*declaration*/
  char str[512];
  char *ch;
  
  /*intialisation*/ 
  strcpy(str,route);
  n_directions = 0;
  ch = strtok(str, ","); //strtok starts at str[0], copies str[i] until ',' met, returns pointer to this new array...
                         //...and somehow remembers where it left off on str[]
  /*induction*/
  while (ch != NULL) {   //ch null iif there was nothing for strtok to copy after the previous token iif str has been... 
    n_directions++;      //...entirely covered
    result[n_directions-1] = string_to_direction(ch);
    ch = strtok(NULL, ","); //when given 'NULL' as arg, strtok starts on str where it last left off
  }

  delete [] ch;
}


char get_next_location(char **map, int height, int width, Direction dir, int cur_r, int cur_c, int &nxt_r, int &nxt_c)
{
  switch(dir) {
  case 0:                   //North
    nxt_r = cur_r - 1;
    nxt_c = cur_c;
    break;

  case 1:                   //South
    nxt_r = cur_r + 1;
    nxt_c = cur_c;
    break;
 
  case 2:                   //West
    nxt_r = cur_r;
    nxt_c = cur_c - 1;
    break;

  case 3:                   //East
    nxt_r = cur_r;
    nxt_c = cur_c + 1;
    break;
 
  case 4:                   //North-East
    nxt_r = cur_r - 1;
    nxt_c = cur_c + 1;
    break;
 
  case 5:                   //North-West
    nxt_r = cur_r - 1;
    nxt_c = cur_c - 1;
    break;
 
  case 6:                   //South-East
    nxt_r = cur_r + 1;
    nxt_c = cur_c + 1;
    break;
 
  case 7:                   //South-West
    nxt_r = cur_r + 1;
    nxt_c = cur_c - 1;
    break;
 
  default:                  //invalid direction - encompasses 'case 8'
    return '?';
  }
  
  if (nxt_r<0 || nxt_c<0 || nxt_r>height || nxt_c>width)  //out of bounds
    return '!';

  return map[nxt_r][nxt_c]; //reach here iif coordinates are valid 
}


void get_name_for_station(char symbol, char station_name[512])
{
  ifstream in[2]; //one to locate symbol, one to make sure symbol is at the begging of a line in the text file
  char ch[2];
  int i=0;

  in[0].open("stations.txt");  
  in[0].get(ch[0]);
  in[1].open("stations.txt");  
  in[1].get(ch[1]); in[1].get(ch[1]);

  while (!in[1].fail() && (ch[1] != symbol || (int) ch[0] != 10)) {  //broken iif: failure or symbol is at the begging...
    in[0].get(ch[0]);                                                //...of a line in the text file.
    in[1].get(ch[1]);                                                //in ASCII, 10 corresponds to 'new line'
  }

  if (ch[1] == symbol) {
    in[1].get(ch[1]); //skip the ' '
    for (in[1].get(ch[1]); (int) ch[1] != 10 && i<50; in[1].get(ch[1])) { //copying text to station_name until end of line
      station_name[i] = ch[1];
      i++;
    }
    station_name[i]='\0';
  }

  in[0].close(); in[1].close();
}


bool isLine(char ch)
{
  if (ch == '*' || ch == '-' || ch == '#' || ch == '&' || ch == '$' || ch == '+' || ch == '|' || ch == '>' || ch == '<')
    return true;
  return false;
}

bool isLine(char ch1, char ch2)
{
  if(isLine(ch1) && isLine(ch2))
    return true;
  return false;
}
