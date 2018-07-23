#include "Cube.hpp"

#define V0 -1.f, -1.f, 1.f
#define V1 1.f, -1.f, 1.f
#define V2 1.f, 1.f, 1.f
#define V3 -1.f, 1.f, 1.f

#define V4 -1.f, -1.f, -1.f
#define V5 1.f, -1.f, -1.f
#define V6 1.f, 1.f, -1.f
#define V7 -1.f, 1.f, -1.f

#define N0 0.f, 0.f, 1.f
#define N1 1.f, 0.f, 0.f
#define N2 0.f, 0.f, -1.f
#define N3 -1.f, 0.f, 0.f
#define N4 0.f, -1.f, 0.f
#define N5 0.f, 1.f, 0.f

const std::vector<GLfloat> Cube::vertices = {
	V0, V1, V2, V2, V3, V0, //front
	V1, V5, V6, V6, V2, V1, //right
	V7, V6, V5, V5, V4, V7, //back
	V4, V0, V3, V3, V7, V4, //left
	V4, V5, V1, V1, V0, V4, //bottom
	V3, V2, V6, V6, V7, V3 //top
};

const std::vector<GLfloat> Cube::normals = {
	N0, N0, N0, N0, N0, N0,
	N1, N1, N1, N1, N1, N1,
	N2, N2, N2, N2, N2, N2,
	N3, N3, N3, N3, N3, N3,
	N4, N4, N4, N4, N4, N4,
	N5, N5, N5, N5, N5, N5
};

Cube::Cube(std::string textureSource, std::vector<GLfloat> uvCoords)
{
	setColor(0.f, 0.f, 0.f);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &cboID);
	glBindBuffer(GL_ARRAY_BUFFER, cboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), &colors[0], GL_STATIC_DRAW);

	glGenBuffers(1, &nboID);
	glBindBuffer(GL_ARRAY_BUFFER, nboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvboID);
	glBindBuffer(GL_ARRAY_BUFFER, uvboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvCoords.size(), &uvCoords[0], GL_STATIC_DRAW);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	sf::Image tex;
	tex.loadFromFile(textureSource);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.getSize().x, tex.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.getPixelsPtr());
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &cboID);
	glDeleteBuffers(1, &nboID);
	glDeleteBuffers(1, &uvboID);
	glDeleteTextures(1, &texID);
}

void Cube::setColor(float r, float g, float b)
{
	for (int i = 0; i < 36 * 3; i += 3) {
		colors.push_back(r);
		colors.push_back(g);
		colors.push_back(b);
	}
}

void Cube::setPosition(float x, float y, float z)
{
	Model = glm::translate(Model, glm::vec3(x - *this->x, y - *this->y, z - *this->z));
}

void Cube::draw()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, cboID);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, nboID);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, uvboID);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::mat4 Cube::getModel()
{
	return Model;
}

float Cube::operator[](int index)
{
	return Model[3][index];
}
