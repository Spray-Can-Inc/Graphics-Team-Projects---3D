#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#ifndef COBJECT
#define COBJECT

#define PI 3.14159265

class Object3D
{

private:
	bool Object3D::Load(char *filename);	// Loads the model
protected:
	float* Object3D::calculateNormal(float* coord1, float* coord2, float* coord3);
	bool Object3D::isBetween(float value, float min, float max);
	float* normals;							// Stores the normals
	float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles
	float maxX = -9999, minX = 9999, maxY = -9999, minY = 9999, maxZ = -9999, minZ = 9999,
		scale=1, red=.8,green=.8,blue=.8;
	char* name = "unknown";

public:
	Object3D(char *filename);
	Object3D::Object3D();
	void Object3D::Draw();					// Draws the model on the screen
	void Object3D::setLocation(float x, float y, float z);
	void Object3D::setColor(float r, float g, float b);
	void Object3D::setScale(float scale);
	void Object3D::move(float amount);
	void Object3D::Release();
	void Object3D::setName(char* name);
	char* Object3D::getName();
	bool Object3D::isIntersecting(const Object3D obj);
	float rotX = 0.0, rotY = 0.0,
		rotZ = 0.0, xPos = 0.0, yPos = 0.0,
		zPos = 0.0;

};

class TextureObj : public Object3D
{

private:
	bool TextureObj::Load(char *filename);	// Loads the model
	bool TextureObj::LoadBitMapImg(char *filename);
protected:
	float* textCords;							// Stores the texture cordinates
	unsigned int imageSize;                         // = width*height*3
	unsigned char * textData;											// Actual RGB data
	float* textCordList;
	long TotalNumberOfTextCords;
	GLuint textureID;

public:
	TextureObj(char *filename, char * imageFile);
	void TextureObj::Draw();					// Draws the model on the screen
	void TextureObj::Release();

};

#endif