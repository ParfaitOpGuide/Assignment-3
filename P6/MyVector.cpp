#include "MyVector.h"
#include <math.h>

using namespace P6;

float MyVector::Magnitude() {
	float mag = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	return mag;
}

MyVector MyVector::Direction() {
	float c1, c2, c3;
	c1 = this->x * this->Magnitude();
	c2 = this->y * this->Magnitude();
	c3 = this->z * this->Magnitude();

	return { c1,c2,c3 };
}

MyVector MyVector::Add(MyVector b) {
	this->x += b.x;
	this->y += b.y;
	this->z += b.z;
	return { this->x,this->y,this->z };
}

MyVector MyVector::Subtract(MyVector b) {
	this->x -= b.x;
	this->y -= b.y;
	this->z -= b.z;
	return { this->x,this->y,this->z };
}

MyVector MyVector::ScalarMult(float b) {
	this->x *= b;
	this->y *= b;
	this->z *= b;

	return { this->x,this->y,this->z };
}

MyVector MyVector::Component(MyVector b) {
	float c1, c2, c3;
	c1 = (this->x * b.x);
	c2 = (this->y * b.y);
	c3 = (this->z * b.z);
	return { c1,c2,c3 };
}

float MyVector::ScalarProd(MyVector b) {
	float dot = (this->x * b.x) + (this->y * b.y) + (this->z * b.z);
	return dot;
}

MyVector MyVector::Cross(MyVector b) {
	float c1, c2, c3;
	c1 = (this->y * b.z) - (this->z * b.y);
	c2 = (this->z * b.x) - (this->x * b.z);
	c3 = (this->x * b.y) - (this->y * b.x);

	return { c1,c2,c3 };
}