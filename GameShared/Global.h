#pragma once
#include <stdint.h>
#include <memory>
#include <cmath>
#include <assert.h>
#include <string>
#include "Types.h"
#undef min
#undef max

constexpr auto S_TICKS_PER_SECOND = 6;
constexpr auto S_TICK_DURATION(1.0f / S_TICKS_PER_SECOND);

constexpr auto MAX_PLAYERS = 256;
constexpr auto MAX_NPCS = 256;
constexpr auto MAX_ENTITIES = (MAX_PLAYERS + MAX_NPCS); // Includes players

//#ifdef WIN32
#define PACK #pragma pack(1)
//( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
//#else
//#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
//#endif

static const int directionDeltaX[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
static const int directionDeltaY[] = { 1, 1, 0, -1, -1, -1, 0, 1 };

struct rgba
{
	u8 r, g, b, a;
};

template <typename T>
struct vec2
{
	T x;
	T y;

	vec2(T x, T y)
		: x(x), y(y) { }
	vec2() : x(0), y(0) { }

	vec2<T> operator*(const T& rhs) const { return vec2<T>((T)(x * rhs), (T)(y * rhs)); }
	vec2<T> operator/(const T& rhs) const { return vec2<T>((T)(x / rhs), (T)(y / rhs)); }
	vec2<T> operator-(const vec2<T>& rhs) const { return vec2<T>((T)(x - rhs.x), (T)(y - rhs.x)); }
	vec2<T> operator+(const vec2<T>& rhs) const { return vec2<T>((T)(x + rhs.x), (T)(y + rhs.x)); }

	bool operator==(const vec2<T>& rhs) const { return x == rhs.x && y == rhs.y; };
	bool operator!=(const vec2<T>& rhs) const { return x != rhs.x || y != rhs.y; };

	float length() const
	{
		return std::sqrtf(std::powf((float)x, 2) + std::powf((float)x, 2));
	}

	void normalize()
	{
		const float length = std::sqrtf(std::powf((float)x, 2) + std::powf((float)x, 2));
		if (length == 0)
			return;
		x /= length;
		y /= length;
	}
};


typedef vec2<float> vec2f;
static float distanceFromTo(const vec2f& from, const vec2f& to)
{
	float dx = to.x - from.x, dy = to.y - from.y;
	return std::sqrtf(dx * dx + dy * dy);
}

// vec2s
typedef vec2<u16> vec2s;
static u32 distanceFromTo(const vec2s& from, const vec2s& to)
{
	u16 dx = (u16)to.x - from.x, dy = (u16)to.y - from.y;
	return (u32)std::sqrt(dx * dx + dy * dy);
}

static u8 directionFromTo(const vec2s& from, const vec2s& to)
{
	if (to.y > from.y)
		if (to.x < from.x)
			return 3;
		else if (to.x > from.x)
			return 1;
		else return 2;
	else if (to.y < from.y)
		if (to.x < from.x)
			return 5;
		else if (to.x > from.x)
			return 7;
		else return 6;
	else if (to.x > from.x)
		return 0;
	else
		return 4;
}

template <typename T>
struct vec3
{
	T x, y, z;

	vec3(T x, T y, T z)
		: x(x), y(y), z(z) { }
	vec3() : x(0), y(0), z(0) { }

	vec3<T> operator*(const T& rhs) { return vec3((T)(x * rhs), (T)(y * rhs), (T)(z * rhs)); }
	vec3<T> operator/(const T& rhs) { return vec3((T)(x / rhs), (T)(y / rhs), (T)(z * rhs)); }
	vec3<T> operator/=(const T& rhs) { *this = *this / rhs; return *this; }
	vec3<T> operator-(const vec3<T>& rhs) { return vec3((T)(x - rhs.x), (T)(y - rhs.x), (T)(z - rhs.z)); }
	vec3<T> operator+(const vec3<T>& rhs) { return vec3((T)(x + rhs.x), (T)(y + rhs.x), (T)(z + rhs.z)); }
	vec3<T>& operator+=(const vec3<T>& rhs) { *this = *this + rhs; return *this; }
	bool operator==(const vec3<T>& rhs) { return x == rhs.x && y == rhs.y && z == rhs.z; }
	bool operator!=(const vec3<T>& rhs) { return x != rhs.x || y != rhs.y || z != rhs.z; }

	float length()
	{
		return sqrtf(x * x + y * y + z * z);
	}
};
typedef vec3<float> vec3f;

inline float lerp(const float& x1, const float& x2, const float& t)
{
	return x1 + (x2 - x1) * t;
}

inline vec2f lerp(const vec2f& v1, const vec2f& v2, const float& t)
{
	return v1 + (v2 - v1) * t;
}

inline vec2f lerp(const vec2s& v1, const vec2s& v2, const float& t)
{
	return vec2f(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t);
}

enum Region : u8
{
	R_Overworld = 0,
	R_Cave1 = 1,
	R_Cave2 = 2,
	NUM_REGIONS
};

template<typename T>
static void printBytes(const T& object)
{
	const u8* const buff = reinterpret_cast<const u8*>(&object);
	for (size_t i = 0; i < sizeof(T); i++)
		printf("0x%02x ", buff[i]);
	printf("\n");
}

static void printBytes(const void* object, const size_t size, const size_t offset = 0)
{
	for (size_t i = offset; i < offset + size; i++)
		printf("0x%02x ", reinterpret_cast<const u8*>(object)[offset + i]);
	printf("\n");
}
