#include <iostream>
#include <cstring>
#include "hexagone.h"


Hexagone(Type t, int e = 0, int n = 1, bool r = false) : type(t), niveau(n), recouvert(r) {
	if (t == Place) {
		etoiles = e;
	}
}