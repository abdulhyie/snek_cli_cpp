#include <iostream>
#include <vector>
#include <Windows.h>
#include <time.h>

using namespace std;

//------------Definitions------------------------------------
//To store co-ordinates of elements/assets on canvas
struct coor
{
	int	x, y;
};

//-------------Assets, Constants and other usables------------
//Height & Width of the canvas
#define						width					25
#define						height					25

//Assets					
#define						canvasPlacehoder		' '
#define						snekPlaceholder			'O'
#define						border					'|'
#define						seedPlaceholder			'@'
#define						bodyPlaceholder			'o'
#define						deadSnek				'X'

//Variables
bool						gameOver				= false;
bool						coorX_increase			= false;
bool						coorY_increase			= false;
bool						coorX_decrease			= false;
bool						coorY_decrease			= false;
int							score					= 0;

coor head;					//Snek head
vector<coor> body;			//Snek body co-ordinates container
coor seed;					//Seed locaction

//Canvas container
vector<vector<char>> canvas(height, vector<char> (width, 0));

//--------------Functions/Utilities---------------------
//Displays canvas
void displayCanvas()
{
	cout << "\t\t\t\t\t\t\t" << "SCORE: " << score << endl;
	for (int i = 0; i < height; i++) {
		cout << "\t\t\t\t";
		for (int j = 0; j < width; j++) {
			cout << canvas[i][j] << ' ';
		}
		cout << endl;
	}
}

//Adds length to the snek after eating seed
void addLength()
{
	coor cell;
	cell.x = cell.y = 0;							//initially assign any coordinate
	body.push_back(cell);
	score += (int)body.size() * 10;					//increase the score as the snek gets bigger
}

//Moves the body of the snek
void moveBody(coor headLocation)
{
	coor preLocation;
	for (int i = 0; i < (int) body.size(); i++) {
		preLocation = body[i];
		body[i] = headLocation;
		headLocation = preLocation;
	}
}

//Randomly places a seed on canvas
void generateSeeds()
{
	seed.x = rand() % (height - 2) + 1;
	seed.y = rand() % (width - 2) + 1;
	canvas[seed.x][seed.y] = seedPlaceholder;
}

//keeps moving the head in specified direction
coor changeCoor()
{
	coor nextCoor = head;
	if (coorX_increase) {
		nextCoor.x--;
	}
	if (coorX_decrease) {
		nextCoor.x++;
	}
	if (coorY_increase) {
		nextCoor.y++;
	}
	if (coorY_decrease) {
		nextCoor.y--;
	}
	return nextCoor;
}

//Updates the canvas
void updateCanvas()
{
	//Clear the canvas
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i == 0 || i == height - 1) {
				canvas[i][j] = border;
			}
			else if (j == 0 || j == width - 1) {
				canvas[i][j] = border;
			}
			else {
				canvas[i][j] = canvasPlacehoder;
			}
		}
	}

	canvas[head.x][head.y] = snekPlaceholder;		//places snek head
	canvas[seed.x][seed.y] = seedPlaceholder;		//places seed
	//places body
	for (int i = 0; i < (int)body.size(); i++) {
		canvas[body[i].x][body[i].y] = bodyPlaceholder;
	}
}

//Checks colisions
void checkCollisions()
{
	char h = canvas[head.x][head.y];
	if (h == border || h == bodyPlaceholder) {
		gameOver = true;
	}
}

//decides the direction of movement
void moveHead()
{
	if (GetAsyncKeyState(VK_UP)) {
		//Head moves up
		if (!coorX_decrease) {					//prevents movement backward
			coorX_increase		= true;
			coorX_decrease		= false;
			coorY_increase		= false;
			coorY_decrease		= false;
		}
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		//Head moves down	
		if (!coorX_increase) {
			coorX_increase		= false;
			coorX_decrease		= true;
			coorY_increase		= false;
			coorY_decrease		= false;
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		//Head moves right
		if (!coorY_decrease) {
			coorX_increase		= false;
			coorX_decrease		= false;
			coorY_increase		= true;
			coorY_decrease		= false;
		}
	}
	else if (GetAsyncKeyState(VK_LEFT)) {
		//Head moves left
		if (!coorY_increase) {
			coorX_increase		= false;
			coorX_decrease		= false;
			coorY_increase		= false;
			coorY_decrease		= true;
		}
	}
	coor nextCoor = changeCoor();
	if (nextCoor.x == seed.x && nextCoor.y == seed.y) {			//if snek hits the seed
		generateSeeds();
		addLength();
	}
	coor preHead = head;
	head = nextCoor;
	moveBody(preHead);
	checkCollisions();
}

//Initializes the game assets as the game starts
void initCanvas()
{
	//sets uo the field and borders (canvas)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i == 0 || i == height - 1) {
				canvas[i][j] = border;
			}
			else if (j == 0 || j == width - 1) {
				canvas[i][j] = border;
			}
			else {
				canvas[i][j] = canvasPlacehoder;
			}
		}
	}

	//starting position of snek head
	int x = height / 2;
	int y = width / 2;
	head.x = x;		head.y = y;
	canvas[head.x][head.y] = snekPlaceholder;

	//Place seed randomly
	generateSeeds();
}

int main()
{
	initCanvas();

	while (!gameOver) {
		srand(time(0));
		displayCanvas();
		moveHead();
		updateCanvas();
		system("cls");
	}
	canvas[head.x][head.y] = deadSnek;
	displayCanvas();
	cout << "Snek Died!!!!" << endl;
	system("pause");
}