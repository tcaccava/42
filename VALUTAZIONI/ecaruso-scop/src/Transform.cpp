#include "Transform.hpp"

Transform::Transform() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

Matrix4 Transform::getMatrix() const {
    Matrix4 matrix = Matrix4::identity();

	matrix = Matrix4::translation(position);

	matrix = matrix * Matrix4::rotationX(rotation.x);
	matrix = matrix * Matrix4::rotationY(rotation.y);
	matrix = matrix * Matrix4::rotationZ(rotation.z);

	matrix = matrix * Matrix4::scale(scale);

    return matrix;
}