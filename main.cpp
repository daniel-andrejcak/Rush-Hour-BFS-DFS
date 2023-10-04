/*root node grafu je pociatocna situacia, treba vymysliet program, ktory bude schopny spravit vsetky mozne kroky pre danu situaciu
bude existovat queue, uzol a sa dostane do queue, zacne sa spracovat. spracovat znamena,ze sa vyberie z queue, skontroluje sa ci uz neni koniec a vsetky mozne situacie,
ktore mozu nasledovat, reprezentovane uzlami daju DO QUEUE NA KONIEC a to zabezpeci ze to bude vyhladavanie do sirky
ak sa namiesto queue pouzije stack, teda ze vsetky mozne situacie, ktore mozu nastat sa budu davat DO STACKU NA ZACIATOK, tak to bude prehliadavanie do hlbky*/

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

int BOUNDARIES = 5;
int BORDER = 6;
//std::vector<Node> stack; //konkretne pre DFS, moze byt queue ked sa robi BFS


class Car {
public:
	Car() {
		xAxis = 0;
		yAxis = 0;
		size = 0;
		move = '0';
	};

	Car(std::string c, int x, int y, int s, char m) {
		color = c;
		xAxis = x;
		yAxis = y;
		size = s;
		move = m;
	};

	~Car() {};

	std::string color;
	int xAxis;
	int yAxis;
	int size;
	char move;
};


class Node {
public:
	Node() { node = nullptr; };
	~Node() {};

	std::vector<Car> cars;
	Node *node;
};




//kontrola pohybu auta, skontroluje, ci je pohyb auta mozny
void moveValidation(Node node, Car movedCar) {
	//node.cars.erase(std::find(node.cars.begin(), node.cars.end(), car));
	
	for (Car car : node.cars) {
		if (car.color != movedCar.color) {

		}
	}
}


Node *up(Node *node, std::string color, int n) {
	Car* car = nullptr;

	for (auto& c : node->cars)
	{
		if (c.color == color) {
			car = &c;
			//node.cars.erase(std::find(node.cars.begin(), node.cars.end(), c));
			break;
		}
	}

	if (car != nullptr) {
		for (auto& c : node->cars)
		{
			if (c.color != color) {

				//ak sa nad autom uz nachadza nejake ine auto otocene vertikalne
				if (c.move == 'v' && c.xAxis == car->xAxis && c.yAxis + c.size <= car->yAxis && c.yAxis + c.size >= car->yAxis - n) {
					std::cout << "neda sa pohnut" << std::endl;
					return nullptr;
				}
				//ak sa nad autom nachadza ine auto otocene horizontalne
				else if (c.move == 'h' && c.xAxis <= car->xAxis && c.xAxis + c.size > car->xAxis && c.yAxis < car->yAxis && c.yAxis >= car->yAxis - n) {
					std::cout << "neda sa pohnut" << std::endl;
					return nullptr;
				}
			}
		}
	}


	std::cout << "auto sa uspesne pohlo" << std::endl;

	if (car != nullptr) { car->yAxis -= n; }
		
	return node;
}

Node* down(Node* node, std::string color, int n) {
	Car* car = nullptr;

	for (auto& c : node->cars)
	{
		if (c.color == color) {
			car = &c;
			//node.cars.erase(std::find(node.cars.begin(), node.cars.end(), c));
			break;
		}
	}

	if (car != nullptr) {
		for (auto& c : node->cars)
		{
			if (c.color != color) {

				//ak sa pod autom uz nachadza nejake ine auto otocene vertikalne
				if (c.move == 'v' && c.xAxis == car->xAxis && c.yAxis >= car->yAxis + car->size && c.yAxis <= car->yAxis + car->size + n) {
					std::cout << "neda sa pohnut" << std::endl;
					return nullptr;
				}
				//ak sa pod autom nachadza ine auto otocene horizontalne
				else if (c.move == 'h' && c.xAxis <= car->xAxis && c.xAxis + c.size > car->xAxis && c.yAxis > car->yAxis && c.yAxis <= car->yAxis + n) {
					std::cout << "neda sa pohnut" << std::endl;
					return nullptr;
				}
			}
		}
	}


	std::cout << "auto sa uspesne pohlo" << std::endl;

	if (car != nullptr) { car->yAxis += n; }

	return node;
}

