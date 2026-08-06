#include "Math/Matrix4.hpp"

Matrix4::Matrix4(){
	for(int i = 0; i < 16; i++)
        m[i] = 0;
}

Matrix4 Matrix4::identity(){
	Matrix4 m;

	for(int i = 0; i < 16; i++){
		m.m[i] = 0.0f;
	}
	m.m[0] = 1.0f;
	m.m[5] = 1.0f;
	m.m[10] = 1.0f;
	m.m[15] = 1.0f;
	return m;
}

const float* Matrix4::data() const{
	return m;
}

float& Matrix4::at(int row, int col){
	return m[col * 4 + row];
}

const float& Matrix4::at(int row, int col) const
{
    return m[col * 4 + row];
}

Matrix4 Matrix4::operator*(const Matrix4 &other) const{
	Matrix4 result;

	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			result.at(row, col) = 0.0f;
			for(int k = 0; k < 4; k++){
				result.at(row, col) += at(row, k) * other.at(k, col);
			}
		}
	}
	return result;
}

Vector3 Matrix4::operator*(const Vector3& other) const
{
    Vector3 result;

    result.x = at(0,0) * other.x + at(0,1) * other.y + at(0,2) * other.z + at(0,3);

    result.y = at(1,0) * other.x + at(1,1) * other.y + at(1,2) * other.z + at(1,3);

    result.z = at(2,0) * other.x + at(2,1) * other.y + at(2,2) * other.z + at(2,3);

    return result;
}

Matrix4 Matrix4::translation(const Vector3 &position){
	Matrix4 result = Matrix4::identity();

	result.at(0,3) = position.x;
	result.at(1,3) = position.y;
	result.at(2,3) = position.z;

	return result;
}

Matrix4 Matrix4::scale(const Vector3 &position){
	Matrix4 result = Matrix4::identity();

	result.at(0,0) = position.x;
	result.at(1,1) = position.y;
	result.at(2,2) = position.z;

	return result;
}

Matrix4 Matrix4::rotationX(const float angle){
	Matrix4 result = Matrix4::identity();

	float c = cos(angle);
	float s = sin(angle);

	result.at(1,1) = c;
	result.at(1,2) = -s;

	result.at(2,1) = s;
	result.at(2,2) = c;

	return result;
}

Matrix4 Matrix4::rotationY(const float angle){
	Matrix4 result = Matrix4::identity();

	float c = cos(angle);
	float s = sin(angle);

	result.at(0,0) = c;
    result.at(0,2) = s;

    result.at(2,0) = -s;
    result.at(2,2) = c;

    return result;
}

Matrix4 Matrix4::rotationZ(const float angle){
	Matrix4 result = Matrix4::identity();

	float c = cos(angle);
	float s = sin(angle);

	result.at(0,0) = c;
    result.at(0,1) = -s;

    result.at(1,0) = s;
    result.at(1,1) = c;

    return result;
}

Matrix4 Matrix4::perspective(float fov, float aspect, float near, float far){
	Matrix4 result;

	float f = 1.0f / tan(fov / 2.0f);

	result.at(0,0) = f / aspect;
	result.at(1,1) = f;
	result.at(2,2) = (far + near) / (near - far);
	result.at(2,3) = (2 * far * near) / (near - far);
	result.at(3,2) = -1;

	return result;
}

Matrix4 Matrix4::lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up){
	Matrix4 result = Matrix4::identity();

	Vector3 forward = (target - eye).normalize();
	Vector3 right = forward.cross(up).normalize();
	Vector3 realUp = right.cross(forward);

	result.at(0,0) = right.x;
	result.at(0,1) = right.y;
	result.at(0,2) = right.z;

	result.at(1,0) = realUp.x;
	result.at(1,1) = realUp.y;
	result.at(1,2) = realUp.z;

	result.at(2,0) = -forward.x;
	result.at(2,1) = -forward.y;
	result.at(2,2) = -forward.z;

	result.at(0,3) = -right.dot(eye);
	result.at(1,3) = -up.dot(eye);
	result.at(2,3) = forward.dot(eye);

	return result;
}

Matrix4 Matrix4::transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale) const{
	Matrix4 result = Matrix4::identity();

	result = result * Matrix4::translation(position);
	result = result * Matrix4::rotationX(rotation.x);
	result = result * Matrix4::rotationY(rotation.y);
	result = result * Matrix4::rotationZ(rotation.z);
	result = result * Matrix4::scale(scale);

	return result;
}