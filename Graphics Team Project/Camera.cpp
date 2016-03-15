#include "glcamera.h"



void Camera::setLocation(double x, double y, double z){
	xPos = x; yPos = y; zPos = z;
	Update();
}

void Camera::lookAt(double x, double y, double z){
	x = x - xPos;
	y = y - yPos;
	z = z - zPos;
	double magnitude = sqrt((x*x) + (y*y) + (z*z));
	if (magnitude == 0){
		xFocus = 0; yFocus = 0; zFocus = 0;
	}
	else{
		xFocus = x / magnitude; yFocus = y / magnitude; zFocus = z / magnitude;
	}
	Update();
}
void Camera::move(double amount){
	xPos += amount*xFocus; 
	yPos += amount*yFocus; 
	zPos += amount*zFocus;
	Update();

}
void Camera::moveSide(double amount){
	xPos -= amount*sin(atan2(zFocus, xFocus));
	zPos += amount*cos(atan2(zFocus, xFocus));
	Update();

}

void Camera::Update(){
	gluLookAt(xPos, yPos, zPos, xFocus + xPos, yFocus + yPos, zFocus + zPos, 0, 1, 0);
}

void Camera::tilt(double angle){

	//Do Later
}
void Camera::turn(double angle){
	//Do Later 

}

void Camera::rotate(double degrees, double x, double y, double z){
	double cosine = cos(degrees * PI / 180.0),
		sine = sin(degrees * PI / 180.0);
	if (x > 0){
		yFocus = yFocus*cosine - zFocus*sine;
		zFocus = yFocus*sine + zFocus*cosine;
	}
	if (y > 0){
		xFocus = xFocus*cosine + zFocus*sine;
		zFocus = -xFocus*sine + zFocus*cosine;
	}
	if (z > 0){
		xFocus = xFocus*cosine - yFocus*sine;
		yFocus = xFocus*sine + yFocus*cosine;
	}

	Update();
}