Node* up(Node* node, std::string color, int n) {
	Car* car = nullptr;

	for (auto& c : node->cars)
	{
		if (c.color == color) {
			car = &c;
			//node.cars.erase(std::find(node.cars.begin(), node.cars.end(), c));
			break;
		}
	}

	if (car != nullptr) {
		for (auto& c : node->cars)
		{
			if (c.color != color) {

				//ak sa nad autom uz nachadza nejake ine auto otocene vertikalne
				if (c.move == 'v' && c.xAxis == car->xAxis && c.yAxis + c.size <= car->yAxis && c.yAxis + c.size >= car->yAxis - n) {
					std::cout << "neda sa pohnut" << std::endl;
					return nullptr;
				}
				//ak sa nad autom nachadza ine auto otocene horizontalne
				else if (c.move == 'h' && c.xAxis <= car->xAxis && c.xAxis + c.size > car->xAxis && c.yAxis < car->yAxis && c.yAxis >= car->yAxis - n) {
					std::cout << "neda sa pohnut" << std::endl;
					return nullptr;
				}
			}
		}
	}


	std::cout << "auto sa uspesne pohlo" << std::endl;

	if (car != nullptr) { car->yAxis -= n; }

	return node;
}





/*Hlavolam je vyriešený, keï je èervené auto (v smere jeho jazdy) na okraji križovatky a môže sa z nej dosta von.
Predpokladajte, že èervené auto je vždy otoèené horizontálne a smeruje doprava.*/
int checkForFinal(Node node) {
	int redX = 0;
	int redY = 0;

	for (auto& c : node.cars)
	{
		if (c.color == "cervene") {
			redX = c.xAxis;
			redY = c.yAxis;
			break;
		}
	}

	for (auto& c : node.cars)
	{
		//ak je ine auto v tom istom riadku
		if (c.move == 'h' && c.yAxis == redY && c.xAxis > redX) {
			std::cout << "nie je to mozne" << std::endl;
			return 0;
		}

		//ak ine auto zasahuje do riadku
		if (c.move == 'v' && c.xAxis > redX && c.yAxis + c.size >= redY && c.yAxis <= redY) {
			std::cout << "auto sa nemoze dostat z parkoviska" << std::endl;
			return 0;
		}
	}

	std::cout << "cervene auto moze vyjst z parkoviska" << std::endl;

	return 1;
}

/*kontrola pociatocneho stavu - skontroluje, ci kazde auto pri vzniku je na korektnej pozicii,
zle umiestnene auta nevzniknu a nebudu umiestnene na hracie pole
predpokladame, ze uzivatel umiestni auta tak, ze sa nebudu prekryvat*/
Car carValidation(std::string color, int xAxis, int yAxis, int size, char move) {
	if (color == "cervene" && move != 'h') {
		std::cout << "red car must be horizontal" << std::endl;

		exit(0);
	}

	if (size != 2 && size != 3) {
		std::cout << "wrong size of car" << std::endl;

		exit(0);
	}

	if ((move == 'h' && xAxis + size - 1 > BOUNDARIES) || (move == 'v' && yAxis + size - 1 > BOUNDARIES) || xAxis >= BOUNDARIES || yAxis >= BOUNDARIES) {
		std::cout << "out of bounds" << std::endl;

		exit(0);
	}

	std::cout << color << " car is positioned correctly" << std::endl;

	return Car(color, xAxis, yAxis, size, move);
}


/*zabezpecenie vsetkych moznych pohybov, pre kazde auto vyskusa kazdy pohyb o kazdy mozny pocet policok*/
std::vector<Car> algorithm(std::vector<Car> root){
	return root;
}

int main() {
	std::cout << "zadat farbu, x polohu, y polohu, velkost, smer pohybu" << std::endl;

	Car car = carValidation("cervene", 1, 2, 2, 'h');
	Car car1 = carValidation("zelene", 3, 3, 3, 'v');
	Car car2 = carValidation("modre", 3, 0, 2, 'h');

	//Car car1 = carValidation("modre", 2, 1, 2, 'v');

	std::cout << car.color << std::endl;

	Node node = Node();

	node.cars.push_back(car);
	node.cars.push_back(car1);
	node.cars.push_back(car2);
	

	Node oldNode = node;

	std::cout << checkForFinal(node) << std::endl;

	Node *newNode = up(&node, "zelene", 2);

	std::cout << checkForFinal(node) << std::endl;


	std::cout << newNode->cars.at(1).yAxis << std::endl;

	return 0;
}