#pragma once

#include <iostream>
#include <GL/glew.h>

extern float w_width;
extern float w_height;

const float PI = 3.14f;

template<class T>
void print(T x, bool endline = true) {
	if (endline) {
		std::cout << x << std::endl;
	}
	else {
		std::cout << x;
	}
}