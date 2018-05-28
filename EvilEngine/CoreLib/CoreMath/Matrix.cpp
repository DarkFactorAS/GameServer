
#include "Precompile.h"
#include "Matrix.h"
#include <math.h>
#include <string.h>

#define MATRIX_SIZE ( sizeof(float) * 16)

static const float MATRIX_IDENTITY[16] =
{
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
};

Matrix::Matrix()
{
  memcpy(m, MATRIX_IDENTITY, MATRIX_SIZE);
}

Matrix::Matrix( const float other[16] )
{
  memcpy(m, other, MATRIX_SIZE);
}

void Matrix::Set( float other[16] )
{
  memcpy(m, other, MATRIX_SIZE);
}

Matrix::Matrix( const Vector3& other )
{
  memcpy(m, MATRIX_IDENTITY, MATRIX_SIZE);

  m[12] = other.x;
  m[13] = other.y;
  m[14] = other.z;
}

Matrix::Matrix( 
  const float f11, const float f12, const float f13, const float f14,
  const float f21, const float f22, const float f23, const float f24,
  const float f31, const float f32, const float f33, const float f34,
  const float f41, const float f42, const float f43, const float f44
  )
{
  m[0]  = f11; m[1]  = f12; m[2]  = f13; m[3]  = f14;
  m[4]  = f21; m[5]  = f22; m[6]  = f23; m[7]  = f24;
  m[8]  = f31; m[9]  = f32; m[10] = f33; m[11] = f34;
  m[12] = f41; m[13] = f42; m[14] = f43; m[15] = f44;
}

/*
String Matrix::ToString()
{
  return String::FormatString( "Matrix[\n%0.2f %0.2f %0.2f %0.2f\n%0.2f %0.2f %0.2f %0.2f\n%0.2f %0.2f %0.2f %0.2f\n%0.2f %0.2f %0.2f %0.2f]\n", 
    m[0], m[1], m[2], m[3],
    m[4], m[5], m[6], m[7],
    m[8], m[9], m[10], m[11],
    m[12], m[13], m[14], m[15]
  );
}
*/

void Matrix::Identity() 
{
  memcpy(m, MATRIX_IDENTITY, MATRIX_SIZE);
}

bool Matrix::IsIdentity() const
{
  return (memcmp(m, MATRIX_IDENTITY, MATRIX_SIZE) == 0);
}

Vector3 Matrix::GetTranslation()
{
  Vector3 newVec( m[12], m[13], m[14] );
  return newVec;
}

void Matrix::Translate(float x, float y, float z)
{
  Translate(x, y, z, this);
}

void Matrix::Translate(float x, float y, float z, Matrix* dst) const
{
  Matrix t;
  CreateTranslation(x, y, z, &t);
  Multiply(*this, t, dst);
}

void Matrix::Translate(const Vector3& t)
{
  Translate(t.x, t.y, t.z, this);
}

void Matrix::Translate(const Vector3& t, Matrix* dst) const
{
  Translate(t.x, t.y, t.z, dst);
}

void Matrix::CreateTranslation(const Vector3& translation, Matrix* dst)
{
  memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

  dst->m[12] = translation.x;
  dst->m[13] = translation.y;
  dst->m[14] = translation.z;
}

void Matrix::CreateTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix* dst)
{
  memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

  dst->m[12] = xTranslation;
  dst->m[13] = yTranslation;
  dst->m[14] = zTranslation;
}

void Matrix::Rotate(const Vector3& axis, float angle)
{
  Rotate(axis, angle, this);
}

void Matrix::Rotate(const Vector3& axis, float angle, Matrix* dst) const
{
  Matrix r;
  CreateRotation(axis, angle, &r);
  Multiply(*this, r, dst);
}

void Matrix::RotateY(float angle)
{
  RotateY(angle, this);
}

void Matrix::RotateY(float angle, Matrix* dst) const
{
  Matrix r;
  CreateRotationY(angle, &r);
  Multiply(*this, r, dst);
}

