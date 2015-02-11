/* Autor: Datcu Andrei Daniel - 331CC
 * GameFieldVisual2D
 * Contextul visual al jocului. Clasa mentine jocul, verifica daca trebuie
 * sa dispara vreun inamic sau nava, modifica scorul, etc
 */

#include "GameFieldVisual2D.h"
#include "../Framework/DrawingWindow.h"
#include "SquareInSquareEnemy.h"
#include "TwoSquaresEnemy.h"
#include "CircularSawEnemy.h"
#include "ArrowEnemy.h"
#include "LifeBonus.h"
#include "ShieldBonus.h"

#include "../Framework//Common.h"
#include <ctime>


GameFieldVisual2D::GameFieldVisual2D (float xf1,float yf1,float xf2,float yf2,
		 int xp1, int yp1,int xp2,int yp2): 
			Visual2D(xf1, yf1, xf2, yf2, xp1, yp1, xp2, yp2), 
			myShip(Point2D(xf1, yf1), Point2D(xf2, yf2), 
				Point2D((xf1 + xf2) / 2, (yf1 + yf2) /2), (yf2 - yf1) / 24, 
				Color(1, 0.5, 0)),
			enemies(){

	score = 0;
	livesLeft = 3;
	maxSimultaniousEnemies = 5;
	maxSimultaniousBonuses = 2;
	minDistanceFromShipOfNewEnemy = (xf2 - xf1) / 8;
	alive = true;
	cadruPoarta(Color(1, 1, 1));
	tipTran(true);
	addComplexObject2D(&myShip);
	level = 1;
	enemiesLeft = 20;
	myShip.makeInvinceable();
	shipBecameInvincible = std::clock();
	lastBonusAppeared = shipBecameInvincible;
}


void GameFieldVisual2D::moveShipUp(){
	myShip.moveUp();
	myShip.modifyLinearVelocity(1.05);
}

void GameFieldVisual2D::resetShipSpeed(){
	//Metoda apelata la ridicarea tastei

	myShip.resetLinearVelocity();
	myShip.resetCircularVelocity();
}

void GameFieldVisual2D::rotateShipLeft(){
	myShip.rotateLeft();
	myShip.modifyCircularVelocity(1.08);
}

void GameFieldVisual2D::rotateShipRight(){
	myShip.rotateRight();
	myShip.modifyCircularVelocity(1.08);
}

void GameFieldVisual2D::animate(){

	std::clock_t now = std::clock();
	
	if ( myShip.isInvinceable() && 
		(now - shipBecameInvincible) / CLOCKS_PER_SEC > 10)
			myShip.unmakeInvinceable();

	myShip.animateMe();
	Point2D result;
	for (auto itr = enemies.begin(); itr != enemies.end();){
		(*itr)->animateMe();
		if (myShip.intersects(**itr, result)){
			if (myShip.checkIfPointIsInGun(result)){
				removeComplexObject2D(*itr);
				score += (*itr)->value * level;
				delete (*itr);
				itr = enemies.erase(itr);
			}
			else{
				reset();
				return;
			}
		}
		else
			++itr;
	}

	for (auto itr = bonuses.begin(); itr != bonuses.end();)
		if (myShip.intersects(**itr, result))
			switch ((*itr)->type){
			case 1:
				livesLeft++;
				removeComplexObject2D(*itr);
				delete(*itr);
				itr = bonuses.erase(itr);
				break;

			case 2:
				myShip.makeInvinceable();
				shipBecameInvincible = std::clock();
				removeComplexObject2D(*itr);
				delete(*itr);
				itr = bonuses.erase(itr);
				break;

			default:
				++itr;
				break;
			}
		else
			++itr;


	if ((now - lastBonusAppeared) / CLOCKS_PER_SEC > 6){
	
		for (Bonus *b : bonuses){
			removeComplexObject2D(b);
			delete(b);
		}
		bonuses.clear();
	}

	if ((now - lastBonusAppeared) / CLOCKS_PER_SEC > 60){
			lastBonusAppeared = now;
			generateBonus();
	}
	
	generateEnemy();

	if (enemiesLeft == 0 && enemies.size() == 0)
		performLevelChange();
}

void GameFieldVisual2D::changeGunStatus(){
	myShip.changeGunStatus();
}

bool GameFieldVisual2D::stillAlive(){
	return alive;
}

const int GameFieldVisual2D::getScore(){
	return score;
}

const int GameFieldVisual2D::getLivesLeft(){
	return livesLeft;
}

const int GameFieldVisual2D::getCurrentLevel(){
	return level;
}

const int GameFieldVisual2D::getEnemiesLeft(){
	return enemiesLeft + enemies.size();
}

