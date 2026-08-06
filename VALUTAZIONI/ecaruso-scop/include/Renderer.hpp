#pragma once

#include "Mesh.hpp"

#include <glad/glad.h>

class Renderer {
	public:
		Renderer();
		~Renderer();

		void clear() const;
		void draw(const Mesh& mesh) const;
};