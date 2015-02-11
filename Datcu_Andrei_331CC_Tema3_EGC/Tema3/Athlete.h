#pragma once
#include "object3d.h"
#include "lab_camera.hpp"
class Athlete : public Object3D{

	using Object3D::Object3D;
public:
	Athlete(const unsigned int &location_model_matrix,
		const glm::mat4 &original_model_matrix, const std::string &fileName,
		const glm::vec3 &topLeft, const glm::vec3 &bottomRight,
		const float speed, const glm::uvec3 &color, Object3D *cam = NULL);
	void translate(glm::vec3 v);
	void rotate(float angle);
	void moveForward();
	void autoMove();
	int getLapsCompleted();

	lab::Camera tpsBackCamera, tpsSideCamera, fpsCamera;
	const glm::uvec3 color;

private:

	bool checkPosition(const glm::vec3 &v);

	Object3D *cam;
	float speed, autoAngle;
	glm::vec3 topLeft, bottomRight;
	bool rotating;
	int lapscompleted;
};

