#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform3D.h"
#include <windows.h>
#include <vector>
#include <bitset>

#include "Objects/Road.h"
#include "Objects/car.h"
#include "Objects/obstacle.h"
#include "Objects/PyramidObstacle.h"
#include "Objects/SphereObstacle.h"
#include "Objects/StatusVisual2D.h"
#include <cstdlib>
#include <ctime>


#define PI 3.14159265358979323846

Visual2D *v2d1, *endGameVisual;
StatusVisual2D *scoreVisual;
Road *sosea;
Car *masina;
std::vector<Obstacle*> obstacles;
int score = 0, livesLeft = 20;

std::bitset<4> keysPressed; //incepand de la sageata stanga -> clockwise

float n=100;

void generateEnemies(){
	int enemiesToGenerate = rand() % sosea->laneCount;
	bool enemyMobile;
	std::bitset<5> lanesUsed;
	int enemyType, enemyLane;

	for (int i = 1; i <= enemiesToGenerate; ++i){
		enemyType = rand() % 2;
		enemyMobile = rand() % 10 == 0;
		do
			enemyLane = rand () % sosea->laneCount;
		while (lanesUsed.test(enemyLane));

		lanesUsed.set(enemyLane);
		switch (enemyType)
		{
		case 0:
			obstacles.push_back(new PyramidObstacle(
				sosea->getBottomLeftForObject(
				enemyLane + 1, 100, 100), 100, enemyMobile));
			break;

		case 1:
			Point3D center =
				sosea->getBottomLeftForObject(enemyLane + 1, 100, 100);
			center.y = 100;
			obstacles.push_back(new SphereObstacle(center, enemyMobile));
			break;
		}
		DrawingWindow::addObject3D_to_Visual2D(obstacles.back(), v2d1);
	}
}

void keyEvents(){
	//functia care trateaza de fapt apasarea tasteleor
	
	if (!keysPressed.test(0) && !keysPressed.test(2))
		masina->straightenUp();

	for (int key = 0; key < 4; ++key)
		if (keysPressed.test(key))
				switch(key){
					case 0:
						masina->rotateLeft();
						break;

					case 1 :
						masina->accelerate();
						score += 1;
						for (Obstacle *o: obstacles)
							o->accelerate();
						break;

					case 2 : 
						masina->rotateRight();
						break;

					case 3:
						for (Obstacle *o: obstacles)
							for (int i = 0; i < 10; ++i)
								o->deccelerate();
						masina->deccelerate();
						break;
				}
}

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	scoreVisual = new StatusVisual2D(0, 0, DrawingWindow::width, 
		DrawingWindow::height, 0, 0, DrawingWindow::width, 
		DrawingWindow::height / 6);

	addVisual2D(scoreVisual);

	v2d1 = new Visual2D(0, 0,DrawingWindow::width,
		DrawingWindow::height, 0, DrawingWindow::height / 6,
		DrawingWindow::width, DrawingWindow::height);
	v2d1->tipTran(true);
	addVisual2D(v2d1);
	sosea = new Road(Point3D(-110, 0, -10), 1500, 88900);
	addObject3D_to_Visual2D(sosea,v2d1);

	masina = new Car(Point3D(100, 40, 140), 150, 100, -110, 1500 - 110);
	addObject3D_to_Visual2D(masina, v2d1);
	generateEnemies();
	scoreVisual->setScore(0);
	scoreVisual->setLives(20);

	endGameVisual = new Visual2D(0, 0, DrawingWindow::width,
		DrawingWindow::height, 0, DrawingWindow::height / 6,
		DrawingWindow::width, DrawingWindow::height);

	DrawingWindow::addText_to_Visual2D(new Text("GAME OVER",
		Point2D(DrawingWindow::width / 2, DrawingWindow::height / 2), 
		Color(0, 1, 0),  BITMAP_TIMES_ROMAN_24), endGameVisual);
}


//functia care permite animatia
void DrawingWindow::onIdle()
{
	//analizam inamicii

	keyEvents();

	if (!masina->move()){
		masina->straightenUp();
		masina->deccelerate();
		for (Obstacle *o : obstacles)
			for (int i = 0; i < 3; ++i)
				o->deccelerate();
	}

	masina->deccelerate();

	if (livesLeft == 0)
		return;

	float maxZ = 0;

	for (auto itr = obstacles.begin(); itr != obstacles.end();)
		if ((*itr)->getTopZ() <= 0){ // obiectul va iesi de pe sosea
			removeObject3D_from_Visual2D(*itr, v2d1);
			itr = obstacles.erase(itr);
		}
		else{
			if ((*itr)->getTopZ() < sosea->height && (*itr)->getTopZ() > maxZ
				&& obstacles.size() < 50)
				maxZ = (*itr)->getTopZ();

			if (masina->intersects(**itr)){
				removeObject3D_from_Visual2D(*itr, v2d1);
				itr = obstacles.erase(itr);
				score -=200;
				--livesLeft;
				if (livesLeft == 0){
					removeVisual2D(v2d1);
					addVisual2D(endGameVisual);
				}

			}
			else{
				(*itr)->moveDown();
				(*itr)->deccelerate();
				++itr;
			}
		}

	if (maxZ < 0.85 * sosea->height)
		generateEnemies();

	scoreVisual->setScore(score);
	scoreVisual->setLives(livesLeft);

	Transform3D::loadIdentityModelMatrix();
	Transform3D::applyTransform(sosea);
	masina->animate();

	for (Obstacle *o : obstacles){
		if (o->mobile)
			o->moveUp();
		o->animate();
	}
	
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{
v2d1->poarta(0,height / 6 ,width,height); 
	scoreVisual->poarta(0, 0, width, height/6);
}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
	if (livesLeft == 0)
		return;

	if (key >= 100 && key <= 103)
			keysPressed.set(key - 100);
}

void DrawingWindow::onKeyUp(unsigned char key){
	if (livesLeft == 0)
		return;

	if (key >= 100 && key <= 103)
		keysPressed.reset(key - 100);
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	//creare fereastra
	DrawingWindow dw(argc, argv, 1280, 720, 200, 50, "TEMA2");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();

	dw.run();
	return 0;

}