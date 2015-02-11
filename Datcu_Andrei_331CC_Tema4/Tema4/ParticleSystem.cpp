/*
 * Autor: Datcu Andrei Daniel
 * Grupa: 331CC
 * 
 * UpdateShaderLoader - clasa care se ocupa shaderele pentru generarea
 *						particulelor cu TransformFeedback
 *
 * BillboardShaderLoader - clasa care se ocupa cu shaderele pentru
 *						   transformarea particulelor in billboarduri si
 *						   texturarea acestora
 *
 * ParticleSystem - clasa care se ocupa cu managamentul particulelor,
 *					celor 2 buffere si interschimbarea acestora inainte de
 *					randare
 */

#include <ctime>

#include "ParticleSystem.h"
#include "Mesh.h"

#include <glm\gtc\type_ptr.hpp>
#include <glew/glew.h>

bool UpdateShaderLoader::init(){

    if (!ShaderLoader::init()) 
        return false;

    if (!addShader(GL_VERTEX_SHADER,
				   "shadere\\particle_update_shader_vertex.glsl")) {
        return false;
    }

    if (!addShader(GL_GEOMETRY_SHADER,
				   "shadere\\particle_update_shader_geometry.glsl")) {
        return false;
    }

	const GLchar* feedbackVaryings[] = { "position1", "velocity1", "color1" };
	glTransformFeedbackVaryings(gl_program_object, 3, feedbackVaryings,
		GL_INTERLEAVED_ATTRIBS);

    if (!finalize()) {
        return false;
    }
    
	location_time = getUniformLocation("time");
    location_initialpos = getUniformLocation("initialpos");
    location_extremepos = getUniformLocation("extremepos");
	location_deltatime = getUniformLocation("delta_time");
    
    if (location_initialpos == INVALID_UNIFORM_LOCATION ||
        location_time == INVALID_UNIFORM_LOCATION ||
		location_deltatime == INVALID_UNIFORM_LOCATION ||
        location_extremepos == INVALID_UNIFORM_LOCATION) {
        return false;
    }
    
    return true;
}

void UpdateShaderLoader::setTime(float time){
	glUniform1f(location_time, time);
}

void UpdateShaderLoader::setDeltaTime(float time){
	glUniform1f(location_deltatime, time);
}

void UpdateShaderLoader::setInitialPos(const glm::vec3 &pos){
	glUniform3f(location_initialpos, pos.x, pos.y, pos.z);
}

void UpdateShaderLoader::setExtremePos(const glm::vec3 &pos){
	glUniform3f(location_extremepos, pos.x, pos.y, pos.z);
}

bool BillboardShaderLoader::init(){
	if (!ShaderLoader::init()) {
        return false;
    }

    if (!addShader(GL_VERTEX_SHADER,
				   "shadere\\particle_billboard_shader_vertex.glsl")) {
        return false;
    }

    if (!addShader(GL_GEOMETRY_SHADER,
				   "shadere\\particle_billboard_shader_geometry.glsl")) {
        return false;
    }

    if (!addShader(GL_FRAGMENT_SHADER,
				   "shadere\\particle_billboard_shader_fragment.glsl")) {
        return false;
    }

    if (!finalize()) {
        return false;
    }

	location_modelmatrix = getUniformLocation("model_matrix");
	location_viewmatrix = getUniformLocation("view_matrix");
	location_projectionmatrix = getUniformLocation("projection_matrix");
	location_texunit = getUniformLocation("texUnit");
	location_billboardsize = getUniformLocation("gBillboardSize");

    if (location_modelmatrix == INVALID_UNIFORM_LOCATION ||
        location_viewmatrix == INVALID_UNIFORM_LOCATION ||
        location_projectionmatrix == INVALID_UNIFORM_LOCATION ||
        location_texunit == INVALID_UNIFORM_LOCATION ||
		location_billboardsize == INVALID_UNIFORM_LOCATION){
        return false;        
    }
    
    return true;
}

void BillboardShaderLoader::setViewMatrix(const glm::mat4 &view_matrix){

	glUniformMatrix4fv(location_viewmatrix, 1, false,
		glm::value_ptr(view_matrix));
}

