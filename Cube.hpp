#pragma once

#include "Globals.hpp"
#include <vector>
#include <SFML/Graphics/Image.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube
{
public:
	Cube(std::string textureSource, std::vector<GLfloat> uvCoords);
	~Cube();

	void setColor(float r, float g, float b);
	void setPosition(float x, float y, float z);
	void draw();

	glm::mat4 getModel();

	float operator[](int index);

private:
	static const std::vector<GLfloat> vertices;
	static const std::vector<GLfloat> normals;
	std::vector<GLfloat> colors;

	GLuint vaoID;
	GLuint vboID;
	GLuint cboID;
	GLuint nboID;
	GLuint texID;
	GLuint uvboID;

	glm::mat4 Model;

	float* x = &Model[3][0];
	float* y = &Model[3][1];
	float* z = &Model[3][2];
};

