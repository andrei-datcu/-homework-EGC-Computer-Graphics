#include "Athlete.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <cmath>

#define PI 3.14159265359f

Athlete::Athlete(const unsigned int &location_model_matrix,
		const glm::mat4 &original_model_matrix, const std::string &fileName,
		const glm::vec3 &topLeft, const glm::vec3 &bottomRight,
		const float speed, const glm::uvec3 &color, Object3D *cam):

		Object3D(location_model_matrix, original_model_matrix, fileName),
		topLeft(topLeft), bottomRight(bottomRight),
		cam(cam), speed(speed), lapscompleted(0), color(color){

	//Pentru atleti, setam, in plus, camerele
	
	tpsBackCamera.set(glm::vec3(0,2.5,-2.5), glm::vec3(0,2.5,0),
		glm::vec3(0, 1, 0));
	tpsSideCamera.set(glm::vec3(-8, 2,0),glm::vec3(0,2,0), glm::vec3(0, 1, 0));
	
	//Daca are un obiect3d Cam atasat camerei de pe margine

	if(cam){
		cam->rotate(PI/2, glm::vec3(0, 1, 0));
		cam->translate(glm::vec3(-8, 0, 0));
	}
	fpsCamera.set(glm::vec3(0,2.5,0), glm::vec3(0,2.5,1), glm::vec3(0, 1, 0));

	float width = topLeft.x - bottomRight.x;
	
	autoAngle = 2 * speed / width;
	translate(bottomRight);
	rotating = false;
}

void Athlete::translate(glm::vec3 v){

	Object3D::translate(v);
	tpsBackCamera.translateForward(v.z);
	tpsBackCamera.translateRight(-v.x);
	tpsBackCamera.translateUpword(v.y);

	tpsSideCamera.translateRight(v.z);
	tpsSideCamera.translateForward(v.x);
	tpsSideCamera.translateUpword(v.y);
	if (cam)
		cam->setTranslation(glm::vec3(tpsSideCamera.position.x - 0.5, 0,
		tpsSideCamera.position.z));

	fpsCamera.translateForward(v.z);
	fpsCamera.translateRight(-v.x);
	fpsCamera.translateUpword(v.y);
}

void Athlete::moveForward(){

	/*
	 * Metoda care muta jucatorul in fata, dupa unghiul curent al rotatiei
	 * Totdata, muta si cele 3 camere asociate, conform enuntului
	 */

	static glm::vec3 center((bottomRight.x + topLeft.x) / 2, topLeft.y,
		topLeft.z);
	float dm = speed;
	glm::vec3 tv(dm * glm::sin(rotationAngle), 0, dm *glm::cos(rotationAngle));
	if (!checkPosition(translationVector + tv))
		return;

	float futurez = translationVector.z + tv.z;

	if (translationVector.x < center.x)
	if (futurez >= bottomRight.z && translationVector.z < bottomRight.z)
		lapscompleted++;
	else
		if (futurez < bottomRight.z && translationVector.z >= bottomRight.z)
			lapscompleted--;

	Object3D::translate(tv);
	tpsBackCamera.translateForward(dm);
	fpsCamera.translateForward(dm);

	int halfsign = translationVector.x < center.x ? -1 : 1;

	if (translationVector.z >topLeft.z || translationVector.z < bottomRight.z){
		center.z = translationVector.z > topLeft.z ? topLeft.z : bottomRight.z;
		static const float distance = 50;
		float playerdistance = sqrtf (pow(translationVector.x - center.x, 2) +
			pow(translationVector.y - center.y, 2) +
			pow(translationVector.z - center.z, 2)); 

		glm::vec3 newcameracenter = center + (glm::vec3(translationVector.x,
			translationVector.y , translationVector.z) - center) /
			playerdistance * distance;

		newcameracenter.y = 2;
		center.y = 2;
		tpsSideCamera.set(newcameracenter, center, glm::vec3(0, 1, 0));
	}
	else{
		tpsSideCamera.translateRight(-halfsign * dm * glm::cos(rotationAngle));
	}

	if (cam){
		int pifactor = !((translationVector.x < center.x || translationVector.z
			< bottomRight.z) && translationVector.z < topLeft.z);
		cam->setTranslation(glm::vec3(tpsSideCamera.position.x + halfsign *0.5,
			0, tpsSideCamera.position.z));
		cam->setRotation(glm::atan(tpsSideCamera.forward.x /
			tpsSideCamera.forward.z) + pifactor * PI, glm::vec3(0, 1, 0));
	}
}

void Athlete::rotate(float dangle){
	
	Object3D::rotate(dangle, glm::vec3(0, 1, 0));
	tpsBackCamera.rotateTPSoY(-dangle, 2.5);
	fpsCamera.rotateFPSoY(-dangle);
}

void Athlete::autoMove(){

	/*
	 * Metoda pentru mutarea automata a jucatorului, pentru cei care nu sunt
	 * controlati de la tastatura
	 */

	float width = topLeft.x - bottomRight.x;
	if (translationVector.z > topLeft.z || translationVector.z < bottomRight.z)
		rotate(autoAngle), rotating = true;
	else
		if (rotating){
			rotating = false;
			float difPI = floorf(rotationAngle / PI + 0.5f);
			rotate(difPI * PI - rotationAngle);
		}
	moveForward();
}

bool Athlete::checkPosition(const glm::vec3 &v){

	/*
	 * Metoda care intoarce true daca jucatorul ramane pe pisa la o viitoare
	 * mutare in fata
	 */

	static glm::vec3 center((bottomRight.x + topLeft.x) / 2,
		topLeft.y, topLeft.z);

	if (v.z > topLeft.z || v.z < bottomRight.z){
		center.z = v.z > topLeft.z ? topLeft.z : bottomRight.z;
		float dist = glm::l2Norm(v - center);
		if (dist > 47.5f || dist < 38.0f)
			return false;
	}
	else
		if (v.x > -102.35f && v.x < -25.8f || v.x < -111.2f || v.x > -16.5f)
			return false;
	return true;
}

int Athlete::getLapsCompleted(){
	return lapscompleted;
}