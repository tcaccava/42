#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

class Window{
    private:
        GLFWwindow* _window;

		unsigned int _width;
		unsigned int _height;
		std::string _title;

		bool _valid;

	public:
		Window(unsigned int width, unsigned int height, const std::string& title);
		~Window();

		GLFWwindow* getNativeWindow() const;
		void update();
		bool shouldClose() const;

		unsigned int getWidth() const;
		unsigned int getHeight() const;
		int isValid() const;
};