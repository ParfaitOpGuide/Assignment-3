#pragma once
namespace P6 {
	class MyVector
	{
	public:
		float x, y, z;

		MyVector() : x(0), y(0), z(0) {}
		MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

		/* sample operator changes
		MyVector operator+ (const MyVector v) {
			return MyVector(this->x + v.x, this->y + v.y, this->z + v.z);
		}

		void operator+= (const MyVector v) {
			this->x += v.x; this->y += v.y; this->z += v.z;
		}*/

		float Magnitude();
		MyVector Direction();

		MyVector Add(MyVector b);
		MyVector Subtract(MyVector b);

		MyVector ScalarMult(float b);
		MyVector Component(MyVector b);

		float ScalarProd(MyVector b);
		MyVector Cross(MyVector b);

		
	};
}