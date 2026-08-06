#pragma once

#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"

class Transform{
	public:
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;

		Transform();

		Matrix4 getMatrix() const;
};