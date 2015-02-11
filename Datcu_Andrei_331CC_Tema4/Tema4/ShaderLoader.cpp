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

#include "ShaderLoader.h"
#include <glew/glew.h>
#include <iostream>
#include <fstream>
#include <vector>

ShaderLoader::ShaderLoader()
	: gl_program_object(0){
}


ShaderLoader::~ShaderLoader(){

	if (gl_program_object != 0)
		glDeleteProgram(gl_program_object);

	for (unsigned int shader : shader_list)
		glDeleteShader(shader);
}

bool ShaderLoader::init(){

	gl_program_object = glCreateProgram();

    if (gl_program_object == 0){
        std::cerr << "Error creating shader program" << std::endl;
        return false;
    }

    return true;
}

bool ShaderLoader::addShader(GLenum shader_type, const std::string &fileName){

	std::string shader_code;
	std::ifstream file(fileName.c_str(), std::ios::in);
	if(!file.good()){
		std::cout<<"Shader Loader: Nu am gasit fisierul shader "<< fileName
			<<" sau nu am drepturile sa il deschid!"<<std::endl;
		return false;
	}
	file.seekg(0, std::ios::end);
	shader_code.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shader_code[0], shader_code.size());
	file.close();

	int info_log_length=0,compile_result=0;
	unsigned int gl_shader_object;
			
	//construieste un obiect de tip shader din codul incarcat
	gl_shader_object = glCreateShader(shader_type);				
	const char *shader_code_ptr = shader_code.c_str();
	const int shader_code_size = shader_code.size();
	glShaderSource(gl_shader_object,1,&shader_code_ptr,&shader_code_size);	
	glCompileShader(gl_shader_object);
	glGetShaderiv(gl_shader_object, GL_COMPILE_STATUS, &compile_result);					
			
	//daca exista erori output la consola
	if(compile_result == GL_FALSE){
		std::string str_shader_type;
		if(shader_type == GL_VERTEX_SHADER) 
			str_shader_type="vertex shader";
		if(shader_type == GL_TESS_CONTROL_SHADER)
			str_shader_type="tess control shader";
		if(shader_type == GL_TESS_EVALUATION_SHADER)
			str_shader_type="tess evaluation shader";
		if(shader_type == GL_GEOMETRY_SHADER)
			str_shader_type="geometry shader";
		if(shader_type == GL_FRAGMENT_SHADER)
			str_shader_type="fragment shader";
		if(shader_type == GL_COMPUTE_SHADER)
			str_shader_type="compute shader";

		glGetShaderiv(gl_shader_object, GL_INFO_LOG_LENGTH, &info_log_length);		
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(gl_shader_object, info_log_length, NULL,
			&shader_log[0]);	
		std::cerr<<"Shader Loader: EROARE DE COMPILARE pentru "
			<< str_shader_type << std::endl <<&shader_log[0] << std::endl;
		return false;
	}

	shader_list.push_front(gl_shader_object);
	glAttachShader(gl_program_object, gl_shader_object);

	std::cerr << "Am incarcat shaderul " << fileName << std::endl;

    return true;
}

void ShaderLoader::enable(){
	glUseProgram(gl_program_object);
}

bool ShaderLoader::finalize(){

	int info_log_length=0,link_result=0;

	glLinkProgram(gl_program_object);												
	glGetProgramiv(gl_program_object, GL_LINK_STATUS, &link_result);

	//if we get link errors log them
	if(link_result == GL_FALSE){														
		glGetProgramiv(gl_program_object, GL_INFO_LOG_LENGTH,
			&info_log_length);		
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(gl_program_object, info_log_length, NULL,
			&program_log[0]);
		std::cerr<<"Shader Loader : EROARE DE LINKARE"<< std::endl
			<< &program_log[0] << std::endl;
		return false;
	}
			
	//delete the shader objects because we do not need them any more
	for( unsigned int shader : shader_list)
		glDeleteShader(shader);

	shader_list.clear();
	return true;
}

unsigned int ShaderLoader::getUniformLocation(const std::string &varName){

	unsigned int location = glGetUniformLocation(gl_program_object,
		varName.c_str());

    if (location == INVALID_UNIFORM_LOCATION) {
        std::cerr << "Warning! Unable to get the location of uniform "  <<
			varName << std::endl;
    }

    return location;
}