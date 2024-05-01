#pragma once
#include <math.h>


#define  Pi         3.1415926535f
#define TwoPi       Pi * 2.0f
#define PiOver2     Pi / 2.0f


#define VEC_EPSILON 0.000001f

#define Clamp01(x) \
	(x < 0.0f? 0.0f : (x > 1.0f? 1.0f : x))

#ifndef RAD2DEG
float RAD2DEG(float radian);
#endif

#ifndef DEG2RAD
float DEG2RAD(float degree);
#endif
float CorrectDegree(float degree);

inline void SinCos(float a, float& sina, float& cosa )
{
    sina = sinf(a);
    cosa = cosf(a);
} 

typedef struct vec2
{
	union 
	{
		struct 
		{
			float x;
			float y;
		};

		struct
		{
			float u;
			float v;
		};

		struct
		{
			float width;
			float height;
		};

		float asArray[2]; 
	};

	inline vec2() : x(0.0f), y(0.0f) {}
	inline vec2(float inX, float inY) : x(inX), y(inY) {}
	inline vec2(float* fv) : x(fv[0]), y(fv[1]) {}

	inline float& operator[](int i)
	{
		return asArray[i];
	}

} vec2;

typedef struct vec3
{
	union 
	{
		struct 
		{
			float x;
			float y;
			float z;
		};

		struct
		{
			float r;
			float g;
			float b;
		};

		struct
		{
			float width;
			float height;
			float depth;
		};

		float asArray[3]; 
	};

	inline vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	inline vec3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}
	inline vec3(float* fv) : x(fv[0]), y(fv[1]), z(fv[2]) {}

	inline float& operator[](int i)
	{
		return asArray[i];
	}

} vec3;

typedef struct vec4
{
	union 
	{
		struct 
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float asArray[4]; 
	};

	inline vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	inline vec4(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW) {}
	inline vec4(float* fv) : x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3]) {}

	inline float& operator[](int i)
	{
		return asArray[i];
	}

} vec4;

vec2 operator+(const vec2& a, const vec2& b);
vec3 operator+(const vec3& a, const vec3& b);

vec2 operator-(const vec2& a, const vec2& b);
vec3 operator-(const vec3& a, const vec3& b);

vec2 operator-(const vec2&  vec);
vec3 operator-(const vec3& vec);

vec2 operator*(const vec2& a, const vec2& b);
vec3 operator*(const vec3& a, const vec3& b);

vec2 operator*(const vec2& vec, float scalar);
vec3 operator*(const vec3& vec, float scalar);

vec2 operator*(float scalar,const vec2& vec);
vec3 operator*(float scalar,const vec3& vec);

vec2 operator/(const vec2& vec, float scalar);
vec3 operator/(const vec3& vec, float scalar);

bool operator==(const vec2& a, const vec2& b);
bool operator==(const vec3& a, const vec3& b);

bool operator!=(const vec2& a, const vec2& b);
bool operator!=(const vec3& a, const vec3& b);

float Dot(const vec2& a, const vec2& b);
float Dot(const vec3& a, const vec3& b);

vec2& operator+=(vec2& l, const vec2& r);
vec2& operator-=(vec2& l, const vec2& r); 

vec2& operator*=(vec2& l, const vec2& r) ;
vec2& operator*=(vec2& l, const float r) ;

vec2& operator/=(vec2& l, const vec2& r); 
vec2& operator/=(vec2& l, const float r);

vec3& operator+=(vec3& l, const vec3& r);
vec3& operator-=(vec3& l, const vec3& r); 

vec3& operator*=(vec3& l, const vec3& r) ;
vec3& operator*=(vec3& l, float r) ;

vec3& operator/=(vec3& l, const vec3& r); 
vec3& operator/=(vec3& l, float r);



float LengthSq(const vec2& vec);
float LengthSq(const vec3& vec);

float Length(const vec2& vec);
float Length(const vec3& vec);

void Normalize(vec2& vec);
vec2 Normalized(const vec2& vec);

void Normalize(vec3& vec);
vec3 Normalized(const vec3& vec);

vec3 Cross(const vec3& a, const vec3& b);

float Angle(const vec2& a, const vec2& b);
float Angle(const vec3& a, const vec3& b);

vec2 Project(const vec2& length, const vec2& direction);
vec3 Project(const vec3& length, const vec3& direction);

vec2 Perpendicular(const vec2& length, const vec2& direction);
vec3 Perpendicular(const vec3& length, const vec3& direction);

vec2 Reflect(const vec2& vec, const vec2& normal);
vec3 Reflect(const vec3& vec, const vec3& normal);

vec2 Lerp(const vec2& start, const vec2& end, float t);
vec3 Lerp(const vec3& start, const vec3& end, float t);

vec2 SLerp(const vec2& start, const vec2& end, float t);
vec3 SLerp(const vec3& start, const vec3& end, float t);

vec2 NLerp(const vec2& start, const vec2& end, float t);
vec3 NLerp(const vec3& start, const vec3& end, float t);

vec3 GetNormalFromTriangle(const vec3& a, const vec3& b, const vec3& c);

void Printvec2(const vec2& vec);
void Printvec3(const vec3& vec);
void PrintVec4(const vec4& vec);


