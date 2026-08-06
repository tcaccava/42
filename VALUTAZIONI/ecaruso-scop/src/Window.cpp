#include "Window.hpp"

    Window::Window(unsigned int width, unsigned int height, const std::string& title){
		_valid = false;

		this->_width = width;
		this->_height = height;
		this->_title = title;

		if (!glfwInit()){
        	std::cerr << "Failed to initialize GLFW" << std::endl;
        	return;
    	}

    	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!_window)
    	{
        	std::cerr << "Failed to create window\n";
        	glfwTerminate();
        	return;
    	}
    	_valid = true;
	}

	Window::~Window(){
		if (_window){
			glfwDestroyWindow(_window);
		}
		glfwTerminate();
	}

	GLFWwindow* Window::getNativeWindow() const{
		return _window;
	}

	void Window::update()
	{
    	glfwSwapBuffers(_window);
    	glfwPollEvents();
	}

	bool Window::shouldClose() const
	{
    	return glfwWindowShouldClose(_window);
	}

	unsigned int Window::getWidth() const {
		return _width;
	}

	unsigned int Window::getHeight() const {
		return _height;
	}
	
	int Window::isValid() const {
		return _valid;
	}