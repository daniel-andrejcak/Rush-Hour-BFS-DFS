/*root node grafu je pociatocna situacia, treba vymysliet program, ktory bude schopny spravit vsetky mozne kroky pre danu situaciu
bude existovat queue, uzol a sa dostane do queue, zacne sa spracovat. spracovat znamena,ze sa vyberie z queue, skontroluje sa ci uz neni koniec a vsetky mozne situacie,
ktore mozu nasledovat, reprezentovane uzlami daju DO QUEUE NA KONIEC a to zabezpeci ze to bude vyhladavanie do sirky
ak sa namiesto queue pouzije stack, teda ze vsetky mozne situacie, ktore mozu nastat sa budu davat DO STACKU NA ZACIATOK, tak to bude prehliadavanie do hlbky*/

#include<iostream>
#include<map>
#include<sstream>
#include<string>
#include<unordered_set>
#include<vector>

#include"structures.h"

//custom "return", ktory vypise do stdout ako pouzivat CLI args pre tento program
#define RETURN(x) { \
    std::cout << "v CLI vyberte scenar (1 - 6), zadajte typ prehladavacieho algoritmu (DFS / BFS) a zadajte maximalny pozadovany pocet krokov (volitelne)" << std::endl; \
    return (x); \
}


//max rozmer hracej plochy - suradnice zacinaju 0,0
const unsigned short BOUNDARIES = 5;

//default MAXDEPTH moze byt zmeneny cez 3. argument CLI
unsigned short MAXDEPTH = 100;



//unordered_set pre visited nodes, lebo vyuziva hash table na hladanie prvkov - rychlejsie ako prehladavanie celeho vectora
std::unordered_set<Node*, CustomNodeHash, CustomNodeEqual> visited;

//tento vektor sa podla zvoleneho search algoritmu bude spravat ako stack, alebo queue
std::vector<Node*> nodesToProcess;

//pomocne funckie pre vypis
std::ostream& operator<<(std::ostream& os, const Node* node) {
	for (auto& car : node->cars) {
		os << colorToString(car.color) << " x: " << car.xAxis << " y: " << car.yAxis << " size: " << car.size << " direction: " << car.dir << std::endl;
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const char dir) {
	if (dir == 'u') 
	{
		os << "HORE";
	}
	else if(dir == 'd')
	{
		os << "DOLE";
	}
	else if (dir == 'l')
	{
		os << "VLAVO";
	}
	else if (dir == 'r')
	{
		os << "VPRAVO";
	}

	return os;
}


//funkcia vrati a odstrani prvy/posledny prvok vectora - zalezi od typu search algoritmu
//pointer na funckiu sa v main nastavi na korektnu implementaciu pop, podla pouziteho search algritmu
Node* (*pop)(std::vector<Node*>&);

//vec.back() funguje ako stack.top()
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
Car carValidation(const Color& color, const unsigned short xAxis, const unsigned short yAxis, const unsigned short size, const char move) {
	if (color == Color::CERVENE && move != 'h') {
		std::cout << "red car must be horizontal" << std::endl;

		exit(0);
	}

	if (size != 2 && size != 3) {
		std::cout << "wrong size of car" << std::endl;

		exit(0);
	}

	if ((move == 'h' && xAxis + size - 1 > BOUNDARIES) || (move == 'v' && yAxis + size - 1 > BOUNDARIES) || xAxis > BOUNDARIES || yAxis > BOUNDARIES) {
		std::cout << "out of bounds" << std::endl;

		exit(0);
	}

	return Car(color, xAxis, yAxis, size, move);
}


//funckia na vyvorenie pociatocneho stavu;
Node* loadCars(const std::string& inputString) {
	Node* root = new Node;

	//docasna zmena std::cin aby citalo zo stringu
	std::istringstream iss(inputString);
	std::cin.rdbuf(iss.rdbuf());

	std::string color;
	unsigned short xAxis;
	unsigned short yAxis;
	unsigned short size;
	char move;

	while (std::cin >> color >> xAxis >> yAxis >> size >> move) {
		Car car = carValidation(stringToColor(color), xAxis, yAxis, size, move);

		root->cars.push_back(car);

	}

	return root;
}





//kontrola ci je pohyb platny, vrati index auta, ktore sa ma pohnut o n, ak sa neda pohnut o n, vrati -1
unsigned short up(const Node* node, const Color& color, unsigned short n) {
	Car car;
	unsigned short index = 0;

	for (unsigned short i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == color) {
			car = node->cars.at(i);
			index = i;

			break;
		}
	}

	
	if(car.yAxis - n < 0){
		//std::cout << "out of bounds" << std::endl;
		return -1;
	}
		
	for (auto& c : node->cars)
	{
		if (c.color != color) {

			//ak sa nad autom uz nachadza nejake ine auto otocene vertikalne
			if (c.dir == 'v' && c.xAxis == car.xAxis && c.yAxis + c.size <= car.yAxis && c.yAxis + c.size > car.yAxis - n) {
				//std::cout << "prekaza auto otocene v tom istom smere" << std::endl;
				return -1;
				
			}
			//ak sa nad autom nachadza ine auto otocene horizontalne
			else if (c.dir == 'h' && c.xAxis <= car.xAxis && c.xAxis + c.size > car.xAxis && c.yAxis < car.yAxis && c.yAxis >= car.yAxis - n) {
				//std::cout << "prekaza auto otocene v inom smere" << std::endl;
				return -1;
			}
		}
	}
	


	//std::cout << "auto sa uspesne pohlo" << std::endl;

	return index;
}

