#include "Renderer.hpp"

Renderer::Renderer(){
}

Renderer::~Renderer(){
}

void Renderer::clear() const{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const Mesh& mesh) const{
	mesh.draw();
}