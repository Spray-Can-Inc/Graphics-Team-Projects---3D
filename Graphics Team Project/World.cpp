#include "world.h"

using namespace std;

World::World(char* filename)
{
	Load(filename);
}

bool World::Load(char* filename)
{
	string line;
	ifstream objFile(filename);

	if (!objFile) {
		cout << "\nFile NOT FOUND:" << filename << endl;
		return false;
	}


	if (objFile.is_open())													// If obj file is open, continue
	{
		objFile.seekg(0, ios::end);										// Go to end of the file, 
		long fileSize = objFile.tellg();									// get file size
		objFile.seekg(0, ios::beg);										// we'll use this to register memory for our 3d model
		string file;
		objects = (Object3D*)malloc(800*sizeof(Object3D));			// Allocate memory for 800 objects
		//Object3D obj;

		while (!objFile.eof())											// Start reading file data
		{
			getline(objFile, line);											// Get line from file

			if (line.c_str()[0] == 'f')										// The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';											// Set first character to 0. This will allow us to use sscanf
				size_t fPos=line.find_first_of("'\"");
				size_t lPos = line.find_last_of("'\"");
				file =line.substr(fPos+1, (lPos - fPos)-1);
				cout << file.c_str() << endl;
			}
			if (line.c_str()[0] == 'o')										// The first character is an 'f': on this line is a point stored
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
				float pos[4] = { 0, 0, 0 };
				sscanf_s(line.c_str(), "%f %f %f ",								// Read integers from the line:  f 1 2 3
					&pos[0],										// the x position
					&pos[1],										// the y position
					&pos[2]);										// the z position
				Object3D obj((char*)file.c_str());
				obj.setLocation(pos[0], pos[1], pos[2]);
				objects[OBJECT_COUNT] = obj;
				OBJECT_COUNT++;
			}
			if (line.c_str()[0] == 'c')										// The first character is an 'f': on this line is a point stored
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
				float color[4] = { 0, 0, 0 };
				sscanf_s(line.c_str(), "%f %f %f ",								// Read integers from the line:  f 1 2 3
					&color[0],										// the x position
					&color[1],										// the y position
					&color[2]);										// the z position
				objects[OBJECT_COUNT-1].setColor(color[0], color[1], color[2]);
			}
			if (line.c_str()[0] == 's')										// The first character is an 'f': on this line is a point stored
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
				float scale = 1;
				sscanf_s(line.c_str(), " %f ", &scale);									
				objects[OBJECT_COUNT - 1].setScale(scale);
			}
		}
		objFile.close();	//Close OBJ file
		return true;
	}
	else
	{
		cout << "Unable to open file";
	}
	return false;
}


void World::Draw() {
	for (int i = 0; i < OBJECT_COUNT; i++) {
		objects[i].Draw();
	}

}

void World::Release()
{
	free(this->objects);
}