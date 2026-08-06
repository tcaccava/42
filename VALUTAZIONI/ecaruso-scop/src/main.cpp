#include "Input.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Vector3.hpp"
#include "ObjLoader.hpp"
#include "Transform.hpp"
#include "Texture.hpp"
#include <vector>
#include <iostream>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <path_to_obj_file> <path_to_texture.ppm>" << std::endl;
		return 1;
	}

	bool is42 = false;

	std::string path = argv[1];

	if (path.find("42.obj") != std::string::npos)
	{
	    is42 = true;
	}

    Input input;

    Window window(1920, 1080, "SCOP");

	Transform transform;
	Transform logoTransform;

	logoTransform.position = Vector3(-1.45f, 0.95f, 0.0f);
	logoTransform.scale = Vector3(0.25f, 0.25f, 0.25f);
	
    if (!window.isValid()){
		return 1;
    }
	
    glfwMakeContextCurrent(window.getNativeWindow());
	
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		std::cerr << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

	glEnable(GL_DEPTH_TEST);
	
	Renderer renderer;

	ObjLoader loader(argv[1]);

	ObjLoader logoLoader("assets/models/42.obj");

	if (!logoLoader.isValid())
	{
	    std::cerr << "Failed to load logo OBJ file" << std::endl;
	    return 1;
	}

	Mesh logo(logoLoader.getVertices(), logoLoader.getIndices());

	if (!loader.isValid()){
		std::cerr << "Failed to load OBJ file" << std::endl;
    	return 1;
	}
	
	Mesh object(loader.getVertices(), loader.getIndices());
	
    Shader shader("shaders/basic.vert", "shaders/basic.frag");
	
	Texture texture(argv[2]);

	if(!texture.isValid()){
		std::cerr << "Failed to load texture" << std::endl;
		return 1;
	}

	transform.rotation.y = 90.0f * M_PI / 180.0f;

	Matrix4 view = Matrix4::lookAt(
	    Vector3(0.0f, 0.0f, 3.0f),
	    Vector3(0.0f, 0.0f, 0.0f),
	    Vector3(0.0f, 1.0f, 0.0f)
	);

	Matrix4 projection = Matrix4::perspective(
	    45.0f * M_PI / 180.0f,
	    800.0f / 600.0f,
	    0.1f,
	    100.0f
	);

	bool textureEnabled = false;
	float textureBlend = 0.0f;
	float lastTime = glfwGetTime();

    while (!window.shouldClose())
    {
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		logoTransform.rotation.y += deltaTime;

        input.processInput(window.getNativeWindow(), transform);

		renderer.clear();

		shader.use();

		shader.setBool("useLighting", is42);

		shader.setMatrix4("model", transform.getMatrix());
		shader.setMatrix4("view", view);
		shader.setMatrix4("projection", projection);

		texture.bind();

		if (input.isKeyPressed(window.getNativeWindow(), GLFW_KEY_T))
		{
   			textureEnabled = !textureEnabled;
		}

		float speed = 0.5f;

	if (textureEnabled)
	{
    	textureBlend += speed * deltaTime;

    	if (textureBlend > 1.0f)
    	    textureBlend = 1.0f;
	}
	else
	{
    	textureBlend -= speed * deltaTime;

    	if (textureBlend < 0.0f)
    	    textureBlend = 0.0f;
	}
		shader.setFloat("textureBlend", textureBlend);
		renderer.draw(object);

		shader.setMatrix4("model", logoTransform.getMatrix());
		shader.setBool("useLighting", true);
		shader.setFloat("textureBlend", 0.0f);

		renderer.draw(logo);

		window.update();
    }

    return 0;
}