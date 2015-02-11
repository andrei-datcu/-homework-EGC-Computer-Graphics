/*
 * Autor: Datcu Andrei Daniel
 * Grupa: 331CC
 * 
 * ShaderLoader - clasa care se ocupa de incarcarea mai multor shadere
 * pentru un singur program. Utilizare:
 *
 * ShaderLoader sl;
 * sl.init();
 * sl.addShader(....);
 * sl.finalize();
 *
 * Clasa este mai flexibila decat functia de la laborator, putand fi exitnsa
 * cu usurinta si realiza anumite operatii intre compilarea si linkarea
 * shaderului (gen enable Transform Feedback)
 *
 */

#pragma once

#include <string>
#include <forward_list>

#include <glew/glew.h>

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

class ShaderLoader
{
public:
	ShaderLoader();
	~ShaderLoader();

	virtual bool init();
	bool addShader(GLenum shader_type, const std::string &fileName);
	unsigned int getUniformLocation(const std::string &varName);
	void enable();
	bool finalize();

protected:
	unsigned int gl_program_object;

private:
	std::forward_list<unsigned int> shader_list;
};

