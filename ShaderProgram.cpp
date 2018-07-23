#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram()
{
	std::string vss = loadFile("shaders/vertexShaderSource.glsl");
	std::string fss = loadFile("shaders/fragmentShaderSource.glsl");
	const char* vertexShaderSource = vss.c_str();
	const char* fragmentShaderSource = fss.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	glDetachShader(programID, vertexShader);
	glDetachShader(programID, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(programID);
}

void ShaderProgram::use()
{
	glUseProgram(programID);
}

void ShaderProgram::setVec3(std::string location, glm::vec3 vector)
{
	glUniform3fv(glGetUniformLocation(programID, location.c_str()), 1, &vector[0]);
}

void ShaderProgram::setMat4(std::string location, glm::mat4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, location.c_str()), 1, false, &matrix[0][0]);
}

std::string ShaderProgram::loadFile(std::string src)
{
	std::fstream file(src);
	if (!file.is_open()) {
		print("Failed to open file: " + src + "!");
	}
	std::string line;
	std::string lines = "";
	while (std::getline(file, line)) {
		lines += lines.length() == 0 ? line + '\n' : line;
	}
	return lines;
}
