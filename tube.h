enum Direction {N, S, W, E, NE, NW, SE, SW, INVALID_DIRECTION};

/* error codes for Question 3 */
#define ERROR_START_STATION_INVALID -1 
#define ERROR_ROUTE_ENDPOINT_IS_NOT_STATION -2
#define ERROR_LINE_HOPPING_BETWEEN_STATIONS -3
#define ERROR_BACKTRACKING_BETWEEN_STATIONS -4
#define ERROR_INVALID_DIRECTION -5
#define ERROR_OFF_TRACK -6
#define ERROR_OUT_OF_BOUNDS -7

/* pre-supplied function to load a tube map from a file */
char **load_map(const char *filename, int &height, int &width);


/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width);


/* pre-supplied helper function to describe errors for Question 3 */
const char *error_description(int code);


/* presupplied helper function for converting string to Direction enum */
Direction string_to_direction(const char *token);

/*============================== Question 1 ====================================*/

bool get_symbol_position(char** map, int height, int width, char target, int& r, int& c);



/*============================== Question 2 ====================================*/

char get_symbol_for_station_or_line(const char name[]);



/*============================== Question 3 ====================================*/

int validate_route(char** map, int height, int width, const char start_station[50], char route[512], char destination[512]);

/* given route, assigns to result an array of directions */
void cut_up(char route[512], int &n_directions, Direction result[50]);

/* given starting coordinates and a direction, assigns to nxt_r, nxt_c coordinates of landing point */
char get_next_location(char **map, int height, int width, Direction dir, int cur_r, int cur_c, int &nxt_r, int &nxt_c);

/* given a char, assigns to station_name the corresponding station if it exists */
void get_name_for_station(char symbol, char station_name[512]);

/* tests for whether a char corresponds to a line in lines.txt */
bool isLine(char);

/* tests for whether two chars each correspond to a line in lines.txt */
bool isLine(char, char);
