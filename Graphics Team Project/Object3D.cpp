#include "cobject.h"


using namespace std;

#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9


Object3D::Object3D(char* filename)
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedPoints = 0;
	Load(filename);
}
Object3D::Object3D()
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedPoints = 0;
}


float* Object3D::calculateNormal(float *coord1, float *coord2, float *coord3)
{
	/* calculate Vector1 and Vector2 */
	float va[3], vb[3], vr[3], val;
	va[0] = coord1[0] - coord2[0];
	va[1] = coord1[1] - coord2[1];
	va[2] = coord1[2] - coord2[2];

	vb[0] = coord1[0] - coord3[0];
	vb[1] = coord1[1] - coord3[1];
	vb[2] = coord1[2] - coord3[2];

	/* cross product */
	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	/* normalization factor */
	val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

	float norm[3];
	norm[0] = vr[0] / val;
	norm[1] = vr[1] / val;
	norm[2] = vr[2] / val;


	return norm;
}


bool Object3D::Load(char* filename)
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

		vertexBuffer = (float*)malloc(fileSize);							// Allocate memory for the verteces
		Faces_Triangles = (float*)malloc(fileSize*sizeof(float));			// Allocate memory for the triangles
		normals = (float*)malloc(fileSize*sizeof(float));					// Allocate memory for the normals

		int triangle_index = 0;												// Set triangle index to zero
		int normal_index = 0;												// Set normal index to zero

		while (!objFile.eof())											// Start reading file data
		{
			getline(objFile, line);											// Get line from file

			if (line.c_str()[0] == 'v')										// The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf

				sscanf_s(line.c_str(), "%f %f %f ",							// Read floats from the line: v X Y Z
					&vertexBuffer[TotalConnectedPoints],
					&vertexBuffer[TotalConnectedPoints + 1],
					&vertexBuffer[TotalConnectedPoints + 2]);

				//build the bounding box
				if (minX > vertexBuffer[TotalConnectedPoints])
					minX = vertexBuffer[TotalConnectedPoints];
				if (maxX < vertexBuffer[TotalConnectedPoints])
					maxX = vertexBuffer[TotalConnectedPoints];
				if (minY > vertexBuffer[TotalConnectedPoints + 1])
					minY = vertexBuffer[TotalConnectedPoints + 1];
				if (maxY < vertexBuffer[TotalConnectedPoints + 1])
					maxY = vertexBuffer[TotalConnectedPoints + 1];
				if (minZ > vertexBuffer[TotalConnectedPoints + 2])
					minZ = vertexBuffer[TotalConnectedPoints + 2];
				if (maxZ < vertexBuffer[TotalConnectedPoints + 2])
					maxZ = vertexBuffer[TotalConnectedPoints + 2];

				TotalConnectedPoints += POINTS_PER_VERTEX;					// Add 3 to the total connected points
			}
			if (line.c_str()[0] == 'f')										// The first character is an 'f': on this line is a point stored
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf

				int vertexNumber[4] = { 0, 0, 0 };
				sscanf_s(line.c_str(), "%i%i%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],										// First point of our triangle. This is an 
					&vertexNumber[1],										// pointer to our vertexBuffer list
					&vertexNumber[2]);										// each point represents an X,Y,Z.

				vertexNumber[0] -= 1;										// OBJ file starts counting from 1
				vertexNumber[1] -= 1;										// OBJ file starts counting from 1
				vertexNumber[2] -= 1;										// OBJ file starts counting from 1


				/********************************************************************
				* Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z).
				* The vertexBuffer contains all verteces
				* The triangles will be created using the verteces we read previously
				*/

				int tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					Faces_Triangles[triangle_index + tCounter] = vertexBuffer[3 * vertexNumber[i]];
					Faces_Triangles[triangle_index + tCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
					Faces_Triangles[triangle_index + tCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
					tCounter += POINTS_PER_VERTEX;
				}

				/*********************************************************************
				* Calculate all normals, used for lighting
				*/
				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index + 1], Faces_Triangles[triangle_index + 2] };
				float coord2[3] = { Faces_Triangles[triangle_index + 3], Faces_Triangles[triangle_index + 4], Faces_Triangles[triangle_index + 5] };
				float coord3[3] = { Faces_Triangles[triangle_index + 6], Faces_Triangles[triangle_index + 7], Faces_Triangles[triangle_index + 8] };
				float *norm = this->calculateNormal(coord1, coord2, coord3);

				tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					normals[normal_index + tCounter] = norm[0];
					normals[normal_index + tCounter + 1] = norm[1];
					normals[normal_index + tCounter + 2] = norm[2];
					tCounter += POINTS_PER_VERTEX;
				}

				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;
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

