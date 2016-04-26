#include "cobject.h"


using namespace std;

#define POINTS_PER_VERTEX 3
#define TEXTCORDS_PER_VERTEX 2
#define TOTAL_FLOATS_IN_TRIANGLE 9


TextureObj::TextureObj(char* filename, char * imageFile)
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedPoints = 0;
	Load(filename);
	LoadBitMapImg(imageFile);
}

bool TextureObj::LoadBitMapImg(char *filename) {
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;


	// Open the file
	FILE * file;
	errno_t err;
	if ((err = fopen_s(&file, filename, "rt")) != 0) {
		printf("File was not opened\n");
		return 0;
	}
	/*if (!file) { 
		printf("Image could not be opened\n"); return 0; 
	}*/

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    
		imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      
		dataPos = 54; // The BMP header is done that way

	// Create a buffer
	textData = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(textData, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//enable texture
	glEnable(GL_TEXTURE_2D);
	// Create one OpenGL texture
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textData); //needs to be GL_BGR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

bool TextureObj::Load(char* filename)
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
		textCords = (float*)malloc(fileSize);
		textCordList = (float*)malloc(fileSize * sizeof(float));

		int triangle_index = 0;												// Set triangle index to zero
		int normal_index = 0;												// Set normal index to zero
		int texture_index = 0;


		while (!objFile.eof())											// Start reading file data
		{
			getline(objFile, line);											// Get line from file

			if (line.c_str()[0] == 'v')										// The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
				//if texture cord
				if (line.c_str()[1] == 't') {
					line[1] = ' ';
					sscanf_s(line.c_str(), "%f %f",							// Read floats from the line: v X Y Z
						&textCordList[TotalNumberOfTextCords],
						&textCordList[TotalNumberOfTextCords + 1]);	
					TotalNumberOfTextCords += TEXTCORDS_PER_VERTEX;
				}
				else {//if a vertex
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
			}
			if (line.c_str()[0] == 'f')										// The first character is an 'f': on this line is a point stored
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf

				int vertexNumber[4] = { 0, 0, 0 };
				int textureCord[4] = { 0, 0, 0 };
				sscanf_s(line.c_str(), "%i/%i %i/%i %i/%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],										// First point of our triangle. This is an 
					&textureCord[0],
					&vertexNumber[1],										// pointer to our vertexBuffer list
					&textureCord[1],
					&vertexNumber[2],
					&textureCord[2]);										// each point represents an X,Y,Z.

				vertexNumber[0] -= 1;										// OBJ file starts counting from 1
				vertexNumber[1] -= 1;										// OBJ file starts counting from 1
				vertexNumber[2] -= 1;										// OBJ file starts counting from 1
				textureCord[0] -= 1;
				textureCord[1] -= 1;
				textureCord[2] -= 1;
				
				/********************************************************************
				* Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z).
				* The vertexBuffer contains all verteces
				* The triangles will be created using the verteces we read previously
				*/

				int tCounter = 0;
				int txtCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					Faces_Triangles[triangle_index + tCounter] = vertexBuffer[3 * vertexNumber[i]];
					Faces_Triangles[triangle_index + tCounter + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
					Faces_Triangles[triangle_index + tCounter + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
					//load textures
					textCords[texture_index + txtCounter] = textCordList[2 * textureCord[i]];
					textCords[texture_index + txtCounter + 1] = textCordList[2 * textureCord[i] + 1];
					//textCords[texture_index + tCounter + 2] = textCordList[3 * textureCord[i] + 2];
					tCounter += POINTS_PER_VERTEX;
					
					txtCounter += 2;
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
				texture_index += 6;
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

void TextureObj::Release()
{
	free(this->Faces_Triangles);
	free(this->textData);
	free(this->textCords);
	free(this->textCordList);
	free(this->normals);
	free(this->vertexBuffer);
	free(this->name);
}

void TextureObj::Draw()
{
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);
	glRotatef(rotZ, 0, 0, 1);
	glScalef(scale, scale, scale);
	glColor3f(red, green, blue);
	
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays	
	//glEnableClientState(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, textureID); //binds to the texture in the gpu
	
	//enable coordinates texture
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,0,textCords);
	//the rest
	glVertexPointer(3, GL_FLOAT, 0, Faces_Triangles);				// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
	//glDisableClientState(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
