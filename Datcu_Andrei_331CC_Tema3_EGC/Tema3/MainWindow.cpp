
#define _VARIADIC_MAX 8 //damn MSVC
#include "MainWindow.h"
#include "lab_shader_loader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Mesh.h"

#include <ctime>
#include <fstream>

MainWindow::MainWindow(){

	//setari pentru desenare opengl
	glClearColor(0.5,0.5,0.5,1);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	//incarca un shaderele principale
	gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl",
		"shadere\\shader_fragment.glsl");

	location_model_matrix = glGetUniformLocation(gl_program_shader,
		"model_matrix");
	location_view_matrix = glGetUniformLocation(gl_program_shader,
		"view_matrix");
	location_projection_matrix = glGetUniformLocation(gl_program_shader,
		"projection_matrix");

	//Bloc uniform prin care trimit proprietatiile de material
	glUniformBlockBinding(gl_program_shader,
		glGetUniformBlockIndex(gl_program_shader,"Material"),
		Mesh::shaderMaterialUniformLocation);

	location_texUnit = glGetUniformLocation(gl_program_shader,"texUnit");


	//matrici de modelare si vizualizare

	light_position = glm::vec3(-50,80,-50);

	original_model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	view_matrix = glm::lookAt(glm::vec3(0, 5,8), glm::vec3(0,0,0),
		glm::vec3(0,1,0));
	glUniform1d(location_texUnit,0);

	//adaugam obiectele din scena - camera de pe margine si stadionul
	objects.reserve(5);
	objects.emplace_back(location_model_matrix, original_model_matrix,
		"resurse\\stadion_light.obj");
	objects.emplace_back(location_model_matrix, original_model_matrix,
		"resurse\\camera_small.obj");

	glm::vec3 topLeft2(-18.3, 0.4, -28),
		bottomRight2(-110, 0.4, -107.8); //-- culoarul 2
	glm::vec3 topLeft4(-20.7, 0.4, -28.1),
		bottomRight4(-107.6, 0.4, -107.7);// --culoarul 4
	glm::vec3 topLeft6(-23.1, 0.4, -28.2),
		bottomRight6(-105.1, 0.4, -107.6);//-- culoarul 6

	//citesc vitezele din fisier

	float speeds[3];

	std::ifstream fin("resurse\\speeds.txt", std::ios::in);

	fin >> speeds[0] >> speeds[1] >> speeds[2];
	fin.close();

	//incarc cei trei jucatori
	//playerul 0 va fi controlat de la tastatura
	players.reserve(5);
	players.emplace_back(location_model_matrix, original_model_matrix,
		"resurse\\sports_guy_small.obj", topLeft6, bottomRight6, speeds[0],
		glm::uvec3(130, 130, 130), &objects[1]);
	players.emplace_back(location_model_matrix, original_model_matrix,
		"resurse\\sports_guy_small_yellow.obj",
		topLeft4, bottomRight4, speeds[1], glm::uvec3(255, 170, 0));
	players.emplace_back(location_model_matrix, original_model_matrix,
		"resurse\\sports_guy_small_blue.obj", topLeft2, bottomRight2,
		speeds[2], glm::uvec3(0, 0, 255));
	
	//initial camera va fi setata pe spatele playerul 0
	currentCamera = &players[0].tpsBackCamera;
	
	//incarc shaderele pentru afisarea steagurilor de final
	loadFinishShaders();
	unsigned int loc_flag_model_matrix = glGetUniformLocation(gl_flag_shader,
		"model_matrix");
	
	//adaug 2 steaguri
	
	flags.reserve(5);
	flags.emplace_back(loc_flag_model_matrix, original_model_matrix,
		"resurse\\flag5.obj");
	flags.emplace_back(loc_flag_model_matrix, original_model_matrix,
		"resurse\\flag5.obj");

	flags[0].translate(glm::vec3(-100, 0, 0));
	flags[1].rotate(3.1415f, glm::vec3(0, 1, 0));
	flags[1].translate(glm::vec3(150, 0, 0));

	gameOn = true;
	gameStarted = false;
}


