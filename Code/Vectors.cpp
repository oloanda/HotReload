#include "Vectors.h"
#include <stdio.h>

float CorrectDegree(float degree)
{
	while(degree > 360.0f)
	{
		degree -= 360.0f;
	}
	while(degree < -360.0f)
	{
		degree += 360.0f;
	}
	return degree;
}

#ifndef RAD2DEG
float RAD2DEG(float radian)
{
	float degree = radian * 57.295754f;
	degree = CorrectDegree(degree);
	return degree;
}
#endif

#ifndef DEG2RAD
float DEG2RAD(float degree)
{
	degree = CorrectDegree(degree);
	float radian = degree * 0.0174533f;
	return radian;
}
#endif

vec2 operator+(const vec2& a, const vec2& b)
{
	return vec2(a.x + b.x, a.y + b.y);
}

vec3 operator+(const vec3& a, const vec3& b)
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec2 operator-(const vec2& a, const vec2& b)
{
	return vec2(a.x - b.x, a.y - b.y);
}

vec3 operator-(const vec3& a, const vec3& b)
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec2 operator-(const vec2&  vec)
{
	return vec2(-vec.x, -vec.y);
}

vec3 operator-(const vec3& vec)
{
	return vec3(-vec.x, -vec.y, -vec.z);
}

vec2 operator*(const vec2& a, const vec2& b)
{
	return vec2(a.x * b.x, a.y * b.y);
}

