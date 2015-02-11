/* Autor: Datcu Andrei Daniel - 331CC
 * GameFieldVisual2D
 * Contextul visual al jocului. Clasa mentine jocul, verifica daca trebuie
 * sa dispara vreun inamic sau nava, modifica scorul, etc
 */

#pragma once
#include "../Framework/Visual2D.h"
#include "ship.h"
#include "Enemy.h"
#include "Bonus.h"
#include <vector>
#include <queue>
#include <ctime>

class GameFieldVisual2D : public Visual2D
{
public:
	GameFieldVisual2D
		(float xf1,float yf1,float xf2,float yf2,int xp1,
			int yp1,int xp2,int yp2);

	void moveShipUp();
	void resetShipSpeed();
	void rotateShipLeft();
	void rotateShipRight();
	void animate();
	void changeGunStatus();
	bool stillAlive();
	const int getScore();
	const int getLivesLeft();
	const int getCurrentLevel();
	const int getEnemiesLeft();

private:
	Ship myShip;
	std::vector<Enemy*> enemies;
	std::vector <Bonus*> bonuses;
	int score, livesLeft, maxSimultaniousEnemies, minDistanceFromShipOfNewEnemy;
	int level, enemiesLeft, maxSimultaniousBonuses, bonusesToGenerateThisRun;
	bool alive;
	std::clock_t shipBecameInvincible, lastBonusAppeared;
	

	void reset();
	void generateEnemy();
	void generateBonus();
	void performLevelChange();
};

