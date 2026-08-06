#pragma once

#include "Vertex.hpp"

#include <vector>
#include <string>

class ObjLoader{
	private:
		bool _valid;

    	std::vector<Vertex> _vertices;
    	std::vector<unsigned int> _indices;

		std::vector<Vector3> _positions;
		std::vector<Vector2> _texCoords;
		std::vector<Vector3> _normals;

		struct FaceVertex
		{
			int position;
			int texCoord;
			int normal;

			FaceVertex() : position(-1), texCoord(-1), normal(-1){
			}
		};

		std::vector<FaceVertex> _faceVertices;

		void parseVertex(std::stringstream& ss);
		void parseTexCoord(std::stringstream& ss);
		void parseNormal(std::stringstream& ss);
		void parseFace(std::stringstream& ss);
		FaceVertex parseFaceVertex(const std::string &token);
		void generateMissingUVs();

		void calculateNormals();
		bool buildVertices();
		Vector2 calculateBoxUV(const Vector3& position, const Vector3& normal);
		bool normalizeModel();

	public:
    	ObjLoader(const std::string& path);

    	const std::vector<Vertex>& getVertices() const;
    	const std::vector<unsigned int>& getIndices() const;
		bool isValid() const;
};