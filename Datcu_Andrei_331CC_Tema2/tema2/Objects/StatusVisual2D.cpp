/* Autor: Datcu Andrei Daniel - 331CC
 * StatusVisual2D
 * Contextul care arata starea jocului
 */

#include "StatusVisual2D.h"
#include "../Framework/Text.h"
#include "../Framework/DrawingWindow.h"


StatusVisual2D::StatusVisual2D(float xf1,float yf1,float xf2,float yf2,int xp1,
			int yp1,int xp2,int yp2): 
				Visual2D(xf1, yf1, xf2, yf2, xp1, yp1, xp2, yp2),
				scoreText("0", Point2D((xf2 - xf1) / 2,
					(yf2 - yf1) / 3), Color(0,1,0), BITMAP_TIMES_ROMAN_24),
				livesText("0", Point2D( 2.2 * (xf2 - xf1) / 3,
					(yf2 - yf1) / 3), Color(0,1,0), BITMAP_TIMES_ROMAN_24)
			{

	DrawingWindow::addText_to_Visual2D(new Text("SCORE", 
		Point2D((xf2 - xf1) / 2 - 35,
				(yf2 - yf1) / 2), Color(0,1,0), BITMAP_TIMES_ROMAN_24), this);

	DrawingWindow::addText_to_Visual2D(&scoreText, this);
	DrawingWindow::addText_to_Visual2D(&livesText, this);
}

void StatusVisual2D::setScore(int score){
	scoreText.text = std::to_string(score);
}

void StatusVisual2D::setLives(int lives){
	livesText.text = "Lives left: " +(lives >= 0 ? std::to_string(lives) : "");
}