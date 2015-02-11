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

#pragma once

#include "ShaderLoader.h"
#include <glm/glm.hpp>
#define MAX_PARTICLES 10000

class UpdateShaderLoader : public ShaderLoader{
public:
	bool init();
	void setTime(float time);
	void setDeltaTime(float time);
	void setInitialPos(const glm::vec3 &pos);
	void setExtremePos(const glm::vec3 &pos);

private:
	unsigned int location_initialpos, location_extremepos,
				 location_time, location_deltatime;
};

class BillboardShaderLoader: public ShaderLoader{
public:
	bool init();
	void setViewMatrix(const glm::mat4 &view_matrix);
	void setModelMatrix(const glm::mat4 &model_matrix);
	void setProjectionMatrix(const glm::mat4 &projection_matrix);
	void setBillboardSize( float size);
	void setTexUnit(unsigned int texunit);

private:
	unsigned int location_viewmatrix, location_modelmatrix,
		location_projectionmatrix, location_texunit, location_billboardsize;
};

class ParticleSystem{

public:
	ParticleSystem(const glm::mat4 &model_matrix, const glm::mat4 &view_matrix,
				   const glm::mat4 &proj_matrix);
	~ParticleSystem();
	void render();
	bool init(const std::string& textureName, const glm::vec3 &initialPos,
			  const glm::vec3 &extremePos);

private:
	void renderParticles();
	void updatePositions();

	unsigned int currVB;
    unsigned int currTFB;
	unsigned int particleBuffer[2];
    unsigned int transformFeedback[2];
	unsigned int texid;
	int lastRenderClock;
	UpdateShaderLoader updateShader;
    BillboardShaderLoader billboardShader;
	const glm::mat4 &model_matrix, &view_matrix, &proj_matrix;
};

