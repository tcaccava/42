#include "Input.hpp"

void Input::processInput(GLFWwindow *window, Transform& transform)
{
	float rotationSpeed = 2.0f * M_PI / 180.0f;
    float movementSpeed = 0.05f;

	//rotazione Y
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        transform.rotation.y += rotationSpeed;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        transform.rotation.y -= rotationSpeed;

    // rotazione X
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        transform.rotation.x += rotationSpeed;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        transform.rotation.x -= rotationSpeed;

	// rotazione Z
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        transform.rotation.z += rotationSpeed;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        transform.rotation.z -= rotationSpeed;

    // traslazione X
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transform.position.x -= movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transform.position.x += movementSpeed;

    // traslazione Y
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform.position.y += movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transform.position.y -= movementSpeed;

	// traslazione Z
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        transform.position.z += movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        transform.position.z -= movementSpeed;

	// reset
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        transform.position = Vector3(0.0f, 0.0f, 0.0f);
        transform.rotation = Vector3(0.0f, 0.0f, 0.0f);
        transform.scale = Vector3(1.0f, 1.0f, 1.0f);
    }

	// close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
    	glfwSetWindowShouldClose(window, true);
	}
}

bool Input::isKeyPressed(GLFWwindow* window, int key)
{
    bool current = glfwGetKey(window, key) == GLFW_PRESS;
    bool previous = _previousKeys[key];

    _previousKeys[key] = current;

    return current && !previous;
}

Input::Input(){
	for (int i = 0; i <= GLFW_KEY_LAST; i++)
        _previousKeys[i] = false;
}

Input::~Input(){
}