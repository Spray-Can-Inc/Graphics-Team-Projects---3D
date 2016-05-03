#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include "cobject.h"
#ifndef GLCAMERA
#define GLCAMERA
#define PI 3.14159265

class Camera{

private:
	double xFocus = 0, yFocus = 0, zFocus = 0;
public:
	double xPos = 0, yPos = 0, zPos = 0,
		upX=0, upY=1, upZ=0;

	void Camera::setLocation(double x, double y, double z);
	void Camera::lookAt(double x, double y, double z);
	void Camera::rotate(double degrees, double x, double y, double z);
	void Camera::move(double amount);
	void Camera::moveSide(double amount); //moves the camera side to side relative to the direction it is facing
	void Camera::turn(double angle);
	void Camera::tilt(double angle);
	double Camera::distance(const Object3D obj);
	void Camera::Update();

};
#endif