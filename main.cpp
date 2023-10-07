/*root node grafu je pociatocna situacia, treba vymysliet program, ktory bude schopny spravit vsetky mozne kroky pre danu situaciu
bude existovat queue, uzol a sa dostane do queue, zacne sa spracovat. spracovat znamena,ze sa vyberie z queue, skontroluje sa ci uz neni koniec a vsetky mozne situacie,
ktore mozu nasledovat, reprezentovane uzlami daju DO QUEUE NA KONIEC a to zabezpeci ze to bude vyhladavanie do sirky
ak sa namiesto queue pouzije stack, teda ze vsetky mozne situacie, ktore mozu nastat sa budu davat DO STACKU NA ZACIATOK, tak to bude prehliadavanie do hlbky*/

#include<algorithm>
#include<iostream>
#include<sstream>
#include<string>
#include<unordered_set>
#include<vector>

#include"Header.h"



unsigned short BOUNDARIES = 5;


struct CustomNodeEqual {
	bool operator()(const Node* node1, const Node* node2) const {

		return node1->cars == node2->cars;
	}
};


std::ostream& operator<<(std::ostream& os, const Node* node) {
	for (auto& car : node->cars) {
		os << car.color << " x: " << car.xAxis << " y: " << car.yAxis << " size: " << car.size << " direction: " << car.dir << std::endl;
	}

	return os;
}


//funkcia vrati a odstrani prvy/posledny prvok vectora - zalezi od typu search algoritmu
//pointer na funckiu sa v main nastavi na korektnu implementaciu pop, podla pouziteho search algritmu
Node* (*pop)(std::vector<Node*>& vec);

//vec.back() je vlastne stack.top()
Node* popDfs(std::vector<Node*>& vec) {
	Node* node = vec.back();

	vec.pop_back();

	return node;
}

Node* popBfs(std::vector<Node*>& vec) {
	Node* node = vec.front();

	vec.erase(vec.begin());

	return node;
}


/*kontrola pociatocneho stavu - skontroluje, ci kazde auto pri vzniku je na korektnej pozicii,
zle umiestnene auta nevzniknu a nebudu umiestnene na hracie pole
predpokladame, ze uzivatel umiestni auta tak, ze sa nebudu prekryvat*/
Car carValidation(std::string color, unsigned short xAxis, unsigned short yAxis, unsigned short size, char move) {
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

Node* loadCars() {
	Node* root = new Node;

	std::string inputString = "cervene 1 1 2 h zelene 4 3 3 v modre 0 0 2 v";

	//docasna zmena std::cin aby citalo zo stringu - !!! len pre DEBUG !!!
	std::istringstream iss(inputString);
	std::cin.rdbuf(iss.rdbuf());

	std::string color;
	unsigned short xAxis;
	unsigned short yAxis;
	unsigned short size;
	char move;

	while (std::cin >> color >> xAxis >> yAxis >> size >> move) {
		Car car = carValidation(color, xAxis, yAxis, size, move);

		root->cars.push_back(car);

	}

	return root;
}



//unordered_set pre visited nodes, lebo vyuziva hash table na hladanie prvkov - rychlejsie ako prehladavanie celeho vectora
std::unordered_set<Node*, std::hash<Node*>, CustomNodeEqual> visited;
std::vector<Node*> nodesToProcess; //!!!premenovat!!!



//kontrola ci je pohyb platny, vrati index auta, ktore sa ma pohnut o n, ak sa neda pohnut o n, vrati -1
unsigned short up(const Node* node, std::string color, unsigned short n) {
	Car car;
	unsigned short index = 0;

	for (size_t i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == color) {
			car = node->cars.at(i);
			index = i;

			break;
		}
	}

	
	if(car.yAxis - n < 0){
		std::cout << "out of bounds" << std::endl;
		return -1;
	}
		
	for (auto& c : node->cars)
	{
		if (c.color != color) {

			//ak sa nad autom uz nachadza nejake ine auto otocene vertikalne
			if (c.dir == 'v' && c.xAxis == car.xAxis && c.yAxis + c.size <= car.yAxis && c.yAxis + c.size > car.yAxis - n) {
				std::cout << "prekaza auto otocene v tom istom smere" << std::endl;
				return -1;
				
			}
			//ak sa nad autom nachadza ine auto otocene horizontalne
			else if (c.dir == 'h' && c.xAxis <= car.xAxis && c.xAxis + c.size > car.xAxis && c.yAxis < car.yAxis && c.yAxis >= car.yAxis - n) {
				std::cout << "prekaza auto otocene v inom smere" << std::endl;
				return -1;
			}
		}
	}
	


	std::cout << "auto sa uspesne pohlo" << std::endl;

	return index;
}

