/* Autor: Datcu Andrei Daniel - 331CC
 * StatusVisual2D
 * Contextul care arata starea jocului
 */

#include "StatusVisual2D.h"
#include "../Framework/Text.h"


StatusVisual2D::StatusVisual2D(float xf1,float yf1,float xf2,float yf2,int xp1,
			int yp1,int xp2,int yp2): 
				Visual2D(xf1, yf1, xf2, yf2, xp1, yp1, xp2, yp2),
				scoreText("0", Point2D((xf2 - xf1) / 2,
					(yf2 - yf1) / 3), Color(0,1,0), BITMAP_TIMES_ROMAN_24),
				livesText("0", Point2D( 2 * (xf2 - xf1) / 3,
					(yf2 - yf1) / 3), Color(0,1,0), BITMAP_TIMES_ROMAN_24),
				levelText("Heat: ", Point2D( 2 * (xf2 - xf1) / 3,
					(yf2 - yf1) / 2), Color(0,1,0), BITMAP_TIMES_ROMAN_24),
				enemiesText("Enemies Left: ", Point2D( 2 * (xf2 - xf1) / 3,
					2 * (yf2 - yf1) / 3), Color(0,1,0), BITMAP_TIMES_ROMAN_24)
				{

	addText(new Text("SCORE", Point2D((xf2 - xf1) / 2 - 35,
				(yf2 - yf1) / 2), Color(0,1,0), BITMAP_TIMES_ROMAN_24));

	addText(&scoreText);
	addText(&livesText);
	addText(&levelText);
	addText(&enemiesText);
}

void StatusVisual2D::setScore(int score){
	scoreText.text = std::to_string(score);
}

void StatusVisual2D::setLives(int lives){
	livesText.text = "Lives left: " +(lives >= 0 ? std::to_string(lives) : "");
}

void StatusVisual2D::setEnemiesLeft(int enemies){
	enemiesText.text = "Enemies Left: " + std::to_string(enemies);
}

void StatusVisual2D::setLevel(int level){
	levelText.text = "Heat: " + std::to_string(level);
}
