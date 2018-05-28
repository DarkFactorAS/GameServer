#ifndef EVILENGINE_CORELIB_COREMATH_MATRIX
#define EVILENGINE_CORELIB_COREMATH_MATRIX 1
#define _USE_MATH_DEFINES 1

#include "Vector3.h"

class Matrix
{
public:

  Matrix();
  Matrix( const float other[16] );
  Matrix( const Vector3& vec );
  Matrix( 
    const float f11, const float f12, const float f13, const float f14,
    const float f21, const float f22, const float f23, const float f24,
    const float f31, const float f32, const float f33, const float f34,
    const float f41, const float f42, const float f43, const float f44 );

  void Identity();
  bool IsIdentity() const;

  void Perspective( float fov, float aspect, float zNear, float zFar );
  void Ortographic( float left, float right, float top, float bottom, float near, float far);

  //void Rotate( float degAngle, const Vector3& vec );

  // Translate functions
  Vector3 GetTranslation();

  void Translate(float x, float y, float z);
  void Translate(float x, float y, float z, Matrix* dst) const;
  void Translate(const Vector3& t);
  void Translate(const Vector3& t, Matrix* dst) const;
  static void CreateTranslation(const Vector3& translation, Matrix* dst);
  static void CreateTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix* dst);

  void Rotate(const Vector3& axis, float angle);
  void Rotate(const Vector3& axis, float angle, Matrix* dst) const;
  void RotateY(float angle);
  void RotateY(float angle, Matrix* dst) const;
  static void CreateRotation(const Vector3& axis, float angle, Matrix* dst);
  static void CreateRotationY(float angle, Matrix* dst);

  void Scale( const Vector3& scale );
  inline static void Scale(const float* m, const Vector3& scale, float* dst);

  // Multiply
  void Multiply(float scalar);
  void Multiply(float scalar, Matrix* dst) const;
  static void Multiply(const Matrix& m, float scalar, Matrix* dst);
  void Multiply(const Matrix& m);
  static void Multiply(const Matrix& m1, const Matrix& m2, Matrix* dst);
  inline static void MultiplyMatrix(const float* m, float scalar, float* dst);
  inline static void MultiplyMatrix(const float* m1, const float* m2, float* dst);

  static Vector3 Multiply( const Matrix& mat, const Vector3& vec );

  void Set( float other[16] );

  //String ToString();

  static void Translate( const Matrix& sourceMatrix, Matrix& destMatrix, float xTranslation, float yTranslation, float zTranslation );
  static void LookAt( Matrix& viewMatrix, const Vector3& eye, const Vector3& center, const Vector3& up );

  //static Matrix Inverse( const Matrix& originalMatrix );
  void Invert();

  float m[16];
};

float MatDeterminant( const Matrix& a );
const Matrix MatInverse( const Matrix& a );


#endif // EVILENGINE_CORELIB_COREMATH_MATRIX