unsigned short down(const Node* node, const Color& color, unsigned short n) {
	Car car;
	unsigned short index = 0;

	for (unsigned short i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == color) {
			car = node->cars.at(i);
			index = i;

			break;
		}
	}

	
	if (car.yAxis + car.size + n - 1 > BOUNDARIES ) {
		//std::cout << "out of bounds" << std::endl;
		return -1;
	}

	for (auto& c : node->cars)
	{
		if (c.color != color) {

			//ak sa pod autom uz nachadza nejake ine auto otocene vertikalne
			if (c.dir == 'v' && c.xAxis == car.xAxis && c.yAxis >= car.yAxis + car.size && c.yAxis < car.yAxis + car.size + n) {
				//std::cout << "prekaza auto otocene v tom istom smere" << std::endl;
				return -1;
			}
			//ak sa pod autom nachadza ine auto otocene horizontalne
			else if (c.dir == 'h' && c.xAxis <= car.xAxis && c.xAxis + c.size > car.xAxis && c.yAxis > car.yAxis && c.yAxis < car.yAxis + car.size + n) {
				//std::cout << "prekaza auto otocene v inom smere" << std::endl;
				return -1;
			}
		}
	}
	


	//std::cout << "auto sa uspesne pohlo" << std::endl;

	return index;
}

unsigned short left(const Node* node, const Color& color, unsigned short n) {
	Car car;
	unsigned short index = 0;

	for (unsigned short i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == color) {
			car = node->cars.at(i);
			index = i;

			break;
		}
	}

	
	if (car.xAxis - n < 0) {
		//std::cout << "out of bounds" << std::endl;
		return -1;
	}

	for (auto& c : node->cars)
	{
		if (c.color != color) {

			//ak sa nad autom uz nachadza nejake ine auto otocene vertikalne
			if (c.dir == 'h' && c.yAxis == car.yAxis && c.xAxis + c.size <= car.xAxis && c.xAxis + c.size > car.xAxis - n) {
				//std::cout << "prekaza auto otocene v tom istom smere" << std::endl;
				return -1;
			}
			//ak sa nad autom nachadza ine auto otocene horizontalne
			else if (c.dir == 'v' && c.yAxis <= car.yAxis && c.yAxis + c.size > car.yAxis && c.xAxis < car.xAxis && c.xAxis >= car.xAxis - n) {
				//std::cout << "prekaza auto otocene v inom smere" << std::endl;
				return -1;
			}
		}
	}
	


	//std::cout << "auto sa uspesne pohlo" << std::endl;

	return index;
}