void BillboardShaderLoader::setModelMatrix(const glm::mat4 &model_matrix){
	
	glUniformMatrix4fv(location_modelmatrix, 1, false,
		glm::value_ptr(model_matrix));
}

void BillboardShaderLoader::setProjectionMatrix(
	const glm::mat4 &projection_matrix){
	
	glUniformMatrix4fv(location_projectionmatrix, 1, false,
		glm::value_ptr(projection_matrix));
}

void BillboardShaderLoader::setTexUnit(unsigned int texunit){
	glUniform1ui(location_texunit, texunit);
}

void BillboardShaderLoader::setBillboardSize( float size){
	glUniform1f(location_billboardsize, size);
}

ParticleSystem::ParticleSystem(const glm::mat4 &model_matrix,
							   const glm::mat4 &view_matrix,
							   const glm::mat4 &proj_matrix)

		: model_matrix(model_matrix),
		  view_matrix(view_matrix),
		  proj_matrix(proj_matrix){

	currVB = 0;
    currTFB = 1;
    lastRenderClock = 0;
    texid = 0;
	particleBuffer[0] = particleBuffer[1] = 0;
	transformFeedback[0] = transformFeedback[1] = 0;
}


ParticleSystem::~ParticleSystem(){

	if (transformFeedback[0] != 0)
        glDeleteTransformFeedbacks(2, transformFeedback);
    
    if (particleBuffer[0] != 0)
        glDeleteBuffers(2, particleBuffer);
}

bool ParticleSystem::init(const std::string& textureName,
						  const glm::vec3 &initialPos,
						  const glm::vec3 &extremePos){

	struct Particle{
		glm::vec3 position, velocity, color;
	};

	Particle particles[MAX_PARTICLES];

    particles[0].position = initialPos;
	particles[0].velocity = glm::vec3(0, 0, 0);
	particles[0].color = glm::vec3(0, 0, 0);
    
    glGenTransformFeedbacks(2, transformFeedback);    
    glGenBuffers(2, particleBuffer);
    
    for (unsigned int i = 0; i < 2 ; i++) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles,
					 GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffer[i]);        
    }
                      
    if (!updateShader.init())
        return false;
    
    updateShader.enable();

	updateShader.setInitialPos(initialPos);
	updateShader.setExtremePos(extremePos);
    
	texid = Mesh::loadTexture(textureName);
    if (texid == 0) {
        return false;
    }
    
    if (!billboardShader.init())
        return false;
    
    billboardShader.enable();

	billboardShader.setTexUnit(0);
	billboardShader.setBillboardSize(0.32f);
	
	return true;
}

void ParticleSystem::updatePositions(){

	updateShader.enable();

	bool firstCall = false;

	if (lastRenderClock == 0){ //first time
		lastRenderClock = std::clock();
		firstCall = true;
	}

	int cclock = std::clock();

	updateShader.setTime((float) cclock / CLOCKS_PER_SEC);
	
	updateShader.setDeltaTime((float) (cclock - lastRenderClock) /
		(1.5 * CLOCKS_PER_SEC));
   
    glEnable(GL_RASTERIZER_DISCARD); // nu folosesc fragment shader la update
    
    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[currVB]);    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback[currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	 // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3),
						  (void*) sizeof(glm::vec3));         // velocity 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3),
						  (void*) (2 * sizeof(glm::vec3)));         // color 
    glBeginTransformFeedback(GL_POINTS);

    if (firstCall) 
        glDrawArrays(GL_POINTS, 0, 1);
    else
        glDrawTransformFeedback(GL_POINTS, transformFeedback[currVB]);         
  
    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void ParticleSystem::renderParticles(){
	billboardShader.enable();

	billboardShader.setModelMatrix(model_matrix);
	billboardShader.setViewMatrix(view_matrix);
	billboardShader.setProjectionMatrix(proj_matrix);
    
    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[currTFB]);    

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3),
		(void*) (2 * sizeof(glm::vec3))); //color

    glBindTexture(GL_TEXTURE_2D, texid);
	glDrawTransformFeedback(GL_POINTS, transformFeedback[currTFB]);
    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void ParticleSystem::render(){

    updatePositions();
    renderParticles();

    currVB = currTFB;
    currTFB = (currTFB + 1) & 0x1;
}