/*
 * Autor: Datcu Andrei Daniel
 * Grupa: 331CC
 * 
 * Mesh - clasa de mesha animabila. Practic tine cele 3 atribute in vbo
 *		  (pozitie, normala, texcoord) si in plus inca un vbo cu cele
 *		  3 atribute ale imaginii urmatoare
 */

#include <glew/glew.h>
#include "Mesh.h"
#include <ctime>
#include "lab_mesh_loader.hpp"
#include "lab_texture_loader.hpp"

unsigned int Mesh::location_shader_time, Mesh::location_shader_totaltime;
std::unordered_map<std::string, unsigned int> Mesh::texturesIds;

Mesh::Mesh(const std::string &objFileName, const std::string &textureFileName,
		   float animation_secs)
		: animation_secs(animation_secs), ticks_since_start(0){

	lab::loadObj(objFileName, vao, attr_vbo, ibo, vertex_no);
	
	texid = loadTexture(textureFileName);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

bool Mesh::renderMesh(){

	if (ticks_since_start == 0){ //primul cadru al animatiei

		glUniform1f(location_shader_totaltime, animation_secs);
		ticks_since_start = std::clock();
	}

	float seconds_passed = (float)(std::clock() - ticks_since_start)
							/ CLOCKS_PER_SEC;

	glUniform1f(location_shader_time, seconds_passed);

	// bind texture
	glBindTexture(GL_TEXTURE_2D, texid);
	// bind VAO
	glBindVertexArray(vao);
	// draw
	glDrawElements(GL_TRIANGLES, vertex_no,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);

	return seconds_passed < animation_secs;
}

void Mesh::resetTicks(){

	ticks_since_start = 0;
}

void Mesh::setNextAttributes(unsigned int vbo){

	nextpos_vbo = vbo;
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, nextpos_vbo);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),
						  (void*)0);			//trimite pozitii pe pipe 0
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),
						  (void*)(sizeof(float)*3));//trimite normale pe pipe 1
	glBindVertexArray(0);
}

unsigned int Mesh::loadTexture(const std::string &path){

	if (texturesIds.find(path) == texturesIds.end()){
		texturesIds[path] = lab::loadTextureBMP(path);
	}

	return texturesIds[path];
}

Mesh::~Mesh(){

    glDeleteBuffers(1, &attr_vbo);
	glDeleteVertexArrays(1, &vao);
}