enum Direction {N, S, W, E, NE, NW, SE, SW, INVALID_DIRECTION};

/* error codes for Question 3 */
#define ERROR_START_STATION_INVALID -1 
#define ERROR_ROUTE_ENDPOINT_IS_NOT_STATION -2
#define ERROR_LINE_HOPPING_BETWEEN_STATIONS -3
#define ERROR_BACKTRACKING_BETWEEN_STATIONS -4
#define ERROR_INVALID_DIRECTION -5
#define ERROR_OFF_TRACK -6
#define ERROR_OUT_OF_BOUNDS -7

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width);

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width);

/* pre-supplied helper function to describe errors for Question 3 */
const char *error_description(int code);

/* presupplied helper function for converting string to Direction enum */
Direction string_to_direction(const char *token);

bool get_symbol_position(char** map, int height, int width, char target, int& r, int& c);

char get_symbol_for_station_or_line(const char name[]);

int get_length(const char name[]); //used in get_symbol_for_station_or_line

int validate_route(char** map, int height, int width, const char start_station[50], char route[512], char destination[512]);

void cut_up(char route[512], int &n_directions, Direction result[50]); //used in validate_route

/*given a direction, and the coordinates of a starting point, changes these to coordinates of 
landing point. RETURNS '!' IF OUT OF BOUNDS, '?' IF INVALID DIRECTION RECEIVED.*/
char get_next_location(char **map, int height, int width, Direction dir, int cur_r, int cur_c, int &nxt_r, int &nxt_c);

void get_name_for_station(char symbol, char station_name[512]); //used in validate_route

bool isLine(char);

bool isLine(char, char);