unsigned short down(const Node* node, std::string color, unsigned short n) {
	Car car;
	unsigned short index = 0;

	for (size_t i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == color) {
			car = node->cars.at(i);
			index = i;

			break;
		}
	}

	
	if (car.yAxis + car.size + n - 1 > BOUNDARIES ) {
		std::cout << "out of bounds" << std::endl;
		return -1;
	}

	for (auto& c : node->cars)
	{
		if (c.color != color) {

			//ak sa pod autom uz nachadza nejake ine auto otocene vertikalne
			if (c.dir == 'v' && c.xAxis == car.xAxis && c.yAxis >= car.yAxis + car.size && c.yAxis < car.yAxis + car.size + n) {
				std::cout << "prekaza auto otocene v tom istom smere" << std::endl;
				return -1;
			}
			//ak sa pod autom nachadza ine auto otocene horizontalne
			else if (c.dir == 'h' && c.xAxis <= car.xAxis && c.xAxis + c.size > car.xAxis && c.yAxis > car.yAxis && c.yAxis < car.yAxis + car.size + n) {
				std::cout << "prekaza auto otocene v inom smere" << std::endl;
				return -1;
			}
		}
	}
	


	std::cout << "auto sa uspesne pohlo" << std::endl;

	return index;
}

unsigned short left(const Node* node, std::string color, unsigned short n) {
	Car car;
	unsigned short index = 0;

	for (size_t i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == color) {
			car = node->cars.at(i);
			index = i;

			break;
		}
	}

	
	if (car.xAxis - n < 0) {
		std::cout << "out of bounds" << std::endl;
		return -1;
	}

	for (auto& c : node->cars)
	{
		if (c.color != color) {

			//ak sa nad autom uz nachadza nejake ine auto otocene vertikalne
			if (c.dir == 'h' && c.yAxis == car.yAxis && c.xAxis + c.size <= car.xAxis && c.xAxis + c.size > car.xAxis - n) {
				std::cout << "prekaza auto otocene v tom istom smere" << std::endl;
				return -1;
			}
			//ak sa nad autom nachadza ine auto otocene horizontalne
			else if (c.dir == 'v' && c.yAxis <= car.yAxis && c.yAxis + c.size > car.yAxis && c.xAxis < car.xAxis && c.xAxis >= car.xAxis - n) {
				std::cout << "prekaza auto otocene v inom smere" << std::endl;
				return -1;
			}
		}
	}
	


	std::cout << "auto sa uspesne pohlo" << std::endl;

	return index;
}

unsigned short right(const Node* node, std::string color, unsigned short n) {
	Car car;
	unsigned short index = 0;

	for (size_t i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == color) {
			car = node->cars.at(i);
			index = i;

			break;
		}
	}

	
	if (car.xAxis + car.size + n - 1 > BOUNDARIES) {
		std::cout << "out of bounds" << std::endl;
		return -1;
	}

	for (auto& c : node->cars)
	{
		if (c.color != color) {

			//ak sa pod autom uz nachadza nejake ine auto otocene vertikalne
			if (c.dir == 'h' && c.yAxis == car.yAxis && c.xAxis >= car.xAxis + car.size && c.xAxis < car.xAxis + car.size + n) {
				std::cout << "prekaza auto otocene v tom istom smere" << std::endl;
				return -1;
			}
			//ak sa pod autom nachadza ine auto otocene horizontalne
			else if (c.dir == 'v' && c.yAxis <= car.yAxis && c.yAxis + c.size > car.yAxis && c.xAxis > car.xAxis && c.xAxis < car.xAxis + car.size + n) {
				std::cout << "prekaza auto otocene v inom smere" << std::endl;
				return -1;
			}
		}
	}
	

	std::cout << "auto sa uspesne pohlo" << std::endl;

	return index;
}


