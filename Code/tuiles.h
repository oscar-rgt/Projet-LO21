#include <iostream>
#include <cstring>

typedef enum{1 2 3} Orientation;

class Tuile {
private :
	int id, niveau;
	Orientation orientation;
	hexagone hexagones[3];
};