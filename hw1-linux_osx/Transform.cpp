// Transform.cpp: implementation of the Transform class.

#include<iostream>
#include "Transform.h"
using namespace std;
//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	// YOUR CODE FOR HW1 HERE
	mat3 I(1.0);

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	mat3 matrix1 = I * cos(degrees); 		
	mat3 matrix2 = (1 - cos(degrees)) * mat3(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z);	
	mat3 matrix3 = sin(degrees) * mat3(0.0,z,-1*y, -1*z,0.0,x, y,-1*x,0.0);
	
	mat3 rotation_matrix = matrix1 + matrix2 + matrix3;
	
	// You will change this return call
	return rotation_matrix;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE
	mat3 ro = rotate(glm::radians(degrees), up); //rotation matrix for rotation along "up" axis
	eye = ro * eye;	//
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE 
	float rdegrees = glm::radians(degrees);
	vec3 left = normalize(cross(eye, up));
	mat3 ro2 = rotate(rdegrees, left);

	eye = ro2 * eye;
	up = normalize(ro2 * up); // to change the up vector too, as when the tea pot initial up vector points towards or away(180 degree of 

	printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
	// YOUR CODE FOR HW1 HERE
	vec3 centre = vec3(0.0);
	vec3 a = centre - eye;
	vec3 b = normalize(up);

	vec3 w = normalize(a); 
	vec3 u = normalize(cross(w,b)); 
	vec3 v = cross(u,w);
	vec3 e = eye;
	mat4 result = mat4(u.x,v.x,-1.0 * w.x,0.0,	\
			   u.y,v.y,-1.0 * w.y,0.0, \
			   u.z,v.z,-1.0 * w.z,0.0, \
	                   -1.0 * u.x*e.x - u.y*e.y - u.z*e.z, \
	                   -1.0 * v.x*e.x - v.y*e.y - v.z*e.z, \
	                   w.x*e.x + w.y*e.y + w.z*e.z, 1.0);
	// You will change this return call
	return result;
}
Transform::Transform()
{

}

Transform::~Transform()
{

}
