#pragma once

enum class Color : unsigned char {
	BIELE, ZLTE, ORANZOVE, CERVENE, RUZOVE, SVETLOMODRE, MODRE, ZELENE, SIVE, CIERNE, NULLCOLOR
};

static const std::map<std::string, Color> colorMap = {
		{"biele", Color::BIELE},
		{"zlte", Color::ZLTE},
		{"oranzove", Color::ORANZOVE},
		{"cervene", Color::CERVENE},
		{"ruzove", Color::RUZOVE},
		{"svetlomodre", Color::SVETLOMODRE},
		{"modre", Color::MODRE},
		{"zelene", Color::ZELENE},
		{"sive", Color::SIVE},
		{"cierne", Color::CIERNE},
};

Color stringToColor(const std::string& str) {
	auto it = colorMap.find(str);
	if (it != colorMap.end()) {
		return it->second;
	}
	
	return Color::NULLCOLOR;
}

std::string colorToString(const Color& color){
	for (auto& i : colorMap)
	{
		if (i.second == color) {
			return i.first;
		}
	}

	return std::string("nullcolor");
}



class Car {
public:

	Color color = Color::NULLCOLOR;
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

	Car(Color c, unsigned short x, unsigned short y, unsigned short s, char d) {
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

	Color color = Color::NULLCOLOR;
	char dir = 0;
	unsigned short n = 0;

	unsigned short depth = 0;

	Node() {};

	~Node() {};
};


struct CustomNodeEqual {
	bool operator()(const Node* node1, const Node* node2) const {

		return node1->cars == node2->cars;
	}
};

struct CustomNodeHash {
	std::size_t operator()(const Node* node) const {
		size_t hash = 0;
		for (auto& i : node->cars)
		{
			hash ^= std::hash<Color>()(i.color) + std::hash<unsigned short>()(i.xAxis) + std::hash<unsigned short>()(i.yAxis);
		}

		return hash;
	}
};

