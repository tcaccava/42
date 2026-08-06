#include "Math/Vector3.hpp"

Vector3::Vector3(){
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Vector3::Vector3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::operator+(const Vector3& other) const{
	return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const{
	return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vector3 Vector3::operator*(float scalar) const{
	return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vector3 Vector3::operator/(float scalar) const{
	if (scalar == 0)
    	return Vector3();
	return Vector3(this->x / scalar, this->y / scalar, this->z / scalar);
}

float Vector3::length() const{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3 Vector3::normalize() const{
	float len = this->length();
	if (len == 0.0f) 
		return Vector3(0.0f, 0.0f, 0.0f);
	return *this / len;
}

Vector3 Vector3::cross(const Vector3& other) const{
	return Vector3(
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x
	);
}

float Vector3::dot(const Vector3& other) const{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}