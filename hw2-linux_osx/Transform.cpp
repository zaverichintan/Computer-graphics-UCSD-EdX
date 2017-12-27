// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.
#include<iostream>
#include "Transform.h"
using namespace std;
#include <glm/gtc/reciprocal.hpp>

// Helper rotation function.  Please implement this.
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
  mat3 ret;
  // YOUR CODE FOR HW2 HERE
  mat3 I(1.0);

  float x = axis.x;
  float y = axis.y;
  float z = axis.z;

  mat3 matrix1 = I * cos(degrees);
  mat3 matrix2 = (1 - cos(degrees)) * mat3(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z);
  mat3 matrix3 = sin(degrees) * mat3(0.0,z,-1*y, -1*z,0.0,x, y,-1*x,0.0);

  ret = matrix1 + matrix2 + matrix3;

  return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
  // YOUR CODE FOR HW2 HERE
  mat3 ro = rotate(glm::radians(degrees), up); //rotation matrix for rotation along "up" axis
  eye = ro * eye;	//
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
  // YOUR CODE FOR HW2 HERE
  float rdegrees = glm::radians(degrees);
  vec3 left = normalize(cross(eye, up));
  mat3 ro2 = rotate(rdegrees, left);

  eye = ro2 * eye;
  up = normalize(ro2 * up); // to change the up vector too, as when the tea pot initial up vector points towards or away(180 degree of

}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  vec3 centre = vec3(0.0);
	vec3 a = centre - eye;
	vec3 b = normalize(up);

	vec3 w = normalize(a);
	vec3 u = normalize(cross(w,b));
	vec3 v = cross(u,w);
	vec3 e = eye;
	ret = mat4(u.x,v.x,-1.0 * w.x,0.0,	\
			   u.y,v.y,-1.0 * w.y,0.0, \
			   u.z,v.z,-1.0 * w.z,0.0, \
	                   -1.0 * u.x*e.x - u.y*e.y - u.z*e.z, \
	                   -1.0 * v.x*e.x - v.y*e.y - v.z*e.z, \
	                   w.x*e.x + w.y*e.y + w.z*e.z, 1.0);
  return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.
  float theta = glm::radians(fovy) / 2 ;
  float d = 1 / tanf(theta);
  float A = -((zFar + zNear) / (zFar - zNear));
	float B = -((2 * zFar*zNear) / (zFar - zNear));

  ret = glm::mat4((d /aspect), 0, 0, 0, 0, d, 0, 0, 0, 0, A, -1, 0, 0, B, 0);
  return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
  mat4 ret = mat4(sx,0.0,0.0,0.0,\
                  0.0,sy,0.0,0.0,\
                  0.0,0.0,sz,0.0,\
                  0.0,0.0,0.0,1.0);

  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
  //mat4 ret = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
  mat4 ret = mat4(1.0,0.0,0.0,0.0,\
             0.0,1.0,0.0,0.0,\
             0.0,0.0,1.0,0.0,\
             tx,ty,tz,1.0);

  return ret;
}

// To normalize the up direction and construct a coordinate frame.
// As discussed in the lecture.  May be relevant to create a properly
// orthogonal and normalized up.
// This function is provided as a helper, in case you want to use it.
// Using this function (in readfile.cpp or display.cpp) is optional.

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
  vec3 x = glm::cross(up,zvec);
  vec3 y = glm::cross(zvec,x);
  vec3 ret = glm::normalize(y);
  return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
