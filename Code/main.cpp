#include <iostream>
#include <cstring>
#include "main.h"


using namespace std;



int main() {
	try {
		cout << '0' + 1;
	}
	catch (const HexaException& e) {
		cout << "erreur ; " << e.getInfo();
	}
	

	return 0;
}
