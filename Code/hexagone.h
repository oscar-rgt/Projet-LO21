#include <iostream>
#include <cstring>

class HexaException {
	string info;
public:
	HexaException(const string& s) :info(s){}
	const string& getInfo() const { return info; }
};


typedef enum { Habitation, Marche, Caserne, Temple, Jardin, Place } Type;


class Hexagone {
private:
	friend class Tuiles;
	int niveau, etoiles;
	Type type;
	bool recouvert;

	Hexagone(Type t, int e = 0, int n = 1, bool r = false);
	~Hexagone() {}
	Hexagone(const Hexagone& h) = delete;
	Hexagone& operator=(const Hexagone&) = delete;
	Type getType() const { return type; }
	int getEtoiles() const { return etoiles; }
	int getNiveau() const { return niveau; }
	bool est_recouvert() const { return recouvert; }
	void recouvrir(bool r);
};