void Matrix::CreateRotation(const Vector3& axis, float angle, Matrix* dst)
{
  float x = axis.x;
  float y = axis.y;
  float z = axis.z;

  // Make sure the input axis is normalized.
  float n = x*x + y*y + z*z;
  if (n != 1.0f)
  {
    // Not normalized.
    n = sqrt(n);
    // Prevent divide too close to zero.
    if (n > 0.000001f)
    {
      n = 1.0f / n;
      x *= n;
      y *= n;
      z *= n;
    }
  }

  float c = cos(angle);
  float s = sin(angle);

  float t = 1.0f - c;
  float tx = t * x;
  float ty = t * y;
  float tz = t * z;
  float txy = tx * y;
  float txz = tx * z;
  float tyz = ty * z;
  float sx = s * x;
  float sy = s * y;
  float sz = s * z;

  dst->m[0] = c + tx*x;
  dst->m[1] = txy + sz;
  dst->m[2] = txz - sy;
  dst->m[3] = 0.0f;

  dst->m[4] = txy - sz;
  dst->m[5] = c + ty*y;
  dst->m[6] = tyz + sx;
  dst->m[7] = 0.0f;

  dst->m[8] = txz + sy;
  dst->m[9] = tyz - sx;
  dst->m[10] = c + tz*z;
  dst->m[11] = 0.0f;

  dst->m[12] = 0.0f;
  dst->m[13] = 0.0f;
  dst->m[14] = 0.0f;
  dst->m[15] = 1.0f;
}

void Matrix::CreateRotationY(float angle, Matrix* dst)
{
  memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

  float c = cos(angle);
  float s = sin(angle);

  dst->m[0]  = c;
  dst->m[2]  = -s;
  dst->m[8]  = s;
  dst->m[10] = c;
}


void Matrix::Scale( const Vector3& scale )
{
  Scale( m, scale, m );
}

inline void Matrix::Scale(const float* m, const Vector3& scale, float* dst)
{
  dst[0]  = m[0]  * scale.x;
  dst[1]  = m[1]  * scale.x;
  dst[2]  = m[2]  * scale.x;
  dst[3]  = m[3]  * scale.x;

  dst[4]  = m[4]  * scale.y;
  dst[5]  = m[5]  * scale.y;
  dst[6]  = m[6]  * scale.y;
  dst[7]  = m[7]  * scale.y;

  dst[8]  = m[8]  * scale.z;
  dst[9]  = m[9]  * scale.z;
  dst[10] = m[10] * scale.z;
  dst[11] = m[11] * scale.z;
}

void Matrix::Multiply(float scalar)
{
  Multiply(scalar, this);
}

void Matrix::Multiply(float scalar, Matrix* dst) const
{
  Multiply(*this, scalar, dst);
}

void Matrix::Multiply(const Matrix& otherMatrix, float scalar, Matrix* dst)
{
  MultiplyMatrix(otherMatrix.m, scalar, dst->m);
}

void Matrix::Multiply(const Matrix& otherMatrix )
{
  Multiply(*this, otherMatrix, this);
}

void Matrix::Multiply(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
  MultiplyMatrix(m1.m, m2.m, dst->m);
}

inline void Matrix::MultiplyMatrix(const float* m, float scalar, float* dst)
{
  dst[0]  = m[0]  * scalar;
  dst[1]  = m[1]  * scalar;
  dst[2]  = m[2]  * scalar;
  dst[3]  = m[3]  * scalar;
  dst[4]  = m[4]  * scalar;
  dst[5]  = m[5]  * scalar;
  dst[6]  = m[6]  * scalar;
  dst[7]  = m[7]  * scalar;
  dst[8]  = m[8]  * scalar;
  dst[9]  = m[9]  * scalar;
  dst[10] = m[10] * scalar;
  dst[11] = m[11] * scalar;
  dst[12] = m[12] * scalar;
  dst[13] = m[13] * scalar;
  dst[14] = m[14] * scalar;
  dst[15] = m[15] * scalar;
}

