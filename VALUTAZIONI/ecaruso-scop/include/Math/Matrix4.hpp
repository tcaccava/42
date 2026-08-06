#pragma once

#include "Vector3.hpp"

#include <cmath>

class Matrix4 {
	private:
		float m[16];

	public:
		Matrix4();
		static Matrix4 identity();

		const float* data() const;

		float& at(int row, int col);
		const float& at(int row, int col) const;

		Matrix4 operator*(const Matrix4 &other) const;
		Vector3 operator*(const Vector3 &other) const;

		static Matrix4 translation(const Vector3 &position);
		static Matrix4 scale(const Vector3 &position);

		static Matrix4 rotationX(const float angle);
		static Matrix4 rotationY(const float angle);
		static Matrix4 rotationZ(const float angle);

		static Matrix4 perspective(float fov, float aspect, float near, float far);
		static Matrix4 lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);

		//in motori 3D seri questa funzione è una classe
		Matrix4 transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale) const;
};