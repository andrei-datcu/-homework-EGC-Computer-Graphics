/* Autor: Datcu Andrei Daniel - 331CC
 * StatusVisual2D
 * Contextul care arata starea jocului
 */

#pragma once
#include "../Framework/visual2d.h"
#include "../Framework/Text.h"
class StatusVisual2D : public Visual2D{
public:
	StatusVisual2D(float xf1,float yf1,float xf2,float yf2,int xp1,
			int yp1,int xp2,int yp2);
	void setScore (int score);
	void setLives (int lives);
	void setLevel (int level);
	void setEnemiesLeft (int enemies);

private:
	Text scoreText, livesText, levelText, enemiesText;
};

