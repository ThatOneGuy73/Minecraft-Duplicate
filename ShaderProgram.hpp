#pragma once

#include "Globals.hpp"
#include <glm/glm.hpp>
#include <string>
#include <fstream>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void use();
	void setVec3(std::string location, glm::vec3 vector);
	void setMat4(std::string location, glm::mat4 matrix);

private:
	std::string loadFile(std::string src);

private:
	GLuint programID;
};