//realny pohyb, vrati novy uzol, kde je uz posunute auto
Node* moveUp(Node* node, unsigned short index, unsigned short n) {
	node->cars.at(index).yAxis -= n;

	Node* nNode = new Node;

	nNode->cars = node->cars;

	return nNode;
}

Node* moveDown(Node* node, unsigned short index, unsigned short n) {
	node->cars.at(index).yAxis += n;

	Node* nNode = new Node;

	nNode->cars = node->cars;

	return nNode;
}

Node* moveLeft(Node* node, unsigned short index, unsigned short n) {
	node->cars.at(index).xAxis -= n;
	Node* nNode = new Node;

	nNode->cars = node->cars;

	return nNode;
}

Node* moveRight(Node* node, unsigned short index, unsigned short n) {
	node->cars.at(index).xAxis += n;

	Node* nNode = new Node;

	nNode->cars = node->cars;

	return nNode;
}


/*Hlavolam je vyrieöen˝, keÔ je ËervenÈ auto (v smere jeho jazdy) na okraji kriûovatky a mÙûe sa z nej dostaù von.
Predpokladajte, ûe ËervenÈ auto je vûdy otoËenÈ horizont·lne a smeruje doprava.*/
 unsigned short checkForFinal(Node* node) {
	unsigned short redX = 0;
	unsigned short redY = 0;
	unsigned short index = 0;

	for (size_t i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == "cervene") {
			redX = node->cars.at(i).xAxis;
			redY = node->cars.at(i).yAxis;

			index = i;

			break;
		}
	}

	for (auto& c : node->cars)
	{
		//ak je ine auto v tom istom riadku
		if (c.dir == 'h' && c.yAxis == redY && c.xAxis > redX) {
			std::cout << "cervene sa nidky nedostane z parkoviska" << std::endl;
			return 0;
		}

		//ak ine auto zasahuje do riadku
		if (c.dir == 'v' && c.xAxis > redX && c.yAxis + c.size > redY && c.yAxis <= redY) {
			std::cout << "cervene sa nemoze dostat z parkoviska" << std::endl;
			return 0;
		}
	}

	std::cout << "cervene auto moze vyjst z parkoviska" << std::endl;


	return 1;
}

Node *moveRedToFinal(Node* node, const unsigned short& i) {
	Node* newNode = new Node;
	newNode->cars = node->cars;
	
	newNode->cars.at(i).xAxis = 4;

	return newNode;
}




//obrati linked list, ktory je od final node k root tak, aby bol od root k final node;
Node* reverse(Node* root)
{
	// Initialize current, previous and next pointers
	Node* current = root;
	Node* prev = NULL;
	Node* next = NULL;

	while (current != NULL) {
		// Store next
		next = current->pNode;

		// Reverse current node's pointer
		current->pNode = prev;

		// Move pointers one position ahead.
		prev = current;
		current = next;
	}
	return prev;
}

//funckia na vyvorenie pociatocneho stavu;