vec3 operator*(const vec3& a, const vec3& b)
{
	return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

vec2 operator*(const vec2& vec, float scalar)
{
	return vec2(vec.x * scalar, vec.y * scalar);
}

vec3 operator*(const vec3& vec, float scalar)
{
	return vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

vec2 operator*(float scalar,const vec2& vec)
{
	return vec2(vec.x * scalar, vec.y * scalar);
}

vec3 operator*(float scalar,const vec3& vec)
{
	return vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

vec2 operator/(const vec2& vec, float scalar)
{
	float s = 1.0f / scalar;
	return vec2(vec.x * s, vec.y  * s);
}

vec3 operator/(const vec3& vec, float scalar)
{
	float s = 1.0f / scalar;
	return vec3(vec.x * s, vec.y * s, vec.z * s);
}

bool operator==(const vec2& a, const vec2& b)
{
	vec2 diff(b - a);
	float lengSq = diff.x * diff.x + diff.y * diff.y; 
	return lengSq < VEC_EPSILON;
}

bool operator==(const vec3& a, const vec3& b)
{
	vec3 diff(b - a);
	float lengSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z; 
	return lengSq < VEC_EPSILON;
}

bool operator!=(const vec2& a, const vec2& b)
{
	return !(a == b);
}

bool operator!=(const vec3& a, const vec3& b)
{
	return !(a == b);
}

vec2& operator+=(vec2& l, const vec2& r)
{
	l.x += r.x;
	l.y += r.y;
	return l;
}

vec2& operator-=(vec2& l, const vec2& r) 
{
	l.x -= r.y;
	l.y -= r.y;
	return l;
}

vec2& operator*=(vec2& l, const vec2& r) 
{
	l.x *= r.x;
	l.y *= r.y;
	return l;
}

vec2& operator*=(vec2& l, const float r) 
{
	l.x *= r;
	l.y *= r;
	return l;
}

vec2& operator/=(vec2& l, const vec2& r) 
{
	l.x /= r.x;
	l.y /= r.y;
	return l;
}

vec2& operator/=(vec2& l, const float r) 
{
	l.x /= r;
	l.y /= r;
	return l;
}

vec3& operator+=(vec3& l, const vec3& r) 
{
	l.x += r.x;
	l.y += r.y;
	l.z += r.z;
	return l;
}

vec3& operator-=(vec3& l, const vec3& r) 
{
	l.x -= r.x;
	l.y -= r.y;
	l.z -= r.z;
	return l;
}

vec3& operator*=(vec3& l, const vec3& r) 
{
	l.x *= r.x;
	l.y *= r.y;
	l.z *= r.z;
	return l;
}

vec3& operator*=(vec3& l, float r) 
{
	l.x *= r;
	l.y *= r;
	l.z *= r;
	return l;
}

vec3& operator/=(vec3& l, const vec3& r) 
{
	l.x /= r.x;
	l.y /= r.y;
	l.z /= r.z;
	return l;
}

vec3& operator/=(vec3& l, float r) 
{
	l.x /= r;
	l.y /= r;
	l.z /= r;
	return l;
}

float Dot(const vec2& a, const vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

float Dot(const vec3& a, const vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float LengthSq(const vec2& vec)
{
	return Dot(vec,vec);
}

float LengthSq(const vec3& vec)
{
	return Dot(vec,vec);
}

float Length(const vec2& vec)
{
	float lengSq = vec.x * vec.x + vec.y * vec.y;
	if(lengSq < VEC_EPSILON)
	{
		return 0.0f;
	}

	return sqrtf(lengSq);
}

float Length(const vec3& vec)
{
	float lengSq = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	if(lengSq < VEC_EPSILON)
	{
		return 0.0f;
	}
	return sqrtf(lengSq);
}

void Normalize(vec2& vec)
{
	float lengSq = vec.x * vec.x + vec.y * vec.y;
	if(lengSq < VEC_EPSILON)
	{
		return;
	}
	float invLen = 1.0f/ sqrtf(lengSq);
	vec.x *=invLen;
	vec.y *=invLen;
}

vec2 Normalized(const vec2& vec)
{
	float lengSq = vec.x * vec.x + vec.y * vec.y;
	if(lengSq < VEC_EPSILON)
	{
		return vec2();
	}
	float invLen = 1.0f/ sqrtf(lengSq);
	return vec2(
		vec.x * invLen,
		vec.y * invLen
	);

}

void Normalize(vec3& vec)
{
	float lengSq = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	if(lengSq < VEC_EPSILON)
	{
		return;
	}
	float invLen = 1.0f/ sqrtf(lengSq);
	vec.x *=invLen;
	vec.y *=invLen;
	vec.z *=invLen;
}

vec3 Normalized(const vec3& vec)
{
	float lengSq = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	if(lengSq < VEC_EPSILON)
	{
		return vec3();
	}
	float invLen = 1.0f/ sqrtf(lengSq);
	return vec3(
		vec.x * invLen,
		vec.y * invLen,
		vec.z * invLen
		);
}

vec3 Cross(const vec3& a, const vec3& b)
{
	return vec3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

float Angle(const vec2& a, const vec2& b)
{
	float sqLengthA  = a.x * a.x + a.y * a.y;
	float sqLengthB	 = b.x * b.x + b.y * b.y;
	if(sqLengthA < VEC_EPSILON || sqLengthB < VEC_EPSILON)
	{
		return 0.0f;
	}

	float length = sqrtf(sqLengthA * sqLengthB);
	return acosf(Dot(a, b) / length);

}

float Angle(const vec3& a, const vec3& b)
{
	float sqLengthA  = a.x * a.x + a.y * a.y + a.z * a.z;
	float sqLengthB	 = b.x * b.x + b.y * b.y + b.z * b.z;
	if(sqLengthA < VEC_EPSILON || sqLengthB < VEC_EPSILON)
	{
		return 0.0f;
	}

	float length = sqrtf(sqLengthA * sqLengthB);
	return acosf(Dot(a, b) / length);
}

vec2 Project(const vec2& length, const vec2& direction)
{
	float lengSq =  LengthSq(direction);
	if(lengSq < VEC_EPSILON)
	{
		return vec2();
	}
	float dot = Dot(length, direction);
	return direction * (dot / lengSq);
}

vec3 Project(const vec3& length, const vec3& direction)
{
	float lengSq =  LengthSq(direction);
	if(lengSq < VEC_EPSILON)
	{
		return vec3();
	}
	float dot = Dot(length, direction);
	return direction * (dot / lengSq);
}

vec2 Perpendicular(const vec2& length, const vec2& direction)
{
	float lengSq =  LengthSq(direction);
	if(lengSq < VEC_EPSILON)
	{
		return vec2();
	}
	return length - Project(length, direction);
}

vec3 Perpendicular(const vec3& length, const vec3& direction)
{
	float lengSq =  LengthSq(direction);
	if(lengSq < VEC_EPSILON)
	{
		return vec3();
	}
	return length - Project(length, direction);
}

vec2 Reflect(const vec2& vec, const vec2& normal)
{
	float length =  Length(normal);
	if(length < VEC_EPSILON)
	{
		return vec2();
	}
	float scale = Dot(vec, normal) / length;
	vec2 proj =  normal * scale * 2;
	return vec - proj;
}

vec3 Reflect(const vec3& vec, const vec3& normal)
{
	float length =  Length(normal);
	if(length < VEC_EPSILON)
	{
		return vec3();
	}
	float scale = Dot(vec, normal) / length;
	vec3 proj =  normal * scale * 2;
	return vec - proj;
}

vec2 Lerp(const vec2& start, const vec2& end, float t)
{
	return vec2(
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t
	);
}

vec3 Lerp(const vec3& start, const vec3& end, float t)
{
	return vec3(
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t,
		start.z + (end.z - start.z) * t
	);
}

vec2 SLerp(const vec2& start, const vec2& end, float t)
{
	if(t < 0.01f)
	{
		return Lerp(start, end, t);
	}

	vec2 from = Normalized(start);
	vec2 to = Normalized(end);

	float theta = Angle(from, to);
	float sinTheta = sinf(theta);

	float a = sinf((1.0f - t) * theta) / sinTheta;
	float b = sinf(t * theta) / sinTheta;

	return from * a + to * b;
}

vec3 SLerp(const vec3& start, const vec3& end, float t)
{
	if(t < 0.01f)
	{
		return Lerp(start, end, t);
	}

	vec3 from = Normalized(start);
	vec3 to = Normalized(end);

	float theta = Angle(from, to);
	float sinTheta = sinf(theta);

	float a = sinf((1.0f - t) * theta) / sinTheta;
	float b = sinf(t * theta) / sinTheta;

	return from * a + to * b;
}

vec2 NLerp(const vec2& start, const vec2& end, float t)
{
	vec2 linear(
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t
	);
	return Normalized(linear);
}

vec3 NLerp(const vec3& start, const vec3& end, float t)
{
	vec3 linear(
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t,
		start.z + (end.z - start.z) * t
	);
	return Normalized(linear);
}

vec3 GetNormalFromTriangle(const vec3& a, const vec3& b, const vec3& c)
{
	vec3 result;
	vec3 v1 = vec3(b.x - a.x, b.y - a.y, b.z - a.z);
	vec3 v2 = vec3(c.x - b.x, c.y - b.y, c.z - b.z);

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}

void Printvec2(const vec2& vec)
{
	printf("(%.6f, %.6f)\n", vec.x, vec.y);
}

void Printvec3(const vec3& vec)
{
	printf("(%.6f, %.6f, %.6f)\n", vec.x, vec.y, vec.z);
}

void PrintVec4(const vec4& vec)
{
    printf("(%.6f; %.6f; %.6f; %.6f)\n", vec.x, vec.y, vec.z, vec.w);
}