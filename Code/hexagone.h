#include <iostream>
#include <cstring>


typedef enum { Habitation, Marche, Caserne, Temple, Jardin } Type;


class Hexagone {
private:
	Type type;
	bool recouvert;
};
