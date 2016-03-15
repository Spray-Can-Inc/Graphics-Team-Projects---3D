#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>

class Object3D
{

private:
	bool Object3D::Load(char *filename);	// Loads the model
	float* Object3D::calculateNormal(float* coord1, float* coord2, float* coord3);
protected:
	float* normals;							// Stores the normals
	float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles

public:
	Object3D(char *filename);
	void Object3D::Draw();					// Draws the model on the screen
	void Object3D::setLocation(float x, float y, float z);				
	void Object3D::Release();				
	float rotX = 0.0, rotY = 0.0,
		rotZ = 0.0, xPos = 0.0, yPos = 0.0,
		zPos = 0.0;

};