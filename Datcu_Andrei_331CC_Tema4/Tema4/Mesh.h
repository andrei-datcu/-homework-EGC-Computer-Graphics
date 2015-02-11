/*
 * Autor: Datcu Andrei Daniel
 * Grupa: 331CC
 * 
 * Mesh - clasa de mesha animabila. Practic tine cele 3 atribute in vbo
 *		  (pozitie, normala, texcoord) si in plus inca un vbo cu cele
 *		  3 atribute ale imaginii urmatoare
 */

#pragma once

#include <vector>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Mesh{
public:
	Mesh(const std::string &objFileName, const std::string &textureFileName,
		 float animation_secs);
	~Mesh();

	static unsigned int loadTexture(const std::string &path);
	bool renderMesh();
	void resetTicks();
	void setNextAttributes(unsigned int vbo); //urmatoarea stare de animatie

	unsigned int attr_vbo, vao;
	static unsigned int location_shader_time, location_shader_totaltime;
	static std::unordered_map<std::string, unsigned int> texturesIds;
	
private:
	unsigned int nextpos_vbo, ibo, texid, vertex_no,
		ticks_since_start;
	const float animation_secs;
};
