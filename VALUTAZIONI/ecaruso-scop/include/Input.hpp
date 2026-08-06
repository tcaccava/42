#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Transform.hpp"

#include <iostream>

class Input {
    private:
		bool _previousKeys[GLFW_KEY_LAST + 1];
    public:
        Input();
        ~Input();

        static void processInput(GLFWwindow *window, Transform& transform);
		bool isKeyPressed(GLFWwindow* window, int key);
};