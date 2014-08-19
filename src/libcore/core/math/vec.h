#pragma once

#include <cassert>
#include <cmath>

namespace atom {

const float EPSILON = 0.01f;

//
// 2D Vector
//

template<typename T>
struct Vec2 {
  enum {
    SIZE = 2
  };

  typedef T value_type;

  Vec2() = default;

  Vec2(T x, T y)
  {
    data[0] = x;
    data[1] = y;
  }

  T& operator[](unsigned i)
  {
    assert(i < SIZE);
    return data[i];
  }

  const T& operator[](unsigned i) const
  {
    assert(i < SIZE);
    return data[i];
  }

  bool is_zero() const
  {
    return x == 0 && y == 0;
  }

  Vec2<T>& operator+=(const Vec2<T> &v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  union {
    struct {
      T x;
      T y;
    };
    T data[SIZE];
  };
};

template<typename T>
inline const Vec2<T> operator+(const Vec2<T> &a, const Vec2<T> &b)
{
  return Vec2<T>(a.x + b.x, a.y + b.y);
}

template<typename T>
inline const Vec2<T> operator-(const Vec2<T> &a, const Vec2<T> &b)
{
  return Vec2<T>(a.x - b.x, a.y - b.y);
}

template<typename T>
inline const Vec2<T> operator*(const Vec2<T> &a, T b)
{
  return Vec2<T>(a.x * b, a.y * b);
}

template<typename T>
inline const Vec2<T> operator*(T a, const Vec2<T> &b)
{
  return Vec2<T>(a * b.x, a * b.y);
}

template<typename T>
inline bool operator==(const Vec2<T> &a, const Vec2<T> &b)
{
  return a.x == b.x && a.y == b.y;
}

template<typename T>
inline bool operator!=(const Vec2<T> &a, const Vec2<T> &b)
{
  return !(a == b);
}

/**
 * Dot product operation for Vec2
 */
template<typename T>
inline T dot_product(
  const Vec2<T> &a,
  const Vec2<T> &b)
{ return a[0] * b[0] + a[1] * b[1]; }

template<typename T>
Vec2<T> normalized(const Vec2<T> &v)
{
  if (v[0] > EPSILON && v[1] > EPSILON) {
    T w = sqrt(v[0] * v[0] + v[1] * v[1]);
    return Vec2<T>(v[0] / w, v[1] / w);
  } else {
    return v;
  }
}


//
// 3D Vector
//

template<typename T>
struct Vec3 {
  enum {
    SIZE = 3
  };

  typedef T value_type;

  Vec3() = default;

  Vec3(T vx, T vy, T vz)
    : x(vx)
    , y(vy)
    , z(vz)
  {}

  static Vec3<T> x_axis()
  { return Vec3<T>(1, 0, 0); }

  static Vec3<T> y_axis()
  { return Vec3<T>(0, 1, 0); }

  static Vec3<T> z_axis()
  { return Vec3<T>(0, 0, 1); }

  T& operator[](unsigned i)
  {
    assert(i < SIZE);
    return data[i];
  }

  const T& operator[](unsigned i) const
  {
    assert(i < SIZE);
    return data[i];
  }

  Vec3<T>& operator+=(const Vec3<T> &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  T length() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  T length2() const
  {
    return x * x + y * y + z * z;
  }

  Vec3<T> normalized() const
  {
    T len = length();
    return Vec3(x / len, y / len, z / len);
  }

  union {
    struct {
      T x;
      T y;
      T z;
    };
    T data[SIZE];
  };
};

template<typename T>
Vec3<T> operator-(const Vec3<T> &a)
{
  return Vec3<T>(-a.x, -a.y, -a.z);
}

template<typename T>
Vec3<T> operator+(const Vec3<T> &a, const Vec3<T> &b)
{
  return Vec3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T>
Vec3<T> operator-(const Vec3<T> &a, const Vec3<T> &b)
{
  return Vec3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
Vec3<T> operator*(const Vec3<T> &a, const Vec3<T> &b)
{
  return Vec3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template<typename T>
Vec3<T> operator*(const Vec3<T> &v, T x)
{
  return Vec3<T>(v.x * x, v.y * x, v.z * x);
}

template<typename T>
Vec3<T> operator*(T x, const Vec3<T> &v)
{
  return Vec3<T>(v.x * x, v.y * x, v.z * x);
}

template<typename T>
Vec3<T> operator/(const Vec3<T> &v, T x)
{
  return Vec3<T>(v.x / x, v.y / x, v.z / x);
}

template<typename T>
inline bool operator==(const Vec3<T> &a, const Vec3<T> &b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

template<typename T>
inline bool operator!=(const Vec3<T> &a, const Vec3<T> &b)
{
  return !(a == b);
}

/**
 * Dot product operation for Vec3
 */
template<typename T>
inline T dot_product(
  const Vec3<T> &a,
  const Vec3<T> &b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

template<typename T>
inline T dot_product(
  T ax, T ay, T az,
  T bx, T by, T bz)
{
  return ax * bx + ay * by + az * bz;
}

/**
 * Skalarny sucin.
 */
template<typename T>
inline Vec3<T> cross_product(
  const Vec3<T> &a,
  const Vec3<T> &b)
{
  return Vec3<T>(
    a[1] * b[2] - a[2] * b[1],
    a[2] * b[0] - a[0] * b[2],
    a[0] * b[1] - a[1] * b[0]
  );
}


//
// 4D Vector
//

template<typename T>
struct Vec4 {
  enum {
    SIZE = 4
  };

  typedef T value_type;

  Vec4() = default;

  Vec4(T vx, T vy, T vz, T vw)
    : x(vx)
    , y(vy)
    , z(vz)
    , w(vw)
  {}

  Vec4(const Vec3<T> &v, T w)
  {
    data[0] = v[0];
    data[1] = v[1];
    data[2] = v[2];
    data[3] = w;
  }

  T& operator[](unsigned i)
  {
    assert(i < SIZE);
    return data[i];
  }

  const T& operator[](unsigned i) const
  {
    assert(i < SIZE);
    return data[i];
  }

  union {
    struct {
      T x;
      T y;
      T z;
      T w;
    };
    T data[SIZE];
  };
};

template<typename T>
Vec4<T> operator+(const Vec4<T> &a, const Vec4<T> &b)
{
  return Vec4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template<typename T>
Vec4<T> operator-(const Vec4<T> &a, const Vec4<T> &b)
{
  return Vec4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template<typename T>
Vec4<T> operator/(const Vec4<T> &v, T a)
{
  return Vec4<T>(v.x / a, v.y / a, v.z / a, v.w / a);
}

template<typename T>
Vec4<T> operator/=(Vec4<T> &v, T a)
{
  v.x /= a;
  v.y /= a;
  v.z /= a;
  v.w /= a;
  return v;
}

template<typename T>
inline bool operator==(const Vec4<T> &a, const Vec4<T> &b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template<typename T>
inline bool operator!=(const Vec4<T> &a, const Vec4<T> &b)
{
  return !(a == b);
}

}
