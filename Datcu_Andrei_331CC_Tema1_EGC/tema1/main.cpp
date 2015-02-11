#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include "Objects/GameFieldVisual2D.h"
#include "Objects/StatusVisual2D.h"

#include <iostream>
#include <windows.h>
#include <time.h>


GameFieldVisual2D *gameField;
StatusVisual2D *status;
Visual2D *endGameVisual;
bool stillAlive = true;

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	srand(time(NULL));
	status = new StatusVisual2D(0, 0, DrawingWindow::width, 
		DrawingWindow::height, 0, 0, DrawingWindow::width, 
		DrawingWindow::height / 6);

	gameField = new GameFieldVisual2D(0, 0,DrawingWindow::width,
		DrawingWindow::height, 0, DrawingWindow::height / 6,
		DrawingWindow::width, DrawingWindow::height);

	endGameVisual = new Visual2D(0, 0, DrawingWindow::width,
		DrawingWindow::height, 0, DrawingWindow::height / 6,
		DrawingWindow::width, DrawingWindow::height);

	endGameVisual->addText(new Text("GAME OVER",
		Point2D(DrawingWindow::width / 2, DrawingWindow::height / 2), 
		Color(0, 1, 0),  BITMAP_TIMES_ROMAN_24));

	addVisual2D(gameField);
	addVisual2D(status);
}


//functia care permite animatia
void DrawingWindow::onIdle()
{
	if (gameField->getLivesLeft() < 0 && stillAlive){
		stillAlive = false;
		removeVisual2D(gameField);
		addVisual2D(endGameVisual);
	}

	if (stillAlive){
		gameField->animate();
		status->setScore(gameField->getScore());
		status->setLives(gameField->getLivesLeft());
		status->setLevel(gameField->getCurrentLevel());
		status->setEnemiesLeft(gameField->getEnemiesLeft());
	}
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height){
	
	gameField->poarta(0,height / 6 ,width,height); 
	endGameVisual->poarta(0,height / 6 ,width,height); 
	status->poarta(0, 0, width, height/6);
}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key){
	if (!stillAlive)
		return;

	switch(key){
		case 101 : 
			gameField->moveShipUp();
			break;

		case 100:
			gameField->rotateShipLeft();
			break;

		case 102:
			gameField->rotateShipRight();
			break;

		case 32:
			gameField->changeGunStatus();
	}
}

void DrawingWindow::onKeyUp(unsigned char key){

		if (!stillAlive)
			return;

		switch(key){
		case 101 : 
		case 100:
		case 102:
			gameField->resetShipSpeed();
			break;
		}
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
	
}


int main(int argc, char** argv)
{
	//creare fereastra
	DrawingWindow dw(argc, argv, 1280, 720, 200, 100, "Laborator EGC");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, 
	//animatia si procesarea evenimentelor
	dw.run();

	//cleaning up
	delete(gameField);
	delete(status);
	delete(endGameVisual);
	return 0;

}