#pragma once
//raz to bude treba vymenit za string
enum farba {
	BIELE, ZLTE, ORANZOVE, CERVENE, FIALOVE, MODRE, ZELENE, HNEDE, SIVE, CIERNE
};

class Car {
public:

	std::string color;
	unsigned short xAxis;
	unsigned short yAxis;
	unsigned short size;
	char dir;

	Car() {
		xAxis = 0;
		yAxis = 0;
		size = 0;
		dir = 0;
	};

	Car(std::string c, unsigned short x, unsigned short y, unsigned short s, char d) {
		color = c;
		xAxis = x;
		yAxis = y;
		size = s;
		dir = d;
	};

	~Car() {};


	bool operator==(const Car& other) const {
		return (color == other.color) && (xAxis == other.xAxis) && (yAxis == other.yAxis) && (size == other.size) && (dir == other.dir);
	}

};


class Node {
public:

	std::vector<Car> cars;
	Node* pNode = nullptr;
	std::string color;
	char dir;
	unsigned short n;

	Node() {};

	~Node() {};


	bool operator==(const Node*& other) const {
		return cars == other->cars;
	}	
};

