#include "ShaderProgram.hpp"
#include "Cube.hpp"
#include <SFML/Graphics.hpp>
#include <thread>

#define GRASS_SIDE 0.5f, 0.5f, 1.f, 0.5f, 1.f, 0.f, 1.f, 0.0f, 0.5f, 0.f, 0.5f, 0.5f
#define GRASS_BACK 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 1.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.f
#define GRASS_TOP 0.5f, 0.5f, 0.5f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.5f, 0.5f, 0.5f
#define GRASS_BOTTOM 0.f, 0.5f, 0.5f, 0.5f, 0.5f, 0.f, 0.5f, 0.f, 0.f, 0.f, 0.f, 0.5f

#define STONE_FACE 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f

std::vector<GLfloat> grassUV = {
	GRASS_SIDE, GRASS_SIDE, GRASS_BACK, GRASS_SIDE, GRASS_BOTTOM, GRASS_TOP
};

std::vector<GLfloat> stoneUV = {
	STONE_FACE, STONE_FACE, STONE_FACE, STONE_FACE, STONE_FACE, STONE_FACE
};

std::vector<GLfloat> dirtUV = {
	GRASS_BOTTOM, GRASS_BOTTOM, GRASS_BOTTOM, GRASS_BOTTOM, GRASS_BOTTOM, GRASS_BOTTOM
};

#define GRASS "textures/MC-GrassBlock.png", grassUV
#define DIRT "textures/MC-GrassBlock.png", dirtUV
#define STONE "textures/MC-Stone.png", stoneUV

float w_width = 800;
float w_height = 600;

bool running = true;

int main() {
	sf::RenderWindow window(sf::VideoMode(w_width, w_height), "Minecraft 2", sf::Style::Default, sf::ContextSettings(3, 3));
	sf::Mouse::setPosition(sf::Vector2i(w_width / 2.f, w_height / 2.f), window);

	if (!glewInit() == GLEW_OK) {
		print("Failed to initialize GLEW!");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderProgram shaderProgram;
	std::vector<Cube*> blocks;

	float h_angle = 4.f;
	float v_angle = -0.5f;
	float mouseSpeed = 0.2f;
	float speed = 10.f;
	glm::vec3 position = glm::vec3(0, 6, 10);
	glm::vec3 direction = glm::vec3(0, 0, 0);

	float offsetZ = 0;
	float offsetX = -16;

	sf::Clock clock;
	while (running) {
		float deltaTime = clock.restart().asSeconds();
		sf::Event evt;
		while (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
				running = false;
			}
			if (evt.type == sf::Event::Resized) {
				w_width = evt.size.width;
				w_height = evt.size.height;
				window.setView(sf::View(sf::FloatRect(0, 0, w_width, w_height)));
			}
			if (evt.type == sf::Event::KeyReleased) {
				if (evt.key.code == sf::Keyboard::Escape) {
					window.close();
					running = false;
				}
			}
		}

		if (blocks.size() != 16 * 16 * 3) {
			Cube* cube1 = new Cube(GRASS);
			Cube* cube2 = new Cube(DIRT);
			Cube* cube3 = new Cube(STONE);
			cube1->setPosition(offsetX, 2, offsetZ);
			cube2->setPosition(offsetX, 0, offsetZ);
			cube3->setPosition(offsetX, -2, offsetZ);
			offsetX += 2;
			if (offsetX == 16){
				offsetZ += 2;
				offsetX = -16;
			}
			blocks.push_back(cube1);
			blocks.push_back(cube2);
			blocks.push_back(cube3);
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Mouse::setPosition(sf::Vector2i(w_width / 2.f, w_height / 2.f), window);

		h_angle += mouseSpeed * deltaTime * float(w_width / 2.f - mousePos.x);
		v_angle += mouseSpeed * deltaTime * float(w_height / 2.f - mousePos.y);
		if (v_angle > PI / 2.f || v_angle < -PI / 2.f) {
			v_angle = v_angle * -1 == abs(v_angle) ? -PI / 2.f : PI / 2.f;
		}

		direction = glm::vec3(cos(v_angle) * sin(h_angle), sin(v_angle), cos(v_angle) * cos(h_angle));
		glm::vec3 right = glm::vec3(sin(h_angle - PI / 2.f), 0, cos(h_angle - PI / 2.f));
		glm::vec3 up = glm::cross(right, direction);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			position += direction * speed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			position -= direction * speed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			position += right * speed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			position -= right * speed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			speed = 20.f;
		}
		else {
			speed = 10.f;
		}

		glm::mat4 Projection = glm::perspective(45.f, w_width / w_height, 0.1f, 100.f);
		glm::mat4 View = glm::lookAt(position, position + direction, up);

		glClearColor(0.5f, 0.5f, 0.9f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();
		shaderProgram.setMat4("Projection", Projection);
		shaderProgram.setMat4("View", View);
		
		for (Cube* cube : blocks) {
			shaderProgram.setMat4("Model", cube->getModel());
			cube->draw();
		}

		window.display();
	}
	selection_thread.join();
}