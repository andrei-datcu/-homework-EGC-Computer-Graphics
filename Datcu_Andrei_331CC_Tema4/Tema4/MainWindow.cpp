/*
 * Autor: Datcu Andrei Daniel
 * Grupa: 331CC
 * 
 * MainWindow - clasa care se ocupa de contextul
 *				si controlul ferestrei principale
 *
 */

#define _VARIADIC_MAX 8 //damn MSVC
#include "MainWindow.h"
#include "lab_shader_loader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Mesh.h"

#include <ctime>
#include <fstream>

MainWindow::MainWindow()
		:ps(NULL){

	//setari pentru desenare opengl
	glClearColor(0.5,0.5,0.5,1);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	//incarca un shaderele principale

	renderShader.init();
	renderShader.addShader(GL_VERTEX_SHADER, "shadere\\shader_vertex.glsl");
	renderShader.addShader(GL_FRAGMENT_SHADER,
						   "shadere\\shader_fragment.glsl");
	renderShader.finalize();

	location_model_matrix = renderShader.getUniformLocation("model_matrix");
	location_view_matrix = renderShader.getUniformLocation("view_matrix");
	location_projection_matrix =
		renderShader.getUniformLocation("projection_matrix");

	location_texUnit = renderShader.getUniformLocation("texUnit");

	Mesh::location_shader_time =
		renderShader.getUniformLocation("current_time");
	Mesh::location_shader_totaltime =
		renderShader.getUniformLocation("total_time");

	//matrici de modelare si vizualizare

	light_position = glm::vec3(- 5,8, 1.5);
	eye_position = glm::vec3(-1, 7, 2.5);
	
	location_light = renderShader.getUniformLocation("light_position");

	location_eyepos = renderShader.getUniformLocation("eye_position");
	
	original_model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	view_matrix = glm::lookAt(eye_position, glm::vec3(0,7,0),
		glm::vec3(0,1,0));
	glUniform1d(location_texUnit,0);

	glActiveTexture(GL_TEXTURE0);
	//adaugam obiectele din scena
	keyframes.reserve(5);
	keyframes.emplace_back("resurse\\girl_sleep.obj",
						   "resurse\\girl_texture.bmp", 5.0f);
	keyframes.emplace_back("resurse\\girl_surprise.obj",
						   "resurse\\girl_texture.bmp", 5.0f);
	keyframes.emplace_back("resurse\\girl_annoyed.obj", 
						   "resurse\\girl_texture.bmp", 5.0f);
	keyframes[0].setNextAttributes(keyframes[0].attr_vbo);
	keyframes[1].setNextAttributes(keyframes[2].attr_vbo);
	keyframes[2].setNextAttributes(keyframes[2].attr_vbo);
	girlAwake = false;
	currentFrame = 0;
}


MainWindow::~MainWindow(){

	for (auto &t : Mesh::texturesIds)
		glDeleteTextures(1, &t.second);
}


//functie chemata inainte de a incepe cadrul de desenare
void MainWindow::notifyBeginFrame(){
}

//functia de afisare (lucram cu banda grafica)
void MainWindow::notifyDisplayFrame(){
	//bufferele din framebuffer sunt aduse la valorile initiale
	//adica se sterge ecranul si se pune culoare (si alte propietati) initiala
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (girlAwake)
		ps->render();
	
	renderShader.enable();
	//trimite variabile uniforme la shader
	glUniformMatrix4fv(location_view_matrix, 1, false,
		glm::value_ptr(view_matrix));
	glUniformMatrix4fv(location_model_matrix, 1, false,
		glm::value_ptr(original_model_matrix));
	glUniformMatrix4fv(location_projection_matrix, 1, false,
		glm::value_ptr(projection_matrix));

	glUniform3f(location_light,
		light_position.x, light_position.y, light_position.z);

	glUniform3f(location_eyepos, eye_position.x,
		eye_position.y, eye_position.z);


	bool result = keyframes[currentFrame].renderMesh();

	switch (currentFrame){
		
	case 0:
		if (girlAwake)
			if (!result){
				currentFrame = 1;
				keyframes[1].resetTicks();
			}
		break;

	case 1:
		if (!result)
			currentFrame = 2;
		break;

	case 2:
		if (!girlAwake)
			if (!result)
				currentFrame = 0;
		break;
	}
}

//functie chemata dupa ce am terminat cadrul de desenare
void MainWindow::notifyEndFrame(){

}

//functei care e chemata cand se schimba dimensiunea ferestrei initiale
void MainWindow::notifyReshape(int width, int height, int previos_width,
							   int previous_height){
	//reshape
	if(height == 0)
		height=1;
	glViewport(0, 0, width, height);
	projection_matrix = glm::perspective(90.0f, (float)width / (float)height,
		0.1f, 10000.0f);
}


//-----------------------------------------------------------------------------
//functii de input output -----------------------------------------------------

//tasta apasata
void MainWindow::notifyKeyPressed(unsigned char key_pressed, int mouse_x,
								  int mouse_y){

	if(key_pressed == 27)
		lab::glut::close();	//ESC inchide glut si 

	switch (toupper(key_pressed)){

	case ' ':
		float t = (float) (std::clock() - lastPressTick) / CLOCKS_PER_SEC;

		const float minTime = girlAwake ? 10.0f : 5.0f;

		if (t < minTime)
			break;

		lastPressTick = std::clock();
		girlAwake = !girlAwake;
		if (girlAwake){
			keyframes[0].setNextAttributes(keyframes[1].attr_vbo);
			keyframes[0].resetTicks();
			keyframes[2].setNextAttributes(keyframes[2].attr_vbo);

			ps = new ParticleSystem(original_model_matrix, 
									view_matrix, projection_matrix);
			ps->init("resurse\\music.bmp", glm::vec3(-3, 9, -0.25),
					 glm::vec3(9, 0, -0.25));
		}
		else{
			delete(ps);
			ps = NULL;
			keyframes[2].setNextAttributes(keyframes[0].attr_vbo);
			keyframes[2].resetTicks();
			keyframes[0].setNextAttributes(keyframes[0].attr_vbo);
		}
		break;
	}
}

//tasta ridicata
void MainWindow::notifyKeyReleased(unsigned char key_released, int mouse_x,
								   int mouse_y){
}


void MainWindow::notifySpecialKeyPressed(int key_pressed, int mouse_x,
										 int mouse_y){

	if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
	if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();

}
//tasta speciala ridicata
void MainWindow::notifySpecialKeyReleased(int key_released, int mouse_x,
										  int mouse_y){
}
//drag cu mouse-ul
void MainWindow::notifyMouseDrag(int mouse_x, int mouse_y){ }
//am miscat mouseul (fara sa apas vreun buton)
void MainWindow::notifyMouseMove(int mouse_x, int mouse_y){ }
//am apasat pe un boton
void MainWindow::notifyMouseClick(int button, int state, int mouse_x,
								  int mouse_y){ }

//scroll cu mouse-ul
void MainWindow::notifyMouseScroll(int wheel, int direction, int mouse_x,
								   int mouse_y){
}