MainWindow::~MainWindow(){

	glDeleteProgram(gl_program_shader);
	glDeleteProgram(gl_flag_shader);

	
	for (auto &p : Mesh::texturesId)
		if (p.second > 0)
			glDeleteTextures(1, &p.second);
}


//functie chemata inainte de a incepe cadrul de desenare
void MainWindow::notifyBeginFrame(){
	
	static float dangle = 0.01f; //viteza unghiulara a playerului 0

	if (!gameOn || !gameStarted)
		return;

	//ceilalti 2 jucatori se deplaseaza automat

	players[1].autoMove();
	players[2].autoMove();

	//Verific care taste sunt apasate si nu au fost inca ridicate

	for (int key = 0; key < 4; ++key)
		if (specialKeys.test(key))
			switch (key + GLUT_KEY_LEFT){
			case GLUT_KEY_LEFT:
				players[0].rotate(dangle);
				break;

			case GLUT_KEY_RIGHT:
				players[0].rotate(-dangle);
				break;

			case GLUT_KEY_UP:
				players[0].moveForward();
				break;
			}
}

//functia de afisare (lucram cu banda grafica)
void MainWindow::notifyDisplayFrame(){
	//bufferele din framebuffer sunt aduse la valorile initiale
	//adica se sterge ecranul si se pune culoare (si alte propietati) initiala
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!gameStarted){// Inca nu s-a apasat space pentru a incepe jocul
		printText("Press space to start!");
		return;
	}

	if (!gameOn){ // s-a terminat jocul

		//foloseste shaderul corespunzator
		glUseProgram(gl_flag_shader);
		static float time = (float)std::clock() / CLOCKS_PER_SEC;
		glUniform1f(location_time,
			((float)std::clock() / CLOCKS_PER_SEC -time) * 100);
		glUniformMatrix4fv(location_flags_view_matrix, 1,
			false, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(location_flags_projection_matrix, 1,
			false, glm::value_ptr(projection_matrix));
		
		for (Object3D &f: flags)
			f.renderObject();
		return;
	}

	glUseProgram(gl_program_shader);
	//trimite variabile uniforme la shader
	glUniformMatrix4fv(location_view_matrix, 1, false,
		glm::value_ptr(currentCamera->getViewMatrix()));
	glUniformMatrix4fv(location_projection_matrix, 1, false,
		glm::value_ptr(projection_matrix));
	glUniform3f(glGetUniformLocation(gl_program_shader, "light_position"),
		light_position.x, light_position.y, light_position.z);
	glUniform3f(glGetUniformLocation(gl_program_shader, "eye_position"),
		currentCamera->position.x, currentCamera->position.y,
		currentCamera->position.z);

	for (Object3D &o : objects)
		o.renderObject();

	for (Athlete &a : players)
		a.renderObject();
}