inline void Matrix::MultiplyMatrix(const float* m1, const float* m2, float* dst)
{
  // Support the case where m1 or m2 is the same array as dst.
  float product[16];

  product[0]  = m1[0] * m2[0]  + m1[4] * m2[1] + m1[8]   * m2[2]  + m1[12] * m2[3];
  product[1]  = m1[1] * m2[0]  + m1[5] * m2[1] + m1[9]   * m2[2]  + m1[13] * m2[3];
  product[2]  = m1[2] * m2[0]  + m1[6] * m2[1] + m1[10]  * m2[2]  + m1[14] * m2[3];
  product[3]  = m1[3] * m2[0]  + m1[7] * m2[1] + m1[11]  * m2[2]  + m1[15] * m2[3];

  product[4]  = m1[0] * m2[4]  + m1[4] * m2[5] + m1[8]   * m2[6]  + m1[12] * m2[7];
  product[5]  = m1[1] * m2[4]  + m1[5] * m2[5] + m1[9]   * m2[6]  + m1[13] * m2[7];
  product[6]  = m1[2] * m2[4]  + m1[6] * m2[5] + m1[10]  * m2[6]  + m1[14] * m2[7];
  product[7]  = m1[3] * m2[4]  + m1[7] * m2[5] + m1[11]  * m2[6]  + m1[15] * m2[7];

  product[8]  = m1[0] * m2[8]  + m1[4] * m2[9] + m1[8]   * m2[10] + m1[12] * m2[11];
  product[9]  = m1[1] * m2[8]  + m1[5] * m2[9] + m1[9]   * m2[10] + m1[13] * m2[11];
  product[10] = m1[2] * m2[8]  + m1[6] * m2[9] + m1[10]  * m2[10] + m1[14] * m2[11];
  product[11] = m1[3] * m2[8]  + m1[7] * m2[9] + m1[11]  * m2[10] + m1[15] * m2[11];

  product[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
  product[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
  product[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
  product[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

  memcpy(dst, product, MATRIX_SIZE);
}

Vector3 Matrix::Multiply( const Matrix& mat, const Vector3& vec )
{
  float x = ( vec.x * mat.m[0] ) + ( vec.y * mat.m[1] ) + ( vec.z * mat.m[2] ) + mat.m[3];
  float y = ( vec.x * mat.m[4] ) + ( vec.y * mat.m[5] ) + ( vec.z * mat.m[6] ) + mat.m[7];
  float z = ( vec.x * mat.m[8] ) + ( vec.y * mat.m[9] ) + ( vec.z * mat.m[10] ) + mat.m[11];
  return Vector3( x, y, z );
}

void Matrix::Perspective( float fov, float aspect, float zNear, float zFar ) 
{
  float degToRad = (float)M_PI / 180.0f;

  float yScale = 1.0f / tan(degToRad * fov / 2);
  float xScale = yScale / aspect;
  
  for ( int i = 0; i < 16; i++ )
  {
    m[i] = 0.0f;
  }
  
  m[0] = xScale;
  m[(1 * 4) + 1] = yScale;
  m[(2 * 4) + 2] = (zFar + zNear)  / (zNear - zFar);
  m[(2 * 4) + 3] = -1.0f;
  m[(3 * 4) + 2] = (2.0f * zFar * zNear) / (zNear - zFar);
}

void Matrix::Ortographic(float left, float right, float top, float bottom, float zNear, float zFar)
{
  m[0] = 2 / (right - left);
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;

  m[4] = 0;
  m[5] = 2 / (top - bottom);
  m[6] = 0;
  m[7] = 0;

  m[8] = 0;
  m[9] = 0;
  m[10] = -1 / (zFar - zNear);
  m[11] = 0;

  m[12] = -(right + left) / (right - left);
  m[13] = -(top + bottom) / (top - bottom);
  m[14] = -zNear / (zFar - zNear);
  m[15] = 1;
}

//void Matrix::createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
//                          float targetPositionX, float targetPositionY, float targetPositionZ,
//                          float upX, float upY, float upZ, Matrix* dst)
//{
//  GP_ASSERT(dst);
//
//  Vector3 eye(eyePositionX, eyePositionY, eyePositionZ);
//  Vector3 target(targetPositionX, targetPositionY, targetPositionZ);
//  Vector3 up(upX, upY, upZ);
//  up.normalize();
//
//  Vector3 zaxis;
//  Vector3::subtract(eye, target, &zaxis);
//  zaxis.normalize();
//
//  Vector3 xaxis;
//  Vector3::cross(up, zaxis, &xaxis);
//  xaxis.normalize();
//
//  Vector3 yaxis;
//  Vector3::cross(zaxis, xaxis, &yaxis);
//  yaxis.normalize();
//
//  dst->m[0] = xaxis.x;
//  dst->m[1] = yaxis.x;
//  dst->m[2] = zaxis.x;
//  dst->m[3] = 0.0f;
//
//  dst->m[4] = xaxis.y;
//  dst->m[5] = yaxis.y;
//  dst->m[6] = zaxis.y;
//  dst->m[7] = 0.0f;
//
//  dst->m[8] = xaxis.z;
//  dst->m[9] = yaxis.z;
//  dst->m[10] = zaxis.z;
//  dst->m[11] = 0.0f;
//
//  dst->m[12] = -Vector3::dot(xaxis, eye);
//  dst->m[13] = -Vector3::dot(yaxis, eye);
//  dst->m[14] = -Vector3::dot(zaxis, eye);
//  dst->m[15] = 1.0f;
//}

void Matrix::LookAt( Matrix& dst, const Vector3& cameraEye, const Vector3& cameraTarget, const Vector3& cameraUp ) 
{
  Vector3 eye( cameraEye );
  Vector3 target(cameraTarget);
  Vector3 up( cameraUp );
  up.Normalize();

  Vector3 zaxis = eye - target;
  zaxis.Normalize();
  Vector3 xaxis = Vector3::Cross( up, zaxis );
  xaxis.Normalize();
  Vector3 yaxis = Vector3::Cross( zaxis, xaxis );
  yaxis.Normalize();

  dst.m[0] = xaxis.x;
  dst.m[1] = yaxis.x;
  dst.m[2] = zaxis.x;
  dst.m[3] = 0.0f;

  dst.m[4] = xaxis.y;
  dst.m[5] = yaxis.y;
  dst.m[6] = zaxis.y;
  dst.m[7] = 0.0f;

  dst.m[8] = xaxis.z;
  dst.m[9] = yaxis.z;
  dst.m[10] = zaxis.z;
  dst.m[11] = 0.0f;

  dst.m[12] = -Vector3::Dot(xaxis, eye);
  dst.m[13] = -Vector3::Dot(yaxis, eye);
  dst.m[14] = -Vector3::Dot(zaxis, eye);
  dst.m[15] = 1.0f;
}

void Matrix::Invert()
{
  float a0 = m[0] * m[5] - m[1] * m[4];
  float a1 = m[0] * m[6] - m[2] * m[4];
  float a2 = m[0] * m[7] - m[3] * m[4];
  float a3 = m[1] * m[6] - m[2] * m[5];
  float a4 = m[1] * m[7] - m[3] * m[5];
  float a5 = m[2] * m[7] - m[3] * m[6];
  float b0 = m[8] * m[13] - m[9] * m[12];
  float b1 = m[8] * m[14] - m[10] * m[12];
  float b2 = m[8] * m[15] - m[11] * m[12];
  float b3 = m[9] * m[14] - m[10] * m[13];
  float b4 = m[9] * m[15] - m[11] * m[13];
  float b5 = m[10] * m[15] - m[11] * m[14];

  // Calculate the determinant.
  float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

  // Close to zero, can't invert.
  if (fabs(det) <= 0.0001f)
  {
    return;
  }

  // Support the case where m == dst.
  Matrix inverse;
  inverse.m[0]  = m[5] * b5 - m[6] * b4 + m[7] * b3;
  inverse.m[1]  = -m[1] * b5 + m[2] * b4 - m[3] * b3;
  inverse.m[2]  = m[13] * a5 - m[14] * a4 + m[15] * a3;
  inverse.m[3]  = -m[9] * a5 + m[10] * a4 - m[11] * a3;

  inverse.m[4]  = -m[4] * b5 + m[6] * b2 - m[7] * b1;
  inverse.m[5]  = m[0] * b5 - m[2] * b2 + m[3] * b1;
  inverse.m[6]  = -m[12] * a5 + m[14] * a2 - m[15] * a1;
  inverse.m[7]  = m[8] * a5 - m[10] * a2 + m[11] * a1;

  inverse.m[8]  = m[4] * b4 - m[5] * b2 + m[7] * b0;
  inverse.m[9]  = -m[0] * b4 + m[1] * b2 - m[3] * b0;
  inverse.m[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
  inverse.m[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;

  inverse.m[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
  inverse.m[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
  inverse.m[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
  inverse.m[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;

  inverse.Multiply( 1.0f / det );
  //Multiply( )
  //multiply(inverse, 1.0f / det, dst);
  //return inverse;
}

//void mat4Print(float* a) 
//{
//  // opengl uses column major order
//  for (int i = 0; i < 4; ++i) {
//    for (int j = 0; j < 4; ++j) {
//      cout << a[j * 4 + i] << " ";
//    }
//    cout << endl;
//  }
//}

float MatDeterminant( const Matrix& a )
{
  return a.m[3] * a.m[6] * a.m[9] * a.m[12] - a.m[2] * a.m[7] * a.m[9] * a.m[12] -
    a.m[3] * a.m[5] * a.m[10] * a.m[12] + a.m[1] * a.m[7] * a.m[10] * a.m[12] +
    a.m[2] * a.m[5] * a.m[11] * a.m[12] - a.m[1] * a.m[6] * a.m[11] * a.m[12] -
    a.m[3] * a.m[6] * a.m[8] * a.m[13] + a.m[2] * a.m[7] * a.m[8] * a.m[13] +
    a.m[3] * a.m[4] * a.m[10] * a.m[13] - a.m[0] * a.m[7] * a.m[10] * a.m[13] -
    a.m[2] * a.m[4] * a.m[11] * a.m[13] + a.m[0] * a.m[6] * a.m[11] * a.m[13] +
    a.m[3] * a.m[5] * a.m[8] * a.m[14] - a.m[1] * a.m[7] * a.m[8] * a.m[14] -
    a.m[3] * a.m[4] * a.m[9] * a.m[14] + a.m[0] * a.m[7] * a.m[9] * a.m[14] +
    a.m[1] * a.m[4] * a.m[11] * a.m[14] - a.m[0] * a.m[5] * a.m[11] * a.m[14] -
    a.m[2] * a.m[5] * a.m[8] * a.m[15] + a.m[1] * a.m[6] * a.m[8] * a.m[15] +
    a.m[2] * a.m[4] * a.m[9] * a.m[15] - a.m[0] * a.m[6] * a.m[9] * a.m[15] -
    a.m[1] * a.m[4] * a.m[10] * a.m[15] + a.m[0] * a.m[5] * a.m[10] * a.m[15];
}
 
// 11 - 0      21 - 4     31 - 8      41 - 12
// 12 - 1      22 - 5     32 - 9      42 - 13
// 13 - 2      23 - 6     33 - 10     43 - 14
// 14 - 3      24 - 7     34 - 11     44 - 15

const Matrix MatInverse( const Matrix& a )
{
  Matrix  mat( a.m[6]*a.m[11]*a.m[13] - a.m[7]*a.m[10]*a.m[13] + a.m[7]*a.m[9]*a.m[14] - a.m[5]*a.m[11]*a.m[14] - a.m[6]*a.m[9]*a.m[15] + a.m[5]*a.m[10]*a.m[15],
    a.m[3]*a.m[10]*a.m[13] - a.m[2]*a.m[11]*a.m[13] - a.m[3]*a.m[9]*a.m[14] + a.m[1]*a.m[11]*a.m[14] + a.m[2]*a.m[9]*a.m[15] - a.m[1]*a.m[10]*a.m[15],
    a.m[2]*a.m[7]*a.m[13] - a.m[3]*a.m[6]*a.m[13] + a.m[3]*a.m[5]*a.m[14] - a.m[1]*a.m[7]*a.m[14] - a.m[2]*a.m[5]*a.m[15] + a.m[1]*a.m[6]*a.m[15],
    a.m[3]*a.m[6]*a.m[9] - a.m[2]*a.m[7]*a.m[9] - a.m[3]*a.m[5]*a.m[10] + a.m[1]*a.m[7]*a.m[10] + a.m[2]*a.m[5]*a.m[11] - a.m[1]*a.m[6]*a.m[11],
    a.m[7]*a.m[10]*a.m[12] - a.m[6]*a.m[11]*a.m[12] - a.m[7]*a.m[8]*a.m[14] + a.m[4]*a.m[11]*a.m[14] + a.m[6]*a.m[8]*a.m[15] - a.m[4]*a.m[10]*a.m[15],
    a.m[2]*a.m[11]*a.m[12] - a.m[3]*a.m[10]*a.m[12] + a.m[3]*a.m[8]*a.m[14] - a.m[0]*a.m[11]*a.m[14] - a.m[2]*a.m[8]*a.m[15] + a.m[0]*a.m[10]*a.m[15],
    a.m[3]*a.m[6]*a.m[12] - a.m[2]*a.m[7]*a.m[12] - a.m[3]*a.m[4]*a.m[14] + a.m[0]*a.m[7]*a.m[14] + a.m[2]*a.m[4]*a.m[15] - a.m[0]*a.m[6]*a.m[15],
    a.m[2]*a.m[7]*a.m[8] - a.m[3]*a.m[6]*a.m[8] + a.m[3]*a.m[4]*a.m[10] - a.m[0]*a.m[7]*a.m[10] - a.m[2]*a.m[4]*a.m[11] + a.m[0]*a.m[6]*a.m[11],
    a.m[5]*a.m[11]*a.m[12] - a.m[7]*a.m[9]*a.m[12] + a.m[7]*a.m[8]*a.m[13] - a.m[4]*a.m[11]*a.m[13] - a.m[5]*a.m[8]*a.m[15] + a.m[4]*a.m[9]*a.m[15],
    a.m[3]*a.m[9]*a.m[12] - a.m[1]*a.m[11]*a.m[12] - a.m[3]*a.m[8]*a.m[13] + a.m[0]*a.m[11]*a.m[13] + a.m[1]*a.m[8]*a.m[15] - a.m[0]*a.m[9]*a.m[15],
    a.m[1]*a.m[7]*a.m[12] - a.m[3]*a.m[5]*a.m[12] + a.m[3]*a.m[4]*a.m[13] - a.m[0]*a.m[7]*a.m[13] - a.m[1]*a.m[4]*a.m[15] + a.m[0]*a.m[5]*a.m[15],
    a.m[3]*a.m[5]*a.m[8] - a.m[1]*a.m[7]*a.m[8] - a.m[3]*a.m[4]*a.m[9] + a.m[0]*a.m[7]*a.m[9] + a.m[1]*a.m[4]*a.m[11] - a.m[0]*a.m[5]*a.m[11],
    a.m[6]*a.m[9]*a.m[12] - a.m[5]*a.m[10]*a.m[12] - a.m[6]*a.m[8]*a.m[13] + a.m[4]*a.m[10]*a.m[13] + a.m[5]*a.m[8]*a.m[14] - a.m[4]*a.m[9]*a.m[14],
    a.m[1]*a.m[10]*a.m[12] - a.m[2]*a.m[9]*a.m[12] + a.m[2]*a.m[8]*a.m[13] - a.m[0]*a.m[10]*a.m[13] - a.m[1]*a.m[8]*a.m[14] + a.m[0]*a.m[9]*a.m[14],
    a.m[2]*a.m[5]*a.m[12] - a.m[1]*a.m[6]*a.m[12] - a.m[2]*a.m[4]*a.m[13] + a.m[0]*a.m[6]*a.m[13] + a.m[1]*a.m[4]*a.m[14] - a.m[0]*a.m[5]*a.m[14],
    a.m[1]*a.m[6]*a.m[8] - a.m[2]*a.m[5]*a.m[8] + a.m[2]*a.m[4]*a.m[9] - a.m[0]*a.m[6]*a.m[9] - a.m[1]*a.m[4]*a.m[10] + a.m[0]*a.m[5]*a.m[10] );

  //float f = 1.0f / MatDeterminant( a );
  //mat.Scale( Vector3(f, f, f ) );
  return mat;
}
