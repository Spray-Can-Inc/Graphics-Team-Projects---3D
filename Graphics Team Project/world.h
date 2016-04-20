#pragma once
#include "cobject.h"
#include "glcamera.h"

class World
{

private:
	bool World::Load(char *filename);	// Loads the world file
	int OBJECT_COUNT = 0;
	
protected:
	Object3D* objects;

public:
	World(char *filename);
	void World::Draw();					// Draws the model on the screen
	void World::addObject(Object3D* obj);
	void World::Release();
	Object3D* World::getObjectByName(char* name);

};