void GameFieldVisual2D::generateEnemy(){

	/**
	  * Metoda care genreaza un nou inamic daca este cazul
	**/

	if (enemies.size() >= maxSimultaniousEnemies || enemiesLeft == 0)
			return;

	enemiesLeft--;

	int enemyType = rand() % 5; 
	//numarul tipurilor de inamici + 1, adica pot sa nu generez
	Point2D position;
	switch (enemyType){
		case 0:
			position = generateRandomPositionAtMinDistance(
				Point2D(2 *(XFM - XFm) / 25 + XFm, 2 * (XFM - XFm) / 25 + YFm),
				Point2D(0.85 * XFM, 0.85 * YFM),
				myShip.getCurrentPosition(), minDistanceFromShipOfNewEnemy);
				
			enemies.push_back(new SquareInSquareEnemy(Point2D(XFm , YFm), 
				Point2D(XFM, YFM), position, (XFM - XFm) / 25));
			addComplexObject2D(enemies.back());
			break;

		case 1:
			position = generateRandomPositionAtMinDistance(
				Point2D(XFm + 2 *(XFM - XFm) / 30, 2 *(XFM - XFm) / 30 +  YFm),
				Point2D(0.8 * XFM, 0.8 * YFM),
				myShip.getCurrentPosition(), minDistanceFromShipOfNewEnemy);			
			enemies.push_back(new TwoSquaresEnemy(Point2D(XFm , YFm), 
				Point2D(XFM, YFM), position, (XFM - XFm) / 30));
			addComplexObject2D(enemies.back());
			break;

		case 2:
			position = generateRandomPositionAtMinDistance(
				Point2D(XFm + (XFM - XFm) / 20, (XFM - XFm) / 20 +  YFm),
				Point2D(0.8 * XFM, 0.8 * YFM),
				myShip.getCurrentPosition(), minDistanceFromShipOfNewEnemy);			
			enemies.push_back(new CircularSawEnemy(Point2D(XFm , YFm), 
				Point2D(XFM, YFM), position, (XFM - XFm) / 300));
			addComplexObject2D(enemies.back());
			break;

		case 3:
			position = generateRandomPositionAtMinDistance(
				Point2D(XFm + (XFM - XFm) / 20, (XFM - XFm) / 20 +  YFm),
				Point2D(0.8 * XFM, 0.8 * YFM),
				myShip.getCurrentPosition(), minDistanceFromShipOfNewEnemy);			
			enemies.push_back(new ArrowEnemy(Point2D(XFm , YFm), 
				Point2D(XFM, YFM), position, (XFM - XFm) / 25));
			addComplexObject2D(enemies.back());
			break;

		default:
			enemiesLeft++; //nu am generat niciunul
			return;

	}

	//Cu cat creste nivelul, cu atat creste viteza de deplasare a inamicilor

	enemies.back()->modifyLinearVelocity(1 + level / 5);
	enemies.back()->modifyCircularVelocity(1 + level / 5);
}

void GameFieldVisual2D::performLevelChange(){
	level++;
	maxSimultaniousEnemies = 5 + level;
	enemiesLeft = 20 + 2 * level;
}

void GameFieldVisual2D::reset(){
	for (Enemy *e : enemies){
		removeComplexObject2D(e);
		delete(e);
	}

	enemiesLeft = 20 + 2 * level;

	enemies.clear();
	if (--livesLeft >= 0){
		myShip.reset();
		myShip.makeInvinceable();
		shipBecameInvincible = std::clock();
	}
	else
		alive = false;
}

void GameFieldVisual2D::generateBonus(){

	bonusesToGenerateThisRun = rand() % (maxSimultaniousBonuses + 1);

	int bt; //bonus type
	Point2D position;

	for (int i = 0; i < bonusesToGenerateThisRun; ++i){
		
		bt = rand() % 2;
		switch (bt){

		case 0:
			position = generateRandomPositionAtMinDistance(
				Point2D(2 *(XFM - XFm) / 25 + XFm, 2 * (XFM - XFm) / 25 + YFm),
				Point2D(0.85 * XFM, 0.85 * YFM),
				myShip.getCurrentPosition(), minDistanceFromShipOfNewEnemy);
				
			bonuses.push_back(new LifeBonus(position, 
				(XFM - XFm) / 45));
			addComplexObject2D(bonuses.back());
			break;
			
		case 1:
			position = generateRandomPositionAtMinDistance(
				Point2D(2 *(XFM - XFm) / 25 + XFm, 2 * (XFM - XFm) / 25 + YFm),
				Point2D(0.85 * XFM, 0.85 * YFM),
				myShip.getCurrentPosition(), minDistanceFromShipOfNewEnemy);
				
			bonuses.push_back(new ShieldBonus(position, 
				(XFM - XFm) / 45));
			addComplexObject2D(bonuses.back());
			break;
		}
	}

}