unsigned short right(const Node* node, const Color& color, unsigned short n) {
	Car car;
	unsigned short index = 0;

	for (unsigned short i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == color) {
			car = node->cars.at(i);
			index = i;

			break;
		}
	}

	
	if (car.xAxis + car.size + n - 1 > BOUNDARIES) {
		//std::cout << "out of bounds" << std::endl;
		return -1;
	}

	for (auto& c : node->cars)
	{
		if (c.color != color) {

			//ak sa pod autom uz nachadza nejake ine auto otocene vertikalne
			if (c.dir == 'h' && c.yAxis == car.yAxis && c.xAxis >= car.xAxis + car.size && c.xAxis < car.xAxis + car.size + n) {
				//std::cout << "prekaza auto otocene v tom istom smere" << std::endl;
				return -1;
			}
			//ak sa pod autom nachadza ine auto otocene horizontalne
			else if (c.dir == 'v' && c.yAxis <= car.yAxis && c.yAxis + c.size > car.yAxis && c.xAxis > car.xAxis && c.xAxis < car.xAxis + car.size + n) {
				//std::cout << "prekaza auto otocene v inom smere" << std::endl;
				return -1;
			}
		}
	}
	

	//std::cout << "auto sa uspesne pohlo" << std::endl;

	return index;
}


//realny pohyb, vrati novy uzol, kde je uz posunute auto
Node* moveUp(Node* node, unsigned short index, unsigned short n) {
	node->cars.at(index).yAxis -= n;

	Node* nNode = new Node;

	nNode->cars = node->cars;

	nNode->dir = 'u';

	return nNode;
}

Node* moveDown(Node* node, unsigned short index, unsigned short n) {
	node->cars.at(index).yAxis += n;

	Node* nNode = new Node;

	nNode->cars = node->cars;

	nNode->dir = 'd';

	return nNode;
}

Node* moveLeft(Node* node, unsigned short index, unsigned short n) {
	node->cars.at(index).xAxis -= n;
	Node* nNode = new Node;

	nNode->cars = node->cars;

	nNode->dir = 'l';

	return nNode;
}

Node* moveRight(Node* node, unsigned short index, unsigned short n) {
	node->cars.at(index).xAxis += n;

	Node* nNode = new Node;

	nNode->cars = node->cars;

	nNode->dir = 'r';

	return nNode;
}


/*Hlavolam je vyrie�en�, ke� je �erven� auto (v smere jeho jazdy) na okraji kri�ovatky a m��e sa z nej dosta� von.
Predpokladajte, �e �erven� auto je v�dy oto�en� horizont�lne a smeruje doprava.*/
 unsigned short checkForFinal(const Node* node) {
	unsigned short redX = 0;
	unsigned short redY = 0;
	unsigned short index = 0;

	for (unsigned short i = 0; i < node->cars.size(); i++)
	{
		if (node->cars.at(i).color == Color::CERVENE) {
			redX = node->cars.at(i).xAxis;
			redY = node->cars.at(i).yAxis;

			index = i;

			break;
		}
	}

	for (auto& c : node->cars)
	{

		//ak ine auto zasahuje do riadku
		if (c.dir == 'v' && c.xAxis > redX && c.yAxis + c.size > redY && c.yAxis <= redY) {
			//std::cout << "cervene sa nemoze dostat z parkoviska" << std::endl;
			return -1;
		}
		
		//ak je ine auto v tom istom riadku, co znamena ze cervene sa nikdy nedostane z parkoviska
		if (c.dir == 'h' && c.yAxis == redY && c.xAxis > redX) {
			return -2;
		}
	}

	//std::cout << "cervene auto moze vyjst z parkoviska" << std::endl;


	return index;
}

 //vrati novy node, kde bude cervene auto na finalnej pozicii
