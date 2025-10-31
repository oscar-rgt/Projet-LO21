#pragma once
#include <iostream>
#include <cstring>

typedef enum { Caserne,
Ville
//....
} typeTuile;

class Tuile {
private :
	int id;
	typeTuile type;
	//....
};