void Object3D::setLocation(float x, float y, float z){
	xPos = x;
	yPos = y;
	zPos = z;
}

void Object3D::setScale(float scale) {
	this->scale = scale;
	this->maxX *= this->scale;
	this->maxY *= this->scale;
	this->maxZ *= this->scale;
	this->minX *= this->scale;
	this->minY *= this->scale;
	this->minZ *= this->scale;
}

void Object3D::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
	free(this->name);
}

void Object3D::setName(char* name) {
	this->name = new char[strlen(name)];
	strcpy_s(this->name, strlen(name)+1, name);
	cout << this->name << endl;
}

char* Object3D::getName() {
	return this->name;
}

void Object3D::setColor(float r, float g, float b) {
	red = r;
	green = g;
	blue = b;
}

void Object3D::setColorRender(bool setting)
{
	colorOn = setting;
}

bool Object3D::isIntersecting(const Object3D obj) {
	bool xAxis = false, yAxis = false, zAxis = false;
	//check if objects 2 is in object 1
	if (isBetween(obj.minX + obj.xPos, this->minX + this->xPos, this->maxX + this->xPos) ||
		isBetween(obj.maxX + obj.xPos, this->minX + this->xPos, this->maxX + this->xPos)) {
		xAxis = true;
	}
	if (isBetween(obj.minY + obj.yPos, this->minY + this->yPos, this->maxY + this->yPos) ||
		isBetween(obj.maxY + obj.yPos, this->minY + this->yPos, this->maxY + this->yPos)) {
		yAxis = true;
	}
	if (isBetween(obj.minZ + obj.zPos, this->minZ + this->zPos, this->maxZ + this->zPos) ||
		isBetween(obj.maxZ + obj.zPos, this->minZ + this->zPos, this->maxZ + this->zPos)) {
		zAxis = true;
	}
	//check if objects 1 is in object 2
	if (isBetween(this->minX + this->xPos, obj.minX + obj.xPos, obj.maxX + obj.xPos) ||
		isBetween(this->maxX + this->xPos, obj.minX + obj.xPos, obj.maxX + obj.xPos)) {
		xAxis = true;
	}
	if (isBetween(this->minY + this->yPos, obj.minY + obj.yPos, obj.maxY + obj.yPos) ||
		isBetween(this->maxY + this->yPos, obj.minY + obj.yPos, obj.maxY + obj.yPos)) {
		yAxis = true;
	}
	if (isBetween(this->minZ + this->zPos, obj.minZ + obj.zPos, obj.maxZ + obj.zPos) ||
		isBetween(this->maxZ + this->zPos, obj.minZ + obj.zPos, obj.maxZ + obj.zPos)) {
		zAxis = true;
	}

	if (xAxis&&yAxis&&zAxis)
		return true;
	return false;
}
bool Object3D::isBetween(float value, float min, float max) {
	if (value <= max && value >= min) {
		return true;
	}
	return false;
}

void Object3D::move(float amount) {
	xPos += cos(rotY * PI / 180.0)*amount;
	yPos += sin(rotX* PI / 180.0)*amount;
	zPos += -sin(rotY* PI / 180.0)*amount;
}

void Object3D::Draw()
{
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);
	glRotatef(rotZ, 0, 0, 1);
	glScalef(scale, scale, scale);
	if(colorOn)
		glColor3f(red, green, blue);
	else
		glColor3f(.8, .8, .8);
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3, GL_FLOAT, 0, Faces_Triangles);				// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
	glPopMatrix();
}
