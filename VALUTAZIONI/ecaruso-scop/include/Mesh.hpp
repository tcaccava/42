#pragma once

#include <glad/glad.h>
#include <vector>
#include <cstddef>

#include "Vertex.hpp"

class Mesh{
	private:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;

		unsigned int _VAO;
		unsigned int _VBO;
		unsigned int _EBO;

	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		~Mesh();

		void draw() const;
};