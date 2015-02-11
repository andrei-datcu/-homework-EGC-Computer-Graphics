#pragma once
#include <bitset>

#include <glew/glew.h>
#include "lab_glut.hpp"
#include <glm/glm.hpp>

#include "Object3D.h"
#include "Athlete.h"

class MainWindow : public lab::glut::WindowListener
{
public:
	MainWindow();
	~MainWindow();

	void notifyBeginFrame();
	//functie de afisare, chemata inainte afisarea efectiva (swapBuffers)
	void notifyDisplayFrame();
	//functie chemata dupa sfarsirea procesului de afisare pe CPU
	void notifyEndFrame();

	//------------------- reshape - se apealeaza atunci cand ecranul este 
	void notifyReshape(int width, int height, int previos_width,
		int previous_height);

	//------------------- functii de input
	//functie chemata cand e apasata o tasta
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y);
	//functie chemata cand se termina apasarea unei taste
	void notifyKeyReleased(unsigned char key_released, int mouse_x,
		int mouse_y);
	//functie chemata cand o tasta speciala e apasata (up down, F1-12, etc)
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y);
	//functie chemata cand ose termina apsarea unei taste speciale
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y);
	//functie chemata cand se face mouse drag
	void notifyMouseDrag(int mouse_x, int mouse_y);
	//functie chemata cand mouse-ul se misca
	void notifyMouseMove(int mouse_x, int mouse_y);
	//functie chemata cand un button de mouse e apasat
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y);
	//functie chemata cand se face scroll cu mouse-ul.
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y);

private:

	unsigned int generateCheckeredTexture(const glm::uvec3 &color);
	void loadFinishShaders();
	void printText(std::string text);

	glm::mat4 original_model_matrix;
	glm::mat4 model_matrix, view_matrix, projection_matrix;
	//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int location_model_matrix, location_view_matrix,
		location_projection_matrix;
	unsigned int gl_program_shader, gl_flag_shader, location_texUnit,
		location_time, location_flags_view_matrix,
		location_flags_projection_matrix;
	glm::vec3 light_position;
	std::bitset<4> specialKeys;
	lab::Camera *currentCamera, topCamera;

	unsigned int checkeredTexture;

	bool gameOn, gameStarted;
	std::vector<Object3D> objects;
	std::vector<Object3D> flags;
	std::vector<Athlete> players;
};