//functie chemata dupa ce am terminat cadrul de desenare
void MainWindow::notifyEndFrame(){

	const static int noOfLaps = 2;
	if (!gameStarted)
		return;

	//Daca jocul este activ si oricare din cei doi jucatori a facut noOfLaps

	if (gameOn)
		for (Athlete &a: players)
			if (a.getLapsCompleted() == noOfLaps){
				gameOn = false;
				original_model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0,
					0,0,0,1);
				view_matrix = glm::lookAt(glm::vec3(50, 100, 169),
					glm::vec3(0, 100, 0), glm::vec3(0, 1, 0));
				checkeredTexture = generateCheckeredTexture(a.color);
				Mesh::texturesId["flag.phoney"] = checkeredTexture;
			}
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
	
	if(key_pressed == ' ')
		gameStarted = true; //La space incepe jocul

	if (!gameOn || !gameStarted)
		return;

	static float dx = 0.1f, dangle = 0.005f;
	//vitezele pentru camera fps de deasupra stadionului

	switch (toupper(key_pressed)){

	case 'Q':
		currentCamera = &players[0].tpsBackCamera;
		break;

	case 'W':
		currentCamera = &players[0].tpsSideCamera;
		break;

	case 'E':
		currentCamera = &players[0].fpsCamera;
		break;

	case 'S':
		currentCamera = &players[1].fpsCamera;
		break;

	case 'X':
		currentCamera = &players[2].fpsCamera;
		break;

	case 'A':
		currentCamera = &players[1].tpsBackCamera;
		break;

	case 'Z':
		currentCamera = &players[2].tpsBackCamera;
		break;

	case 'T':
		topCamera.set(glm::vec3(-60, 80, -65), glm::vec3(-60, 1, -65),
			glm::vec3(1, -1, 0));
		currentCamera = &topCamera;
		break;

	case 'I':
		topCamera.translateForward(dx);
		break;

	case 'K':
		topCamera.translateForward(-dx);
		break;

	case 'L':
		topCamera.translateRight(dx);
		break;

	case 'J':
		topCamera.translateRight(-dx);
		break;

	case 'O':
		topCamera.translateUpword(dx);
		break;

	case 'P':
		topCamera.translateUpword(-dx);
		break;

	case '[':
		topCamera.rotateFPSoX(-dangle);
		break;

	case ']':
		topCamera.rotateFPSoX(dangle);
		break;

	case ';':
		topCamera.rotateFPSoY(-dangle);
		break;

	case '\'':
		topCamera.rotateFPSoY(dangle);
		break;

	case '.':
		topCamera.rotateFPSoZ(-dangle);
		break;

	case '/':
		topCamera.rotateFPSoZ(dangle);
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

	switch (key_pressed){
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:	
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		specialKeys.set(key_pressed - GLUT_KEY_LEFT);
		break;
	}
}
//tasta speciala ridicata
void MainWindow::notifySpecialKeyReleased(int key_released, int mouse_x,
										  int mouse_y){
	switch (key_released){
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:	
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		specialKeys.reset(key_released - GLUT_KEY_LEFT);
		break;
	}
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

unsigned int MainWindow::generateCheckeredTexture(const glm::uvec3 &color){

	static const unsigned int checkImageHeight = 64, checkImageWidth = 64;
	static unsigned char checkImage[checkImageHeight][checkImageWidth][4];

	for (unsigned int i = 0; i < checkImageHeight; i++)
		for (unsigned int j = 0; j < checkImageWidth; j++){
			unsigned char c = ((((i&0x8)==0)^((j&0x8))==0));
			checkImage[i][j][0] = (1 - c) * color.r + c * 255;  
			checkImage[i][j][1] = (1 - c) * color.g + c * 255;  
			checkImage[i][j][2] = (1 - c) * color.b + c * 255;  
			checkImage[i][j][3] = (GLubyte) 255;
		}

	unsigned int texid;

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texid;
}

void MainWindow::loadFinishShaders(){
		
	gl_flag_shader = lab::loadShader("shadere\\shader_flag_vertex.glsl",
		"shadere\\shader_flag_fragment.glsl");
	location_flags_view_matrix = glGetUniformLocation(gl_flag_shader,
		"view_matrix");
	location_flags_projection_matrix = glGetUniformLocation(gl_flag_shader,
		"projection_matrix");
	location_time = glGetUniformLocation(gl_flag_shader, "time"); 

	glUniformMatrix4fv(location_flags_view_matrix, 1, false,
		glm::value_ptr(view_matrix));
	glUniformMatrix4fv(location_flags_projection_matrix, 1, false,
		glm::value_ptr(projection_matrix));
	glUniform1d(glGetUniformLocation(gl_flag_shader, "texUnit"), 0);
}

void MainWindow::printText(std::string text){

	glColor3f(0.0f, 0.0f, 1.0f);
	glRasterPos2i(0, 0);
	for (char c : text)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
}