Node *moveRedToFinal(const Node* node, const unsigned short& redIndex) {
	Node* newNode = new Node;
	newNode->cars = node->cars;

	newNode->color = Color::CERVENE;
	newNode->dir = 'r';
	newNode->n = 4 - newNode->cars.at(redIndex).xAxis;
	
	newNode->cars.at(redIndex).xAxis = 4;

	return newNode;
}



//obrati cestu od final node k root tak, aby bola od root k final node;
Node* reversePath(Node* root)
{
	Node* current = root;
	Node* prev = nullptr;
	Node* next = nullptr;

	while (current != nullptr)
	{
		next = current->pNode;

		current->pNode = prev;

		prev = current;
		current = next;
	}

	return prev;
}


/*
pre dane auto - car, v danom stave - node, sa pokusi vykonat pozadovany pohyb o 1 - 4 policka a novo vygenerovane stavy vlozi do stack alebo queue
*/
Node* move(Node* node, const Car& car, unsigned short (*dirValidFunc)(const Node*, const Color&, unsigned short), Node* (*dir)(Node*, unsigned short, unsigned short)) {
	for (unsigned short n = 1; n <= 4; n++)
	{
		unsigned short index = (*dirValidFunc)(node, car.color, n);

		//ak vrati index 0xffff tak to znamena ze auto sa uz nemohlo posunut dalej, tym padom nema zmysel skusat pohyb o viac policok
		if (index == 0xffff)
		{
			return nullptr;
		}

		Node* newNode = new Node;
		newNode->cars = node->cars;

		newNode = (*dir)(newNode, index, n);


		newNode->color = car.color;
		newNode->n = n;

		//ak sa uz taky stav nachadza v grafe
		if (visited.find(newNode) != visited.end())
		{
			delete newNode;

			continue;
		}

		newNode->pNode = node;
		newNode->depth = node->depth + 1;


		/*ak sa v novo vygenerovanom stave moze cervene auto dostat z parkoviska, tak vytvori stav v ktorom posunie cervene auto na finalnu poziciu,
		pre tento stav nastavi pointer na newNode a ukonci sa vyhladavanie grafu
		pre pohyby aut, ktore su otocene horizontalne to netreba kontrolovat, pretoze ziaden horizontalny pohyb auta neovplyvni trasu pre cervene auto,
		iba ze by bolo v tom istom riadku ako je cervene auto, co je ale skontrolovane pred zaciatkom search algoritmu*/
		if (dirValidFunc == &up || dirValidFunc == &down)
		{
			unsigned short redIndex = checkForFinal(newNode);

			if (redIndex != 0xffff)
			{
				Node *finalNode = moveRedToFinal(newNode, redIndex);

				finalNode->pNode = newNode;

				return finalNode;
			}
		}

		//ak este nemozno hru ukoncit, tak ho prida do stack/queue a dalej tvori pohyby
		nodesToProcess.push_back(newNode);
	}

	return nullptr;
}