/*
zabezpecenie vsetkych moznych pohybov, pre kazde auto vyskusa kazdy pohyb o kazdy mozny pocet policok
takzvane to zrobi to co chce uloha a vrati to root s cestou k final nodu, alebo to vrati samotny root, ked nebude existovat riesenie
*/
Node* searchAlgorithm(Node* root) {

	//ak sa da s cervenym autom vyjst z parkoviska v pociatocnom stave;
	unsigned short index = checkForFinal(root);

	if (index != -1) {
		root->pNode = moveRedToFinal(root, index);

		visited.insert(root);
		visited.insert(root->pNode);

		return root;
	}


	//pracuje s "najdenymi nodes"
	while (!nodesToProcess.empty()) {
		//z datastructure, ktora sa aktualne pouziva vyberie node, ktory ma nasledovat a s nim pracuje 
		Node* node = pop(nodesToProcess);

		//pre kazde auto
		for (auto& car : node->cars) 
		{
			//kazdy pohyb
			if (car.dir == 'h') {
				//kazdy pohyb od 1 do 4
				for (size_t n = 1; n <= 4; n++)
				{
					unsigned short index = left(node, car.color, n);

					if (index ==  -1)
					{
						break;
					}

					Node *newNode = new Node;
					newNode->cars = node->cars;

					newNode = moveLeft(newNode, index, n);

					newNode->pNode = node;

					nodesToProcess.push_back(newNode);
				}

				for (size_t n = 1; n <= 4; n++)
				{
					unsigned short index = right(node, car.color, n);

					if (index == -1)
					{
						break;
					}

					Node* newNode = new Node;
					newNode->cars = node->cars;

					newNode = moveRight(newNode, index, n);

					newNode->pNode = node;

					nodesToProcess.push_back(newNode);
				}
			}
			else {
				for (size_t n = 1; n <= 4; n++)
				{
					unsigned short index = up(node, car.color, n);

					if (index == -1)
					{
						break;
					}

					Node* newNode = new Node;
					newNode->cars = node->cars;

					newNode = moveUp(newNode, index, n);

					newNode->pNode = node;

					nodesToProcess.push_back(newNode);
				}

				for (size_t n = 1; n <= 4; n++)
				{
					unsigned short index = down(node, car.color, n);

					if (index == -1)
					{
						break;
					}

					Node* newNode = new Node;
					newNode->cars = node->cars;

					newNode = moveDown(newNode, index, n);

					newNode->pNode = node;

					nodesToProcess.push_back(newNode);
				}
			}
		}


		visited.insert(node);
	}


	/*
	dokoncit: kontrolu, ci uz sa nenachadza vo visited
	kontrolu, ci sa neda dostat do final nodu, umiestnit na vhodne miesto
	a teoreticky by to malo fahcat
	*/



	Node* finalNode = nullptr;


	return reverse(finalNode);
}



int main(int argc, char* argv[]) {
	std::cout << "zadat farbu, x polohu, y polohu, velkost, smer pohybu" << std::endl;

	/*Car car = carValidation("cervene", 1, 1, 2, 'h');
	Car car1 = carValidation("zelene", 4, 3, 3, 'v');
	Car car2 = carValidation("modre", 0, 0, 2, 'v');

	//Car car1 = carValidation("modre", 2, 1, 2, 'v');

	std::cout << car.color << std::endl;

	Node *root = new Node;

	root->cars.push_back(car);
	root->cars.push_back(car1);
	root->cars.push_back(car2);
	

	std::cout << checkForFinal(*root) << std::endl;

	root->pNode = up(*root, "zelene", 1);

	Node *temp = root;

    temp->pNode = up(*temp, "zelene", 1);

	temp = temp->pNode;

    temp->pNode = up(*temp, "zelene", 1);


	temp = root;

	while (temp != nullptr) {
		std::cout << temp->cars.at(1).yAxis << std::endl;

		temp = temp->pNode;
	}

	std::cout << "konec pico" << std::endl;*/

	/*Car car = Car("cervene", 1, 1, 2, 'h');
	Car car1 = Car("cervene", 1, 1, 2, 'h');


	Node node = Node();
	Node node1 = Node();

	node.cars.push_back(car);
	node1.cars.push_back(car1);


	if (node == node1) {
		std::cout <<  "rovnaju sa" << std::endl;

	}*/


	if (argc == 1) {
		std::cout << "v CLI zadajte typ prehladavacieho algoritmu (DFS / BFS)" << std::endl;

		return 1;
	}


	if (argv[1] == "DFS") {
		pop = popDfs;
	}
	else if(argv[1] == "BFS")
	{
		pop = popBfs;
	}



	Node* root = loadCars();

	//nodesToProcess.push_back(root);


	std::unordered_set<Node*>::const_iterator got = visited.find(node1);

	if (got == visited.end()) {
		std::cout << "neexistuje" << std::endl;
	}
	else {
		std::cout << "uz existuje" << std::endl;
	}

	return 0;

	//ak sa nenajde riesenie, tak sa len vypise pociatocny stav a sprava
	if (root->pNode == nullptr) {
		std::cout << root << "nenaslo sa riesenie" << std::endl;

		return 1;
	}

	//vypisanie priebehu od pociatocneho stavu po finalny
	while (root != nullptr) {
		std::cout << root << std::endl;

		root = root->pNode;
	}

	//uvolnenie pamate na konci programu
	for (auto& node : visited)
	{
		delete node;
	}


	return 0;
}