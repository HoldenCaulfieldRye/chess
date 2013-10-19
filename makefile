strtok: strtok.cpp
	g++ -Wall -g strtok.cpp -o strtok

strtok2: strtok2.cpp
	g++ -Wall -g strtok2.cpp -o strtok2

tube: tube.cpp main.cpp tube.h
	g++ -Wall -g main.cpp tube.cpp -o tube

# make tube_functions: tube.cpp
# 	g++ -Wall -g tube.cpp -o tube_functions

# make tube_main: main.cpp
# 	g++ -Wall -g main.cpp -o tube_main