Node* searchAlgorithm(Node* root) {

	Node* node = nullptr;

	Node* finalNode = root;

	nodesToProcess = { root };

	//ak sa da s cervenym autom vyjst z parkoviska v pociatocnom stave;
	unsigned short redIndex = checkForFinal(root);

	//kontrola ci cervene auto neni blokovane horizontalnym autom
	if (redIndex == 0xfffe)
	{
		std::cout << "cervene auto sa nikdy nedostane z parkoviska" << std::endl;

		delete root;

		exit(1);
	}

	if (redIndex != 0xffff) {
		root->pNode = moveRedToFinal(root, redIndex);

		visited.insert(root);
		visited.insert(root->pNode);

		return root;
	}


	//pracuje s "najdenymi nodes"
	while (!nodesToProcess.empty()) {
		//z datastructure, ktora sa aktualne pouziva vyberie stav, ktory ma nasledovat a s nim pracuje 
		node = pop(nodesToProcess);

		if (node->depth >= MAXDEPTH)
		{
			delete node;

			continue;
		}

		//pre kazde auto vykona kazdy mozny pohyb o 1 - 4 policka a novo vygenerovane stavy vlozi do stack / queue
		for (auto& car : node->cars)
		{
			//kazdy pohyb
			if (car.dir == 'v')
			{
				finalNode = move(node, car, &up, &moveUp);
				if (finalNode) { break; }

				finalNode = move(node, car, &down, &moveDown);
				if (finalNode) { break; }
			}
			else
			{
				move(node, car, &left, &moveLeft);

				move(node, car, &right, &moveRight);
			} 
		}

		//ak uz z daneho stavu vygeneroval vsetky mozne nasledujuce stavy, tak dany stav vlozi do visited
		visited.insert(node);

		if (finalNode)
		{
			//ulozi final node do visited, len kvoli tomu, aby sa pri ukonceni programu mohla vymazat
			visited.insert(finalNode);

			return reversePath(finalNode);
		}
	}


	return root;
}


int main(int argc, char* argv[])
{
	//spracovanie argumentov zadanych v CLI....zvolenie scenara, nastavenie DFS / BFS, urcenie maximalneho poctu tahov
	if (argc == 1)
	{
		RETURN(1);
	}


	//vyber scenara
	char s = argv[1][0];
	std::string scenar;

	switch (s)
	{
	case '1':
		//scenar rovnaky ako v zadani
		scenar = "oranzove 0 0 2 h zlte 0 1 3 v ruzove 0 4 2 v cervene 1 2 2 h zelene 3 1 3 v modre 5 0 3 v sive 4 4 2 h svetlomodre 2 5 3 h";
		break;
	case '2':
		//scenar, ze cervene auto je blokovane horizontalnym autom
		scenar = "oranzove 0 0 2 h zlte 0 1 3 v ruzove 0 4 2 v cervene 1 2 2 h zelene 4 2 2 h modre 3 0 3 v sive 4 4 2 h svetlomodre 2 5 3 h";
		break;
	case '3':

		break;
	case '4':
		break;
	case '5':
		break;
	case '6':
		break;

	default:
		RETURN(1);
	}



	//vyber vyhladavacieho algoritmu
	std::string stringArg(argv[2]);

	if (stringArg == "DFS")
	{
		pop = popDfs;
	}
	else if(stringArg == "BFS")
	{
		pop = popBfs;
	}
	else 
	{
		RETURN(1);
	}

	//urcenie maximalneho poctu tahov
	if (argc >= 4) { MAXDEPTH = atoi(argv[3]); }




	//inicializacia pociatocneho stavu
	Node* root = loadCars(scenar);

	//spustenie vyhladavacieho algoritmu
	root = searchAlgorithm(root);




	//ak sa nenajde riesenie, tak sa len vypise pociatocny stav a sprava
	if (root->pNode == nullptr)
	{
		std::cout << root << std::endl << "nenaslo sa riesenie na pozadovany pocet tahov" << std::endl;

		return 1;
	}

	//vypisanie priebehu od pociatocneho stavu po finalny
	std::cout << root << std::endl;

	while (root->pNode != nullptr) 
	{
		root = root->pNode;

		std::cout << colorToString(root->color) << " " << root->dir << " " << root->n << " " << std::endl;
	}

	std::cout << std::endl << root << std::endl;



	//uvolnenie pamate na konci programu
	for (auto& node : visited)
	{
		delete node;
	}

	for (auto& node : nodesToProcess)
	{
		delete node;
	}

	return 0;
}