#ifndef luciconnect_GEOMETRY_H
#define luciconnect_GEOMETRY_H

#define EPS 0.00001

#include <string>
#include <float.h> // FLT_MIN
#include <math.h> // sqrt

namespace luciconnect {
  /**
   * 2D vector functions
   */
  typedef struct vec2 {
    float x;
    float y;

    vec2 operator*(const float scalar) {
      return {x*scalar, y*scalar};
    }

    vec2 operator/(const float scalar) {
      return {x/scalar, y/scalar};
    }

    vec2 operator-(const vec2 other) {
      return {x-other.x, y-other.y};
    }

    vec2 operator+(const vec2 other) {
      return {x+other.x, y+other.y};
    }

    float operator*(const vec2 other) {
      return x*other.x + y*other.y;
    }

    bool operator==(const vec2 other) {
      return x==other.x && y==other.y;
    }

    operator std::string() const {
      return "{" + std::to_string(x) + "," + std::to_string(y) + "}";
    }
  } vec2;

  inline float abs(float x) {
    return x > 0 ? x : -x;
  }

  inline float abs(vec2 p) {
    return sqrt(p*p);
  }

  inline float ccw(vec2 p1, vec2 p2, vec2 p3) {
    return (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
  }

  /**
   * 3D Vector functions
   */
  typedef struct vec3 {
    float x;
    float y;
    float z;

    vec3 operator*(const float scalar) {
      return {x*scalar, y*scalar, z*scalar};
    }

    vec3 operator/(const float scalar) {
      return {x/scalar, y/scalar, z/scalar};
    }

    vec3 operator-(const vec3 other) {
      return {x-other.x, y-other.y, z-other.z};
    }

    vec3 operator+(const vec3 other) {
      return {x+other.x, y+other.y, z+other.z};
    }

    float operator*(const vec3 other) {
      return x*other.x + y*other.y + z*other.z;
    }

    bool operator==(const vec3 other) {
      return abs(x-other.x) < EPS && abs(y-other.y) < EPS && abs(z-other.z) < EPS;
    }

    operator std::string() const {
      return "{" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "}";
    }
  } vec3;

  inline float abs(vec3 p) {
    return sqrt(p*p);
  }

  typedef struct mat4 {
    float data[16];
  } mat4;
}

#endif
