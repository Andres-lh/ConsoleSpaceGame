#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>

using namespace std;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77


bool gameOver;


void position(int x, int y)
{
	HANDLE hCon;

	// Recupera el identificador de la consola
	hCon = GetStdHandle(STD_OUTPUT_HANDLE); // Controla la salida de la consola

	// Se crea objeto de la estructura COORD definida wn Windows.h
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}

void hideCursor() 
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CCI;
	CCI.dwSize = 10;
	CCI.bVisible = FALSE;
	SetConsoleCursorInfo(hCon, &CCI);
}

void limits()
{
	for (int i = 2; i < 119; i++) 
	{
		position(i, 1); printf("%c", 205);
		position(i, 28); printf("%c", 205);
	}

	for (int i = 2; i < 28; i++)
	{
		position(1, i); printf("%c", 186);
		position(118, i); printf("%c", 186);
	}
	 


	position(1, 1); printf("%c", 201);
	position(1, 28); printf("%c", 200);
	position(118, 1);printf("%c", 187);
	position(118, 28);printf("%c", 188);

}

class Ship
{

private:

	int x, y, h;
	int nLives;

public:
	Ship(int _x, int _y, int _h, int _nLives) :x(_x), y(_y), h(_h), nLives(_nLives) {};
	int X() {
		return x;
	}
	int Y() {
		return y;
	}
	int lives() { return nLives; }
	void draw();
	void erase();
	void move();
	void healthBar();
	void die();
	void decreaseH() {
		h--;
	}
	

};

void Ship::draw() 
{
	position(x, y); printf("  %c", 30);
	position(x, y + 1); printf(" %c%c%c", 40, 207, 41);
	position(x, y + 2); printf("%c%c %c%c", 30, 190, 190, 30);
}

void Ship::erase()
{
	position(x, y); printf("       ");
	position(x, y + 1); printf("       ");
	position(x, y + 2); printf("       ");
}

void Ship::move() 
{
	
	if (_kbhit()) //Evalua si se presiona una tecla
	{
		//Recupera el valor de la tecla que se presiona
		char key = _getch();

		erase();

		if (key == LEFT && x > 2)
			x--;
		if (key == RIGHT && x + 6 < 119)
			x++;
		if (key == UP && y > 3)
			y--;
		if (key == DOWN && y + 3 < 28)
			y++;
		if (key == 'q')
			h--;
		
		draw();
		healthBar();
	}
	
}

void Ship::healthBar() 
{	
	position(90, 2); printf("Lives: %d", nLives);
	position(106, 2); printf("Health:");
	position(114, 2); printf("    ");
	for (int i = 0; i < h; i++)
	{
		position(114 + i, 2); printf("%c ", 3);
	}
}

void Ship::die()
{
	if (h == 0) 
	{
		erase();
		position(x, y);     printf("   *   ");
		position(x, y + 1); printf("  ***  ");
		position(x, y + 2); printf(" ***** ");
		Sleep(200);
		erase();

		position(x, y);     printf("* ** *");
		position(x, y + 1); printf(" **** ");
		position(x, y + 2); printf("* ** *");
		Sleep(200);
		erase();

		position(x, y);     printf("*     *");
		position(x, y + 1); printf("  * *  ");
		position(x, y + 2); printf("*     *");
		Sleep(200);
		erase();

		nLives--; 
		h = 3;
		healthBar();
		draw();
	}
}

class Asteroid
{
private:
	int x, y;

public:
	Asteroid(int _x, int _y) : x(_x), y(_y) {};
	int X() { return x; }
	int Y() { return y; }
	void draw();
	void move();
	void crash(Ship &n) {

		if (x >= n.X() && x <= n.X()+ 6 && y >= n.Y() && y <= n.Y() + 2)
		{
			n.decreaseH();
			n.erase();
			n.draw();
			n.healthBar();
			x = rand() % 116 + 2;
			y = 3;
		}
	}

};

void Asteroid::draw() 
{
	position(x, y); printf("O");
}

void Asteroid::move()
{
	position(x, y); printf(" ");
	y++;

if (y > 27) {

	x = rand() % 116 + 2;
	y = 3;

}


draw();
}

class Bullet
{
private:
	int x, y;
public:
	Bullet(int _x, int _y) : x(_x), y(_y) {};

	int X() { return x; }
	int Y() { return y; }

	void move() {
		position(x, y); printf(" ");
		y--;
		position(x, y); printf("*");

	}


	bool limit() {
		//return (y == 2) ? true : false;
		if (y == 2)return true;
		return false;
	}

};



int main()
{
	hideCursor();
	limits();
	Ship ship(55, 23, 3, 3);
	ship.draw();
	ship.healthBar();

	list<Asteroid*> ast;
	list<Asteroid*>::iterator iAst;

	for (int i = 0; i < 5; i++)
	{
		ast.push_back(new Asteroid(rand() % 115 + 3, rand() % 5 + 2));
	}



	list<Bullet*> b;
	list<Bullet*>::iterator i;

	gameOver = false;
	int score = 0;
	// Game loop
	while (!gameOver)
	{

		position(4, 2); printf("Score %d", score);

		if (_kbhit())
		{
			char key = _getch();
			if (key == 's') {
				b.push_back(new Bullet(ship.X() + 2, ship.Y() - 1));
			}

		}

		for (i = b.begin(); i != b.end();)
		{
			(*i)->move();
			if ((*i)->limit())
			{
				position((*i)->X(), (*i)->Y());
				printf(" ");
				delete(*i);
				i = b.erase(i);
			}
			else {
				i++;
			}


		}

		for (iAst = ast.begin(); iAst != ast.end(); iAst++)
		{
			(*iAst)->move();
			(*iAst)->crash(ship);
		}

		for (iAst = ast.begin(); iAst != ast.end(); iAst++) 
		{
			for (i = b.begin(); i != b.end();) 
			{
				if ((*iAst)->X() == (*i)->X() && ((*iAst)->Y() + 1 == (*i)->Y() || (*iAst)->Y() == (*i)->Y()))
				{
					position((*i)->X(), (*i)->Y()); printf(" ");
					delete(*i);
					i = b.erase(i);

					ast.push_back(new Asteroid(rand() % 115 + 3, 2));
					position((*iAst)->X(), (*iAst)->Y()); printf(" ");
					delete(*iAst);
					iAst = ast.erase(iAst);

					score++;
				}
				else
				{
					i++;
				}
			}
		}

		if (ship.lives() == 0) {
			gameOver = true;
		}

		ship.move();
		ship.die();
		 

		
		Sleep(30);
		
	}
	
	system("cls");
	printf("\n\n\n\n\n\n\n");
	Sleep(400);
	printf("\t\t\t\tGA");
	Sleep(600);
	printf("ME\n");
	Sleep(700);
	printf("\t\t\t\tOVER\n");
	Sleep(2000);
	printf("\n\n\n\n\n\n\n");

	printf("\n\n\n\n\n\n\n");
	printf("Thank for playing %c", 2);
	printf("\n\n\n\n");
}